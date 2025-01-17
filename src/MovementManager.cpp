#include "MovementManager.h"
#include "Utilities.h"


// Inicializa el MovementManager cargando los movimientos desde un archivo JSON y configurando el movimiento actual a "IDLE".
void MovementManager::setup(const std::string& filename, SpriteSheetManager& spriteSheetManager) {
    // Carga los movimientos desde el archivo JSON proporcionado
    loadMovements(filename);
    // Establece el movimiento actual en "IDLE" utilizando el movimiento cargado desde el JSON
    currentMovement = std::make_unique<Movement>(movements["IDLE"]);
}


// Actualiza el estado del MovementManager basado en el tiempo actual.
void MovementManager::update(float currentTime) {
    
    // Obtiene el intervalo de frame del movimiento actual
    float currentMovementFrameInterval = getCurrentMovementFrameInterval();
    
    // Determina si se debe actualizar la región en función del tiempo y el intervalo
    if (shouldUpdateRegion(currentTime, currentMovementFrameInterval)) {
        
        // Actualiza la lógica de la región del movimiento
        updateRegion();
        lastUpdateTime = currentTime;
    }
}


// Maneja la lógica de actualización de frames y transiciones
void MovementManager::updateRegion() {

    // Incrementa la región actual
    currentRegion++;

    // Si se está esperando una transición,
    if (waitingForTransition) {
       // Si la región actual alcanza el objetivo de transición,
       if (currentRegion == targetRegion) {
           //Ya no espera transición
           waitingForTransition = false;
           //Realiza la transición
           playMovement(currentMovement->transitions[targetRegion]);
           // Salimos de la función
           return;
       }
    }

    // Si el movimiento actual es una transición y se ha completado,
    if (currentMovement->isTransition && currentRegion >= currentMovement->numRegions) {
        // Vuelve al movimiento "IDLE"
        playMovement("IDLE");
        // Salimos de la función
        return;
    }

    // Si la región actual alcanza el último frame,
    if (currentRegion >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       currentRegion = 0;
    }
}


// Inicia el movimiento especificado por su nombre
void MovementManager::playMovement(const std::string& movementName, int region) {
    
    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movements.count(movementName)) {
        
        // Actualiza el movimiento actual utilizando el movimiento especificado
        currentMovement = std::make_unique<Movement>(movements[movementName]);
        
        // Actualiza el nombre del movimiento actual
        currentMovementName = movementName;
        
        // Establece la fila inicial del movimiento
        currentRow = currentMovement->row;
        
        // Establece la región inicial del movimiento
        currentRegion = region;
    }
}


// Maneja las transiciones entre movimientos.
void MovementManager::handleTransition() {
    
    // Obtiene una referencia al movimiento actual
    Movement& movement = *currentMovement;
    // Inicializa la región de transición más cercana como -1 (no encontrada)
    int closestRegion = -1;
    
    // Variable para guardar el Nombre de la transición a ejecutar
    std::string transitionName;
    
    // Encuentra la región de transición más cercana
    for (int region : movement.transitionRegions) {
        
        // Verifica si la región es mayor que la región actual
        // Y si es la más cercana encontrada hasta ahora
        if (region > currentRegion && (closestRegion == -1 || region < closestRegion)) {
            
            // Actualiza la región de transición más cercana
            closestRegion = region;
            // Obtiene el nombre de la transición para esa región
            transitionName = movement.transitions[region];
        }
    }
    
    // Si no se encontró una región de transición cercana, usa la primera disponible
    if (closestRegion == -1 && !movement.transitionRegions.empty()) {
        
        // Usa la primera región de transición disponible
        closestRegion = movement.transitionRegions[0];
        
        // Obtiene el nombre de la transición para esa región
        transitionName = movement.transitions[closestRegion];
    }
    
    // Si se encontró una región de transición válida, establece el objetivo de transición
    if (closestRegion != -1) {
        
        // Establece la región objetivo para la transición
        targetRegion = closestRegion;
        
        // Indica que se está esperando una transición
        waitingForTransition = true;
    } else {
        // Si no hay transiciones, reproduce al movimiento "IDLE"
        playMovement("IDLE");
    }
}


// Actualiza los intervalos de frame desde la GUI.
void MovementManager::updateFrameIntervalFromGUI() {
    // Lógica para actualizar los intervalos de frame desde la GUI (a completar)
}


// Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
bool MovementManager::shouldUpdateRegion(float currentTime, float interval) const {
    return currentMovement && (currentTime - lastUpdateTime >= interval);
}



//GETS
// Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
int MovementManager::getCurrentRow() const {
    return currentRow;
}

// Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
int MovementManager::getCurrentRegion() const {
    return currentRegion;
}

// Obtiene la siguiente región a la que debe llegar.
int MovementManager::getNextRegionToGo() const {
    return targetRegion;
}

// Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global.
float MovementManager::getCurrentMovementFrameInterval() const {
    return (currentMovement && currentMovement->frameInterval > 0.0f)
    ? currentMovement->frameInterval
    : frameInterval;
}

// Obtiene el intervalo de fotogramas global.
float MovementManager::getFrameInterval() const {
    // Devuelve el intervalo de fotogramas global
    return frameInterval;
}

// Obtiene el intervalo de fotogramas para un movimiento específico
float MovementManager::getMovementFrameInterval(const std::string& movementName) const {
    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movements.count(movementName)) {
        // Devuelve el intervalo de fotogramas del movimiento específico
        return movements.at(movementName).frameInterval;
    }
    // Valor por defecto si el movimiento no se encuentra
    return 0.1f;
}



//SETS
// Establece el intervalo de fotogramas global
void MovementManager::setFrameInterval(float interval) {
    frameInterval = interval;
}

// Establece el intervalo de fotogramas para un movimiento específico
void MovementManager::setMovementFrameInterval(const std::string& movementName, float interval) {
    if (movements.count(movementName)) {
        movements[movementName].frameInterval = interval;
    }
}



// Carga los movimientos desde un archivo JSON.
void MovementManager::loadMovements(const std::string& filename) {
    ofxJSONElement json;

    // Intenta abrir el archivo JSON
    if (!json.open(filename)) {
        ofLogError("MovementManager::loadMovements") << "No se pudo cargar el archivo JSON: " << filename;
        return;
    }

    // Itera sobre cada movimiento en el JSON
    for (const auto& movementKey : json["movements"].getMemberNames()) {
        const auto& jsonMovement = json["movements"][movementKey];

        // Crea un nuevo objeto Movement y lo inicializa con los datos del JSON
        Movement movement;
        movement.name = jsonMovement["name"].asString();
        movement.row = jsonMovement["row"].asInt();
        movement.numRegions = jsonMovement["numRegions"].asInt();
        movement.isTransition = jsonMovement["isTransition"].asBool();
        movement.frameInterval = jsonMovement["frameInterval"].asFloat();
        movement.key = mapKeyStringToCode(jsonMovement["key"].asString());

        // Carga las regiones de transición del movimiento
        for (const auto& region : jsonMovement["transitionRegions"]) {
            movement.transitionRegions.push_back(region.asInt());
        }

        // Carga las transiciones del movimiento
        for (const auto& transition : jsonMovement["transitions"].getMemberNames()) {
            movement.transitions[std::stoi(transition)] = jsonMovement["transitions"][transition].asString();
        }

        // Añade el movimiento al mapa de movimientos
        movements[movement.name] = movement;
    }
}


