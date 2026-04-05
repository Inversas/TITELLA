#include "CollisionManager.h"
// !!!! VERSIÓN OF 0.10 !!!!
// #include "ofxJSON.h"



// *** CONSTRUCTOR ***
CollisionManager::CollisionManager()
    : currentScale(1.0f) // Inicializamos la escala por defecto
{
    // Inicializamos las estructuras de datos con valores seguros (0 o vacíos)
    baseHitbox = {0, 0, 0, 0, 0};
    currentHitbox = {0, 0, 0, 0, 0};
    
    // El CollisionResult tiene punteros internos (wall, floor),
    // los aseguramos a nullptr para que no apunten a basura.
    lastResult.wall = nullptr;
    lastResult.floor = nullptr;
    lastResult.isWalledLeft = false;
    lastResult.isWalledRight = false;
    lastResult.isGrounded = false;

    ofLogNotice("CollisionManager") << "Constructor: Hitboxes y Resultados inicializados a cero.";
}

// *** DESTRUCTOR ***
CollisionManager::~CollisionManager() {
    // 1. Limpiamos los punteros del último resultado de colisión.
    // OJO: No usamos 'delete' porque esos punteros apuntan a elementos
    // que están DENTRO del vector 'interactors'.
    lastResult.wall = nullptr;
    lastResult.floor = nullptr;

    // 2. Vaciamos el vector de interactores explícitamente.
    // Aunque el vector lo hace solo al morir, hacerlo aquí ayuda a
    // depurar y asegura que la memoria se libere ordenadamente.
    interactors.clear();

    ofLogNotice("CollisionManager") << "Destructor: Vector de interactores liberado.";
}

// *** SETUP ***
void CollisionManager::setup(float regionW, float regionH, std::string filename) {
    
    // *** HITBOX BASE, valores sin escalar ***
    baseHitbox.width = regionW + 87;      // Mayor que la region, para no traspasar si la frenada tiene que llegar a un PS lejano
    baseHitbox.height = regionH;          // 300px
    baseHitbox.floorRayX = regionW/2;     // Posición horizontal del rayo de detección del suelo (ajustable por GUI)
    baseHitbox.regionW = regionW;         // Guardamos el ancho de la región para usarlo en los cálculos de colisión
    baseHitbox.regionH = regionH;         // Guardamos la altura de la región para usarlo en los cálculos de colisión
    
    currentScale = 1.0f;
    
    // Debemos dar valor a las hitbox de trabajo con la escala inicial
    updateScaledHitbox();
    
    // *** JSON INTERACTORS - LOAD ***
    loadInteractorsJSON(filename);
    // AJUSTE VALORES LIMITES BASE
    applyMandatorySettings(regionW, regionH);
}

void CollisionManager::applyMandatorySettings(float regionW, float regionH) {
    for (auto & inter : interactors) {
        if (inter.name == "FLOOR_BASE") inter.p2.x = ofGetWidth();
        if (inter.name == "LIMIT_LEFT") { inter.p2.y = ofGetHeight(); inter.influenceRadius = 300; }
        if (inter.name == "LIMIT_RIGHT") { inter.p2.y = ofGetHeight(); inter.influenceRadius = 300; }
    }
}

void CollisionManager::update() {
    // ?????
}

void CollisionManager::draw() {
    // ?????
}

void CollisionManager::drawInteractors(const bool& debug, const bool& editMode) {
    // Recorremos todos los interactores
    for (auto &inter : interactors) {
        
        if (inter.type == InteractorType::SURFACE) {
            if(inter.gui){
                ofSetColor(255, 255, 100); // Amarillo brillante
            }
            else {
                ofSetColor(180, 120, 255); // Lila
            }
        }
        else if (inter.type == InteractorType::WALL) {
            if(inter.gui){
                ofSetColor(255, 255, 100); // Amarillo brillante
            }
            else {
                ofSetColor(255, 0, 0);     // Rojo
            }
        }
        else if (inter.type == InteractorType::BUTTON) {
            if(inter.gui){
                ofSetColor(255, 255, 100); // Amarillo brillante
            }
            else {
                ofSetColor(100, 200, 255); // Celeste
            }
        }
        // DRAW la línea que une los dos puntos
        ofDrawLine(inter.p1.x, inter.p1.y, inter.p2.x, inter.p2.y);
        
        // ************************ DEBUG ************************
        if(debug || editMode){
            // HIGHLIGHTS HITTED
            if (inter.hit) {
                if (inter.type == InteractorType::WALL) {
                    drawWallHighlight(inter);
                }
                else if (inter.type == InteractorType::SURFACE) {
                    drawFloorHighlight(inter);
                }
            }
        }
    }
    
    // ************************ DEBUG ************************
     // DRAW los nombres moviendo el texto según la pared
    if(debug || editMode){
        drawInteractorsLabels();
    }
}

// *** DETECCIÓN DE COLISIONES A FUTURO ***
// Retorna el paquete de colisiones detectadas
CollisionResult CollisionManager::checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, bool isFacingRight) {
    
    // ==========================================
    // FASE 0: RESETEO DE TODOS LOS INTERACTORS
    // ==========================================
    for (auto &inter : interactors) {
        inter.hit = false;
    }
    
    // ==========================================
    // FASE 1: CÁLCULO DE SENSORES, posiciones futuras
    // ==========================================
    
    //Resultados Frame Actual
    CollisionResult result;
    // Sensores actuales y proyectados al futuro
    SensorState sensors = calculateSensors(currentPos, velocity, gravity, isFacingRight);
    
    
    // ==========================================
    // FASE 2: BUCLES DE DETECCIÓN DE INTERACTORES
    // ==========================================
    
    // Recorremos todos los Interactors
    for (auto &inter : interactors) {
        
        // WALL
        if (inter.type == InteractorType::WALL) {
            
            // Si estoy mirando adelante y la pared está más que mi posicion central
            // Porque el sistema no puede dejar pasar mi posición si hay una pared, y si la paso por lo que sea, ya será detras de mi
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // Si un día queremos detectar paredes delante y detrás a la vez?
            // por la espalda, sería con un hitbox de espalda, no?
            // Se pueden detectar dos interactors de un mismo tipo a la vez?
            // Puede con un vector o array de interactos tipo wall?
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            // Calculamos la distancia absoluta desde el centro (floorRayX) hasta la pared
            float distAlMuro = abs(inter.p1.x - sensors.floorRayX);
            
            // El radio depende de la escala
            float radioInfluencia = inter.influenceRadius * currentScale;
            
            // DETERMINAR LÍMITES VERTICALES DE LA PARED (p1 y p2 pueden estar invertidos)
            float wallTop = min(inter.p1.y, inter.p2.y);
            float wallBottom = max(inter.p1.y, inter.p2.y);
            
            // FILTRO VERTICAL: ¿Está el personaje en el rango de la pared?
            // Si la Cabeza está más arriba que el final de la pared
            // Y los Pies están más abajo que la base de la pared
            if (sensors.headY < wallBottom && sensors.feetY > wallTop) {
                
                // FILTRO DE INFUENCIA: ¿Está la pared dentro de mi radio de acción?
                if (distAlMuro <= radioInfluencia) {
                    
                    if(isFacingRight && inter.p1.x > sensors.floorRayX ){
                        // Comprobamos bloqueo derecho
                        if (checkWallRightCollision(inter, sensors)) {
                            inter.hit = true;     // Interactor Hitted
                            result.isWalledRight = true;
                            result.wall = &inter; // Guardamos la referencia por si el Juez la necesita
                        }
                    }
                    else if(!isFacingRight && inter.p1.x < sensors.floorRayX ){
                        // Comprobamos bloqueo izquierdo
                        if (checkWallLeftCollision(inter, sensors)) {
                            inter.hit = true; // Interactor Hitted
                            result.isWalledLeft = true;
                            result.wall = &inter;
                        }
                    }
                }
            }
        }

        // SURFACE
        if (inter.type == InteractorType::SURFACE) {
            // Si hay colisión
            if (checkFloorCollision(inter, sensors)) {
                inter.hit = true; // Interactor Hitted
                result.isGrounded = true;
                result.floor = &inter;
            }
        }
    }
    
    // Guardamos una copia
    lastResult = result;
    // Aunque lo hemos guardado, devolvemos el resultado
    return result;
}
    
// *** MÉTODOS DE EDICIÓN ***
// Añade un nuevo interactor a la colección
void CollisionManager::addInteractor(const Interactor& inter) {
    // Añadimos el interactor que llega al vector de interactores
    interactors.push_back(inter);
    ofLogNotice("CollisionManager") << "Nuevo interactor añadido: " << inter.name;
}
// Elimina interactor de la colección, por nombre
void CollisionManager::removeInteractor(string name) {
    // std::remove_if recorre el vector y "mueve" los elementos que NO queremos borrar al principio
    // Los que SI coinciden con el nombre, los deja al final en un estado "inválido"
    // 'it' es un iterador que marca dónde empiezan esos elementos que queremos descartar
    auto it = std::remove_if(interactors.begin(), interactors.end(), [&](const Interactor& inter) {
        // Esta es la condición (lambda): si el nombre coincide, "márcalo" para quitarlo.
        return inter.name == name;
    });

    // Si 'it' no llegó al final (.end()), significa que encontró al menos un elemento que coincide
    if (it != interactors.end()) {
        // .erase corta físicamente el vector desde la posición 'it' hasta el final.
        // Aquí es donde el vector realmente reduce su tamaño (size).
        interactors.erase(it, interactors.end());
        
        ofLogNotice("CollisionManager") << "Interactor eliminado: " << name;
    } else {
        ofLogWarning("CollisionManager") << "No se pudo eliminar: " << name << " (no encontrado)";
    }
}

// *** ACTUALIZA VALORES DE TRABAJO SEGUN ESCALA ***
void CollisionManager::updateScaledHitbox() {
    currentHitbox.regionW = baseHitbox.regionW * currentScale;
    currentHitbox.regionH = baseHitbox.regionH * currentScale;
    currentHitbox.width   = baseHitbox.width * currentScale;
    currentHitbox.height  = baseHitbox.height * currentScale;
    currentHitbox.floorRayX = baseHitbox.floorRayX * currentScale;
}

// *** CACULA SENSORES FUTUROS ***
//Traduce las coordenadas locales del personaje al mundo real.
SensorState CollisionManager::calculateSensors(ofVec2f pos, ofVec2f vel, float grav, bool isFacingRight) {
    
    SensorState s;
    HitboxData h = currentHitbox;
    
    // Colocar calculos en el centro de la región
    // Representa la esquina izquierda de nuestra caja de 300x300
    float drawX = -h.regionW / 2.0f;

    // Paredes
    if (isFacingRight) {
        // El sensor está en: Mi Centro + Inicio del dibujo + Ancho de mi Hitbox
        s.wallCurrent = pos.x + drawX + h.width;
        // Lo mismo pero con la velocidad aplicada
        s.wallFuture  = (pos.x + vel.x) + drawX + h.width;
    } else {
        s.wallCurrent = pos.x - (drawX + h.width);
        s.wallFuture  = (pos.x + vel.x) - (drawX + h.width);
    }
    
    // Calculamos la parte superior de la hitbox (posicion base menos offset de region + margen)
    s.headY = pos.y - h.regionW / 2.0f;

    // Suelo
    // El rayo X se calcula a partir del centro del personaje más un desplazamiento específico (floorRayX)
    float localRayX;

    if (isFacingRight) {
        // El rayo X está a una distancia 'floorRayX' desde el borde izquierdo (drawX)
        localRayX = drawX + h.floorRayX;
    }
    else {
        // Lógica de Espejo: Invertimos la posición respecto al centro
        localRayX = -(drawX + h.floorRayX);
    }
    
    // Posición Y de los pies, actual y futura:
    // Bajamos desde el centro (drawX es -150) + margen de seguridad (10) + altura hitbox.
    s.floorRayX    = pos.x + localRayX;
    s.floorCurrentY = pos.y - h.regionW / 2.0f + h.height;
    s.floorFutureY  = (pos.y + vel.y + grav) - h.regionW / 2.0f + h.height;
    
    // Guardamos la posición de los pies para usarla en la lógica de colisión de paredes
    s.feetY = s.floorCurrentY;

    return s;
}


// |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
// Para suelos de altura limitada o plataformas elevadas,
// ya filtramos por X (groundMinX/MaxX). Falta decidir si queremos "one-way platforms"
// (atravesables desde abajo).

//Suelos: Como ya tienes el minX y maxX, ya puedes crear un nivel con "flotantes" y el personaje se caerá por los huecos.
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


// *** CHECK SURFACE COLLISION ***
// Verifica si el rayo vertical del personaje toca una superficie.
bool CollisionManager::checkFloorCollision(Interactor& inter, const SensorState& s) {
    
    // Determinamos los límites horizontales del suelo, líneas dibujadas de izq a der o viceversa
    float groundMinX = min(inter.p1.x, inter.p2.x);
    float groundMaxX = max(inter.p1.x, inter.p2.x);

    // 1. COMPROBACIÓN HORIZONTAL: ¿Está el rayo de mis pies sobre la plataforma?
    if (s.floorRayX >= groundMinX && s.floorRayX <= groundMaxX) {
        
        // 2. COMPROBACIÓN VERTICAL (Detección de cruce):
        // ¿Estaba por encima del suelo (o tocándolo) y en el siguiente frame estaría por debajo?
        if (s.floorCurrentY <= inter.p1.y && s.floorFutureY >= inter.p1.y) {
            return true;
        }
    }
    return false;
}

// *** CHECK WALL COLLISION ***
// Verifica si el sensor lateral ha invadido el espacio de una pared.
// Detecta si una pared bloquea el avance hacia la DERECHA
bool CollisionManager::checkWallRightCollision(Interactor& inter, const SensorState& s) {
    
    float wallX = inter.p1.x;

    // SOLO una cosa: ¿He pasado la pared?
    // Si mi X actual YA está al otro lado (más allá) = colisión
    return s.wallCurrent >= wallX;
}
// Detecta si una pared bloquea el avance hacia la IZQUIERDA
bool CollisionManager::checkWallLeftCollision(Interactor& inter, const SensorState& s) {
    
    float wallX = inter.p1.x;
    
    // SOLO una cosa: ¿He pasado la pared?
    // Si mi X actual YA está al otro lado (antes) = colisión
    return s.wallCurrent <= wallX;
}


// *** DRAWS DEBUG ***
void CollisionManager::drawWallHighlight(const Interactor& inter) {
    // 1. Estilo: Azul transparente
    ofFill();
    ofSetColor(0, 150, 255, 50); // Azul con poca opacidad
    
    // 2. Calculamos el rectángulo del radio de influencia (escalado)
    // La pared es vertical, así que usamos p1.x y la altura entre p1.y y p2.y
    float x = inter.p1.x - (inter.influenceRadius * currentScale);
    float y = inter.p1.y;
    float w = (inter.influenceRadius * currentScale) * 2;
    float h = abs(inter.p2.y - inter.p1.y);
    
    ofDrawRectangle(x, y, w, h);
}

void CollisionManager::drawFloorHighlight(const Interactor& inter) {
    // 1. Estilo: Azul/Cian transparente
    ofFill();
    ofSetColor(0, 255, 255, 80);
    
    // 2. Dibujamos el rectángulo de 10px por debajo del suelo
    // El ancho es la diferencia entre p2 y p1
    float x = inter.p1.x;
    float y = inter.p1.y; // El suelo está en esta altura
    float w = abs(inter.p2.x - inter.p1.x);
    float h = 10.0f; // Los 10 píxeles de grosor que pediste
    
    ofDrawRectangle(x, y, w, h);
}

// Dibujo textos interactors
void CollisionManager::drawInteractorsLabels(){
    // Texto en blanco
    ofSetColor(255);
    for (auto &inter : interactors) {
        float textX = inter.p1.x;
        // Bajamos un poco el texto para que no pise el borde superior
        float textY = inter.p1.y + 20;
        
        // Tipo Surface centra el texto
        if (inter.type == InteractorType::SURFACE) {
            textX = (inter.p1.x + inter.p2.x) / 2;
        }
        // Otros TIPOS
        else {
            // Si es el Limite Base Derecho
            if (inter.name == "LIMIT_RIGHT") {
                // Movemos el texto a la IZQUIERDA de la línea
                // Restamos unos 80 píxeles (aprox. lo que ocupa la palabra)
                textX = inter.p1.x - 85;
            }
            // Todas las demás, a la DERACHA
            else {
                // Para el suelo y la pared izquierda, lo movemos un poco a la derecha
                textX = inter.p1.x + 10;
            }
        }
                // DIBUJAR TEXTO
        ofDrawBitmapString(inter.name, textX, textY);
    }

}


// *** JSON INTERACTORS ***
// *** CARGA ***
// !!!! VERSIÓN OF 0.12 !!!!
void CollisionManager::loadInteractorsJSON(const std::string& filename) {
    
    // Intentamos abrir el archivo
    ofFile file(filename);
    
   // Si no existe, salimos
    if (!file.exists()) {
        ofLogError("CollisionManager") << "No se pudo encontrar " << filename;
        return;
    }
    
    // Leemos el contenido del archivo y lo volcamos en la variable 'json'
    ofJson json = ofLoadJson(filename);
    // Limpiamos los actuales
    interactors.clear();

    // Recorremos los interactores del JSON
    for (auto & jInter : json) {
        // Objeto temporal vacío
        Interactor inter;
        
        // Asignamos las valores a las claves
        inter.name = jInter["name"];
        inter.type = (InteractorType)jInter["type"];
        inter.p1.set(jInter["p1"]["x"], jInter["p1"]["y"]);
        inter.p2.set(jInter["p2"]["x"], jInter["p2"]["y"]);
        inter.influenceRadius = jInter["influenceRadius"];
        inter.hit = false;
        inter.gui = false;
        
        // Añadimos este objeto individual dentro de la lista de interactores
        interactors.push_back(inter);
    }
    
    ofLogNotice("CollisionManager") << "Interactors cargados: " << interactors.size();
}

// !!!! VERSIÓN OF 0.10 !!!!
/*void CollisionManager::loadInteractorsJSON(const std::string& filename) {
 ofxJSONElement json;
 
 // Usamos .open() como en MovementManager para mayor consistencia
 if (!json.open(filename)) {
     ofLogError("CollisionManager") << "No se pudo encontrar o abrir " << filename;
     return;
 }
 
 interactors.clear(); // Limpiamos los actuales
 
 // Si el JSON es un array, iteramos con un índice
 for (int i = 0; i < json.size(); i++) {
     const auto& jInter = json[i];
     
     Interactor inter;
     // Usamos asString(), asInt(), etc., para evitar errores de tipo
     inter.name = jInter["name"].asString();
     inter.type = (InteractorType)jInter["type"].asInt();
     
     // Acceso a puntos anidados
     inter.p1.set(jInter["p1"]["x"].asFloat(), jInter["p1"]["y"].asFloat());
     inter.p2.set(jInter["p2"]["x"].asFloat(), jInter["p2"]["y"].asFloat());
     
     // Si influenceRadius puede no existir, podrías usar .get("influenceRadius", 0.0f).asFloat()
     inter.influenceRadius = jInter["influenceRadius"].asFloat();
     
     inter.hit = false;
     inter.gui = false;
     
     interactors.push_back(inter);
 }
 
 ofLogNotice("CollisionManager") << "Interactors cargados: " << interactors.size();
}*/

// *** GUARDADO ***
void CollisionManager::saveInteractorsJSON(const std::string& filename) const {
    // Preparamos una lista (array) de JSON vacía
    ofJson json = ofJson::array();

    // Recorremos los interactores existentes
    for (const auto & inter : interactors) {
        // Objeto JSON temporal (jInter) y asignamos las claves y valores
        ofJson jInter;
        jInter["name"] = inter.name;
        jInter["type"] = (int)inter.type;
        jInter["p1"] = { {"x", inter.p1.x}, {"y", inter.p1.y} };
        jInter["p2"] = { {"x", inter.p2.x}, {"y", inter.p2.y} };
        jInter["influenceRadius"] = inter.influenceRadius;
        
        // Añadimos este objeto individual dentro de la lista principal JSON.
        json.push_back(jInter);
    }

    // ofSaveJson toma toda esa estructura y la escribe físicamente en el archivo
    if (ofSaveJson(filename, json)) {
        ofLogNotice("CollisionManager") << "Archivo guardado con éxito: " << filename;
    }
}

// ------------------- GETTERS - SETTERS -------------------
const vector<Interactor>& CollisionManager::getInteractors() const {
    return interactors;
}
CollisionResult CollisionManager::getLastResult() const {
    return lastResult;
}

HitboxData CollisionManager::getHitbox() const {
    return currentHitbox;
}
HitboxData CollisionManager::getBaseHitbox() const {
    return baseHitbox;
}
void CollisionManager::setHitboxWidth(float w) {
    baseHitbox.width = w;
    // Debemos recalcular la hitbox de trabajo
    updateScaledHitbox();
}
void CollisionManager::setHitRayXFloor(float x) {
    baseHitbox.floorRayX = x;
    // Debemos recalcular la hitbox de trabajo
    updateScaledHitbox();
}

// Actualiza el parametro "gui" de Interactor, segun su nombre.
void CollisionManager::setInteractorGUI(string name, bool state){
    // Recorremos los interactors acutales
    for (auto &inter : interactors) {
        // Al encontrarlo, actualizamos el parametro "gui"
        if(inter.name == name){
            inter.gui = state;
        }
    }
}

// Modifica el valor de escala
void CollisionManager::setCurrentScale(float scale) {
    currentScale = scale;
    // Debemos recalcular la hitbox de trabajo
    updateScaledHitbox();
}
