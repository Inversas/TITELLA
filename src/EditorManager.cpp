#include "EditorManager.h"

// *** CONSTRUCTOR ***
EditorManager::EditorManager() {
    // Inicialización de seguridad: punteros a null
    collisionManager = nullptr;
    guiManager = nullptr;

    // Estado inicial por defecto
    bEnabled = false;
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
    // Vinculamos los managers que vienen de ofApp
    collisionManager = _collisionManager;
    guiManager = _guiManager;
    
    // Nos aseguramos de que el editor empiece apagado y limpio
    bEnabled = false;
    currentState = EDITOR_WAITING_P1;
    bNeedsGuiUpdate = false;
    
    ofLogNotice("EditorManager") << "Editor inicializado y vinculado correctamente.";
}

// *** UPDATE ***
void EditorManager::update(int mouseX, int mouseY) {
    if (!bEnabled) return;

    // 1. Calculamos la posición imantada del ratón constantemente
    // Esto permite que el usuario vea el "imán" antes de hacer clic
    snappedMousePos = getSnappedPoint(ofVec2f(mouseX, mouseY));

    // 2. Si ya fijamos el P1, calculamos el P2 con las restricciones
    if (currentState == EDITOR_WAITING_P2) {
        // Aplicamos la restricción axial sobre el punto ya imantado
        p2Preview = applyAxialRestriction(p1, snappedMousePos);
    }
}

// *** DRAW ***
void EditorManager::draw() {
    if (!bEnabled) return;

    ofPushStyle();
    
    // --- INDICADOR DE SNAP ---
    // Dibujamos un pequeño círculo donde "caerá" el punto realmente
    ofSetColor(guideLineColor, 180);
    ofNoFill();
    ofDrawCircle(snappedMousePos, 6); // Aro exterior
    ofFill();
    ofDrawCircle(snappedMousePos, 2); // Punto central

    // --- LÓGICA DE DIBUJO SEGÚN ESTADO ---
    if (currentState == EDITOR_WAITING_P2) {
        // Dibujamos la línea guía (Línea Amarilla)
        ofSetLineWidth(2);
        ofSetColor(guideLineColor);
        ofDrawLine(p1, p2Preview);
        
        // Dibujamos un punto sólido en P1 para marcar el origen
        ofDrawCircle(p1, 4);
        
        // Pequeño texto de ayuda al lado del cursor
        ofSetColor(255);
        ofDrawBitmapString("P2: Click para crear\nBACK: Cancelar", snappedMousePos.x + 15, snappedMousePos.y);
    }
    else if (currentState == EDITOR_WAITING_P1) {
        // Texto de ayuda en el estado inicial
        ofSetColor(255, 255, 255, 200);
        ofDrawBitmapString("P1: Click para iniciar linea", snappedMousePos.x + 15, snappedMousePos.y);
    }

    ofPopStyle();
}



void EditorManager::handleMousePressed(int x, int y, int button) {
    if (!bEnabled || button != 0) return; // Solo respondemos al click izquierdo

    if (currentState == EDITOR_WAITING_P1) {
        // PASO 1: Fijamos el punto inicial
        p1 = snappedMousePos;
        currentState = EDITOR_WAITING_P2;
        ofLogNotice("EditorManager") << "P1 fijado en: " << p1;
    }
    else if (currentState == EDITOR_WAITING_P2) {
        // PASO 2: Crear el interactor real
        // Usamos p2Preview porque ya tiene el Snap y la Restriccion Axial aplicada
        ofVec2f p2 = p2Preview;

        // Evitamos crear interactores sin longitud
        if (p1.distance(p2) > 5) {
            
            // 1. DETERMINAR TIPO SEGÚN ORIENTACIÓN
            // Calculamos la diferencia absoluta en cada eje
            float diffX = abs(p2.x - p1.x);
            float diffY = abs(p2.y - p1.y);
            
            InteractorType type;
            float radius = 300.0f; // Valor por defecto para superficies
            
            if (diffX >= diffY) {
                // Es predominantemente horizontal -> SUELO
                type = InteractorType::SURFACE;
            } else {
                // Es predominantemente vertical -> PARED
                type = InteractorType::WALL;
                // Para las paredes, usamos el ancho de región como radio de influencia
                // (puedes ajustarlo si prefieres otro valor)
                radius = 300.0f;
            }

 
            // 2. GENERAR NOMBRE COHERENTE
            // Ahora pasamos el 'type' real para que el nombre sea SURFACE_X o WALL_X
            string newName = generateUniqueName(type);
            
            // Creamos el objeto
            

            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // ELegir tipo BUTTON con shift
            // Tema rampas
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
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


void EditorManager::handleKeyPressed(int key) {
    if (!bEnabled) return;

    // Si presionamos BACKSPACE cancelamos el P1
    if (key == OF_KEY_BACKSPACE) {
        if (currentState == EDITOR_WAITING_P2) {
            currentState = EDITOR_WAITING_P1;
            ofLogNotice("EditorManager") << "Creacion cancelada.";
        }
    }
}


void EditorManager::deleteInteractor(string name) {
    if (!collisionManager) return;

    // Llamamos al CollisionManager para que lo elimine de su vector
    // NOTA: Asegúrate de añadir el método removeInteractor en CollisionManager
    collisionManager->removeInteractor(name);
    
    // Avisamos que la GUI debe refrescarse (el botón debe desaparecer)
    bNeedsGuiUpdate = true;
    
    ofLogNotice("EditorManager") << "Eliminado interactor: " << name;
}




// Calcula el punto con imán (Snap) hacia otros interactores
ofVec2f EditorManager::getSnappedPoint(ofVec2f rawPos) {
    ofVec2f snappedPos = rawPos;
    float minDist = snapTolerance;
    bool found = false;

    if (!collisionManager) return rawPos;

    for (const auto& inter : collisionManager->getInteractors()) {
        
        // 1. SNAP A EXTREMOS (Prioridad Máxima)
        // Seguimos comprobando P1 y P2 porque si estamos cerca de una esquina,
        // queremos precisión total en el vértice.
        float d1 = rawPos.distance(inter.p1);
        if (d1 < minDist) {
            minDist = d1;
            snappedPos = inter.p1;
            found = true;
        }
        float d2 = rawPos.distance(inter.p2);
        if (d2 < minDist) {
            minDist = d2;
            snappedPos = inter.p2;
            found = true;
        }

        // 2. SNAP A CUERPO (Si no hemos encontrado un extremo ya muy cerca)
        if (!found) {
            if (inter.type == InteractorType::SURFACE) {
                // Es horizontal: Nos fijamos en la distancia vertical (Y)
                // Comprobamos si el ratón está dentro del rango horizontal de la plataforma
                float minX = min(inter.p1.x, inter.p2.x);
                float maxX = max(inter.p1.x, inter.p2.x);
                
                if (rawPos.x >= minX && rawPos.x <= maxX) {
                    float distY = abs(rawPos.y - inter.p1.y);
                    if (distY < minDist) {
                        minDist = distY;
                        snappedPos.y = inter.p1.y; // Imantamos solo la Y
                        found = true;
                    }
                }
            }
            else if (inter.type == InteractorType::WALL) {
                // Es vertical: Nos fijamos en la distancia horizontal (X)
                // Comprobamos si el ratón está dentro del rango vertical de la pared
                float minY = min(inter.p1.y, inter.p2.y);
                float maxY = max(inter.p1.y, inter.p2.y);
                
                if (rawPos.y >= minY && rawPos.y <= maxY) {
                    float distX = abs(rawPos.x - inter.p1.x);
                    if (distX < minDist) {
                        minDist = distX;
                        snappedPos.x = inter.p1.x; // Imantamos solo la X
                        found = true;
                    }
                }
            }
        }
    }

    return snappedPos;
}


// Aplica la restricción de ejes (Horizontal / Vertical)
ofVec2f EditorManager::applyAxialRestriction(ofVec2f start, ofVec2f end) {
    ofVec2f restrictedPos = end;

    // Calculamos la diferencia en cada eje
    float dx = abs(end.x - start.x);
    float dy = abs(end.y - start.y);

    // Si el movimiento es más horizontal que vertical
    if (dx > dy) {
        // Forzamos la misma Y del origen
        restrictedPos.y = start.y;
    }
    // Si el movimiento es más vertical que horizontal
    else {
        // Forzamos la misma X del origen
        restrictedPos.x = start.x;
    }

    return restrictedPos;
}


// ------------------- GETTERS - SETTERS -------------------

// *** GETTERS ***
bool EditorManager::getEnabled() const {
    return bEnabled;
}

EditorState EditorManager::getState() const {
    return currentState;
}

bool EditorManager::hasPendingChanges() const {
    return bNeedsGuiUpdate;
}

// *** SETTERS ***
void EditorManager::setEnabled(bool value) {
    bEnabled = value;
    // Si apagamos el editor a mitad de una creación, lo reseteamos a IDLE
    if (!bEnabled) {
        currentState = EDITOR_WAITING_P1;
    }
}


void EditorManager::setChangesProcessed() {
    bNeedsGuiUpdate = false;
}


// *** MÉTODOS INTERNOS ***
string EditorManager::generateUniqueName(InteractorType type) {
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






