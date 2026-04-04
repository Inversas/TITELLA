#include "EditorManager.h"

// *** CONSTRUCTOR ***
EditorManager::EditorManager() {
    // Inicialización de seguridad: punteros a null
    collisionManager = nullptr;
    guiManager = nullptr;

    // Estado inicial por defecto
    editMode = false;
    currentState = EDITOR_WAITING_P1;
    bNeedsGuiUpdate = false;

    // Geometría a cero
    p1.set(0, 0);
    p2Preview.set(0, 0);
    snappedMousePos.set(0, 0);

    // Configuración visual y técnica
    snapTolerance = 20.0f; // 20 píxeles de radio de acción para el imán
    guideLineColor = ofColor(255, 255, 0); // Amarillo clásico de guía
}

// *** DESTRUCTOR ***
EditorManager::~EditorManager() {
    // Limpiamos los punteros por cortesía, aunque no somos los dueños de los objetos
    collisionManager = nullptr;
    guiManager = nullptr;
}

// *** SETUP ***
void EditorManager::setup(CollisionManager* _collisionManager, GUIManager* _guiManager) {
    // VINCULAR MANAGERS
    collisionManager = _collisionManager;
    guiManager = _guiManager;
    
    // Nos aseguramos de que el editor empiece apagado y limpio
    editMode = false;
    currentState = EDITOR_WAITING_P1;
    bNeedsGuiUpdate = false;
    
    ofLogNotice("EditorManager") << "Editor inicializado y vinculado correctamente.";
}

// *** UPDATE ***
void EditorManager::update(int mouseX, int mouseY) {
    
    // Si el editor no está activo, no hacemos nada
    if (!editMode) return;

    // 1. Calculamos la posición imantada del ratón constantemente
    snappedMousePos = getSnappedPoint(ofVec2f(mouseX, mouseY));

    // 2. Si ya fijamos el P1, calculamos el P2 con las restricciones
    if (currentState == EDITOR_WAITING_P2) {
        // Aplicamos la restricción axial (HORIZONTAL O VERTICAL) sobre el punto ya imantado
        p2Preview = applyAxialRestriction(p1, snappedMousePos);
    }
}

// *** DRAW ***
void EditorManager::draw() {
    // Si el editor no está activo, no hacemos nada
    if (!editMode) return;

    // Foto del estilo actual
    ofPushStyle();
    
    // --- INDICADOR DE SNAP ---
    // Dibujamos un pequeño círculo donde "caerá" el punto realmente
    ofSetColor(guideLineColor, 180);
    ofNoFill();
    ofDrawCircle(snappedMousePos, 6); // Aro exterior
    ofFill();
    ofDrawCircle(snappedMousePos, 2); // Punto central

    // --- LÓGICA DE DIBUJO SEGÚN ESTADO ---
    // PINTAR P2
    if (currentState == EDITOR_WAITING_P2) {
        // Dibujamos la línea guía (Línea Amarilla)
        ofSetLineWidth(2);
        ofSetColor(guideLineColor);
        ofDrawLine(p1, p2Preview);
        
        // Dibujamos un punto sólido en P1 para marcar el origen
        ofDrawCircle(p1, 4);
        
        // TEXTO AYUDA CURSOR
        ofSetColor(255);
        ofDrawBitmapString("P2: Click para crear\nBACK: Cancelar", snappedMousePos.x + 15, snappedMousePos.y);
    }
    // PINTAR P1
    else if (currentState == EDITOR_WAITING_P1) {
        // Texto de ayuda en el estado inicial
        ofSetColor(255, 255, 255, 200);
        ofDrawBitmapString("P1: Click para iniciar linea", snappedMousePos.x + 15, snappedMousePos.y);
    }
    // Recuperar estilo anterior
    ofPopStyle();
}


// *** INPUT HANDLERS ***
// MOUSE PRESSED
void EditorManager::handleMousePressed(int x, int y, int button) {
    
    // Solo respondemos al click izquierdo (button es del raton)
    if (!editMode || button != 0) return;

    // FIJAR PUNTO 1
    if (currentState == EDITOR_WAITING_P1) {
        // Fijamos el punto inicial
        p1 = snappedMousePos;
        // ACTUALIZAR ESTADO EDITOR
        currentState = EDITOR_WAITING_P2;
        ofLogNotice("EditorManager") << "P1 fijado en: " << p1;
    }
    // FIJAR PUNTO 1
    else if (currentState == EDITOR_WAITING_P2) {
        // Crear el interactor real
        // Usamos p2Preview porque ya tiene el Snap y la Restriccion Axial aplicada
        ofVec2f p2 = p2Preview;

        // Evitamos crear interactores sin longitud
        if (p1.distance(p2) > 5) {
            
            // *** DETERMINAR TIPO SEGÚN ORIENTACIÓN ***
            // Calculamos la diferencia absoluta en cada eje
            float diffX = abs(p2.x - p1.x);
            float diffY = abs(p2.y - p1.y);
            
            // Nuevo Interactor
            InteractorType type;
            // Valor por defecto para superficies
            float radius = 0;
            
            // Es predominantemente horizontal -> SUELO
            if (diffX >= diffY) {
                type = InteractorType::SURFACE;
            }
            // Es predominantemente vertical -> PARED
            else {
                type = InteractorType::WALL;
                radius = 300.0f;
            }
 
            // *** GENERAR NOMBRE UNICO ***
            // Ahora pasamos el 'type' definido  para que el nombre sea SURFACE_X o WALL_X
            string newName = generateUniqueName(type);
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // ELegir tipo BUTTON con shift
            // Tema rampas
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            
            // Creamos el objeto
            Interactor newInter(p1, p2, type, newName);
            
            // Lo añadimos al CollisionManager
            collisionManager->addInteractor(newInter);
            
            // Marcamos que la GUI debe refrescarse para mostrar el nuevo botón
            bNeedsGuiUpdate = true;
            
            ofLogNotice("EditorManager") << "Creado nuevo " << (type == InteractorType::SURFACE ? "SURFACE" : "WALL") << ": " << newName;
        }
        // Volvemos al estado inicial para permitir crear otro seguido
        currentState = EDITOR_WAITING_P1;
    }
}

// TECLADO: BACKSPACE PARA CANCELAR LA CREACIÓN DE P2
void EditorManager::handleKeyPressed(int key) {
    // Si el editor no está activo, no hacemos nada
    if (!editMode) return;

    // [ BACKSPACE ] --> cancelamos el P1
    if (key == OF_KEY_BACKSPACE) {
        if (currentState == EDITOR_WAITING_P2) {
            currentState = EDITOR_WAITING_P1;
            ofLogNotice("EditorManager") << "Creacion cancelada.";
        }
    }
}

// *** LÓGICA DE EDICIÓN ***
void EditorManager::deleteInteractor(string name) {
    // Solo puede si el collisionManager esta referenciado
    if (!collisionManager) return;

    // Llamamos al CollisionManager para que lo elimine de su vector
    collisionManager->removeInteractor(name);
    
    // Avisamos que la GUI debe refrescarse (el botón debe desaparecer)
    bNeedsGuiUpdate = true;
    
    ofLogNotice("EditorManager") << "Eliminado interactor: " << name;
}


// *** PUNTO DE IMAN ***
ofVec2f EditorManager::getSnappedPoint(ofVec2f rawPos) {
    // Posición real del ratón
    ofVec2f snappedPos = rawPos;
    // Distancia de actuación
    float minDist = snapTolerance;
    // Flag punto de imán válido
    bool found = false;
    
    // Si no hay referencia a CollisionManager, no se puede imantar a nada
    if (!collisionManager) return rawPos;

    // Recorremos Interactors
    for (const auto& inter : collisionManager->getInteractors()) {
        
        // 1. SNAP A EXTREMOS (Prioridad Máxima)
        // Seguimos comprobando P1 y P2 porque si estamos cerca de una esquina,
        // queremos precisión total en el vértice.
        
        // Calculamos la distancia entre el ratón y el P1
        float d1 = rawPos.distance(inter.p1);
        // CERCA DE P1
        if (d1 < minDist) {
            // Guardamos minima distancia encontrada
            minDist = d1;
            // Forzamos punto
            snappedPos = inter.p1;
            // Flag punto válido encontrado
            found = true;
        }
        // Calculamos la distancia entre el ratón y P2
        float d2 = rawPos.distance(inter.p2);
        // CERCA DE P2
        if (d2 < minDist) {
            // Guardamos minima distancia encontrada
            minDist = d2;
            // Forzamos punto
            snappedPos = inter.p2;
            // Flag punto válido encontrado
            found = true;
        }

        // 2. SNAP A RECTA (SIN P1 ni P2 CERCA)
        // Si no hemos encontrado punto válido aun
        if (!found) {
            // EVALUAR SURFACE
            if (inter.type == InteractorType::SURFACE) {
                
                // Obtenemos el límite izquierdo (X mínima)
                float minX = min(inter.p1.x, inter.p2.x);
                // Obtenemos el límite derecho (X máxima)
                float maxX = max(inter.p1.x, inter.p2.x);
                
                // CERCA DE X Interactor
                if (rawPos.x >= minX && rawPos.x <= maxX) {
                    // Calculamos la distancia vertical entre el ratón y P1
                    float distY = abs(rawPos.y - inter.p1.y);
                    // CERCA DE Y Interactor
                    // Si el ratón está "casi" tocando la línea horizontal...
                    if (distY < minDist) {
                        // Guardamos minima distancia encontrada
                        minDist = distY;
                        // Forzamos solo la Y
                        snappedPos.y = inter.p1.y;
                        // Flag punto válido encontrado
                        found = true;
                    }
                }
            }
            // EVALUAR WALL
            else if (inter.type == InteractorType::WALL) {
                // Obtenemos el límite superior (Y mínima)
                float minY = min(inter.p1.y, inter.p2.y);
                // Obtenemos el límite inferior (Y máxima)
                float maxY = max(inter.p1.y, inter.p2.y);
                
                // CERCA DE Y Interactor
                if (rawPos.y >= minY && rawPos.y <= maxY) {
                    // Calculamos la distancia horizontal entre el ratón y P1
                    float distX = abs(rawPos.x - inter.p1.x);
                    // CERCA DE X Interactor
                    // Si el ratón está "casi" tocando la línea vertical...
                    if (distX < minDist) {
                        // Guardamos minima distancia encontrada
                        minDist = distX;
                        // Forzamos solo la X
                        snappedPos.x = inter.p1.x;
                        // Flag punto válido encontrado
                        found = true;
                    }
                }
            }
        }
    }
    // Posición final encontrada
    return snappedPos;
}

// *** RESTRICCION HORIZOTNAL / VERTICAL ***
ofVec2f EditorManager::applyAxialRestriction(ofVec2f start, ofVec2f end) {
    
    // Posicion final actual (P2)
    ofVec2f restrictedPos = end;

    // Calculamos la diferencia en cada eje
    float dx = abs(end.x - start.x);
    float dy = abs(end.y - start.y);

    // MAYOR MOV EN X QUE EN Y
    if (dx > dy) {
        // Forzamos la misma Y del origen
        restrictedPos.y = start.y;
    }
    // MAYOR MOV EN Y QUE EN X
    else {
        // Forzamos la misma X del origen
        restrictedPos.x = start.x;
    }
    // Restricción final encontrada
    return restrictedPos;
}


// ------------------- GETTERS - SETTERS -------------------

// *** GETTERS ***
bool EditorManager::getEnabled() const {
    return editMode;
}

EditorState EditorManager::getState() const {
    return currentState;
}

bool EditorManager::hasPendingChanges() const {
    return bNeedsGuiUpdate;
}

// *** SETTERS ***

void EditorManager::setEnabled(bool value) {
    editMode = value;
    // Si apagamos el editor a mitad de una creación, lo reseteamos a WAITING P1
    if (!editMode) {
        currentState = EDITOR_WAITING_P1;
    }
}

void EditorManager::setChangesProcessed() {
    bNeedsGuiUpdate = false;
}


// *** MÉTODOS INTERNOS ***
string EditorManager::generateUniqueName(InteractorType type) {
    // Inicializamos el string vacio
    string prefix = "";
    
    // 1. Determinamos el prefijo según el tipo
    switch (type) {
        case InteractorType::SURFACE: prefix = "SURFACE"; break;
        case InteractorType::WALL:    prefix = "WALL";    break;
        case InteractorType::BUTTON:  prefix = "BUTTON";  break;
        default:                      prefix = "OBJ";     break;
    }
    
    // 2. Obtenemos el índice actual del vector para evitar repetidos
    // Usamos el tamaño actual como sufijo
    int count = 0;
    if (collisionManager) {
        count = collisionManager->getInteractors().size();
    }
    
    // Resultado: "TIPO_Numero" (ej: SURFACE_5)
    return prefix + "_" + ofToString(count);
}






