#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "Utilities.h"


// Inicializa el MovementManager cargando los movimientos desde un archivo JSON y configurando el movimiento actual a "IDLE".
void MovementManager::setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input) {
    
    // Guardamos la referencia del inputManager para usarla luego
    this->inputManager = &input;
    
    // Carga los movimientos desde el archivo JSON proporcionado
    loadMovements(filename);
    // Establece el movimiento actual en "IDLE" utilizando el movimiento cargado desde el JSON
    currentMovement = std::make_unique<Movement>(movements["IDLE"]);
    currentMovementName = "IDLE";
}

// Actualiza el estado del MovementManager basado en el tiempo actual.
void MovementManager::update(float currentTime) {
    
    // Primero el cerebro decide qué hacer
    updateState();

    // Luego el cuerpo (tus frames) se actualiza
    // Obtiene el intervalo de frame del movimiento actual
    float currentMovementFrameInterval = getCurrentMovementFrameInterval();
    
    // Determina si se debe actualizar la región en función del tiempo y el intervalo
    if (shouldUpdateRegion(currentTime, currentMovementFrameInterval)) {
        // Actualiza la lógica de la región del movimiento
        updateRegion();
        lastUpdateTime = currentTime;
    }
}



//
void MovementManager::updateState() {
    // Si estamos esperando o ejecutando una transición comprometida, no tomamos nuevas decisiones
    if (waitingForTransition || currentState == MovementState::TURNING || currentState == MovementState::STOPPING) {
        return;
    }
    
    // 1. Obtenemos los "deseos" del jugador desde el InputManager
    InputState intent = inputManager->getState();

    // 2. Decidimos según el estado actual
    switch (currentState) {
        
        case MovementState::IDLE:
            // Si estoy quieto y está pulsada una dirección...
            if (intent.wantsRight || intent.wantsLeft) {
                
                // Comprobamos si hay que girar primero
                bool lookingRight = getIsFacingRight();
                bool movingRight = intent.wantsRight;

                if (lookingRight != movingRight) {
                    
                    std::cout << "!!! ESTA IDLE, DEBE GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                    // Si la dirección que quiero no es donde miro: GIRO
                    playMovement("TURN");
                } else {
                    std::cout << "!!! ESTA IDLE, DEBE CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                    // Si miro hacia donde quiero ir: CAMINO
                    // Si además pulsa '1', podríamos ir directo a RUN (lo añadiremos luego)
                    playMovement("WALK");
                }
            }
            break;

        case MovementState::WALKING:
            // Si dejo de pulsar direcciones: FRENO
            if (!intent.wantsRight && !intent.wantsLeft) {
                std::cout << "!!! ESTA WALKING, DEBE FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleTransition(); // buscará WALK_TO_IDLE correcto según puntos de salida cercanos
            }
            // Si intento girar mientras camino...
            else if ((intent.wantsRight && !getIsFacingRight()) ||
                     (intent.wantsLeft && getIsFacingRight())) {
                std::cout << "!!! ESTA WALKING, DEBE GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleTransition(); // buscará el WALK_TURN correcto según puntos de salida cercanos
            }
            // Si pulso el botón de correr (tecla '1')
            else if (intent.wantsRun) {
                std::cout << "!!! ESTA WALKING, DEBE CORRER DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleWalkToRun(); // Cambio según mapa de regiones
            }
            break;

        case MovementState::RUNNING:
            // Si deja de pulsar direcciones
            if (!intent.wantsRight && !intent.wantsLeft) {
                std::cout << "!!! ESTA RUNNING, DEBE FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleTransition(); // buscará el RUN_TO_IDLE correcto según puntos de salida cercanos
            }
            // Si deja de pulsar '1' (deja de querer correr)
            else if (!intent.wantsRun) {
                std::cout << "!!! ESTA RUNNING, DEBE CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleRunToWalk(); // Cambio según mapa de regiones
            }
            // Si intenta girar mientras corre
            else if ((intent.wantsRight && !getIsFacingRight()) ||
                     (intent.wantsLeft && getIsFacingRight())) {
                std::cout << "!!! ESTA RUNNING, DEBE GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                handleTransition(); // buscará el RUN_TO_TURN correcto según puntos de salida cercanos
            }
            break;

            
        // EN PRINCIPIO ESTOS DOS CASE NO HARÍA FALTA POR EL PRIMER IF CON RETURN DE ESTA FUNCIÓN
        case MovementState::TURNING:
            std::cout << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!" << std::endl;

            // En este estado NO hacemos nada.
            // Esperamos a que la animación termine.
            // Cuando termine, 'updateRegion' detectará que es una transición,
            // llamará a toggleIsFacingRight y nos devolverá a IDLE o WALK.
            break;

        case MovementState::STOPPING:
            std::cout << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!" << std::endl;
            // Al igual que TURNING, esperamos a que termine la frenada.
            break;
    }
}






//*** INICIA MOVIMIENTO ***//
void MovementManager::playMovement(const std::string& movementName, int region) {

    // ^^^ PARA MONITORIZAR EN LA GUI ^^^ //
    // CUANDO NO HAGA FALTA MONITORIZAR LO METEMOS EN EL IF
    // Actualiza el nombre del movimiento actual
    currentMovementName = movementName;

    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movements.count(movementName)) {
        
        // Actualiza el movimiento actual utilizando el movimiento especificado
        currentMovement = std::make_unique<Movement>(movements[movementName]);
        
        // ^^^ CUANDO NO HAGA FALTA MONITORIZAR LO METEMOS EN EL IF ^^^
        // Actualiza el nombre del movimiento actual
        //currentMovementName = movementName;
        
        
        // Establece la fila inicial del movimiento
        currentRow = currentMovement->row;
        
        // Establece la región inicial del movimiento
        currentRegion = region;
        
        
        // --- ACTUALIZACIÓN DEL ESTADO LÓGICO ---
        if (movementName == "IDLE") currentState = MovementState::IDLE;
        else if (movementName == "WALK") currentState = MovementState::WALKING;
        else if (movementName == "RUN") currentState = MovementState::RUNNING;
        // Se usa find porque no hay una sola animación de giro o de parada. (std::string::npos --> valor constante especial que significa "no encontrado")
        else if (movementName.find("TURN") != std::string::npos) currentState = MovementState::TURNING;
        else if (movementName.find("TO_IDLE") != std::string::npos) currentState = MovementState::STOPPING;
        
        
        // Mensaje de depuración para mostrar el estado actual
        switch (static_cast<int>(currentState)) {
            case 0:
                std::cout << "*** IDLE ***" << std::endl;
                break;
            case 1:
                std::cout << "*** WALKING ***" << std::endl;
                break;
            case 2:
                std::cout << "*** RUNNING ***" << std::endl;
                break;
            case 3:
                std::cout << "*** TURNING ***" << std::endl;
                break;
            case 4:
                std::cout << "*** STOPPING ***" << std::endl;
                break;
            default:
                std::cout << "*** UNKNOWN (" << static_cast<int>(currentState) << ") ***" << std::endl;
                break;
        }
        
    }
}

//*** MANEJO DE MOVIMIENTOS CONCRETOS ***//
// Implementación de handleWalkToRun
void MovementManager::handleWalkToRun() {
    // Si el movimiento actual es WALK
    if (currentMovementName == "WALK") {
        //std::cout << "WALK TO RUN" << std::endl;
        // Obtener la región actual de WALK
        int currentRegion = getCurrentRegion();
        // Obtener la región de RUN correspondiente
        int runRegion = walkToRunRegionMap[currentRegion];
        // Activar el movimiento RUN con la región correspondiente
        playMovement("RUN", runRegion);
    }
}
// Implementación de handleRunToWalk
void MovementManager::handleRunToWalk() {
    // Si el movimiento actual es RUN
    if (currentMovementName == "RUN") {
        //std::cout << "RUN TO WALK" << std::endl;
        // Obtener la región actual de RUN
        int currentRegion = getCurrentRegion();
        // Obtener la región de WALK correspondiente
        int walkRegion = runToWalkRegionMap[currentRegion];
        // Activar el movimiento WALK con la región correspondiente
        playMovement("WALK", walkRegion);
    }
}






//*** DETERMINAR REGIÓN DE TRANSICIÓN ***//
void MovementManager::handleTransition() {
    if (waitingForTransition) return;
    
    // Obtiene una referencia al movimiento actual
    Movement& movement = *currentMovement;
    // Inicializa la región de transición más cercana como -1 (no encontrada)
    int closestRegion = -1;
    
    // Variable para guardar el Nombre de la transición a ejecutar
    std::string transitionName;
    
    
    //*** ENCUENTRA LA REGIÓN DE TRANSICIÓN MÁS CERCANA ***//
    for (int region : movement.transitionRegions) {
        // Verifica si la región es mayor que la región actual
        // Y si es la más cercana encontrada hasta ahora
        if (region > currentRegion && (closestRegion == -1 || region < closestRegion)) {
            // Actualiza la región de transición más cercana
            closestRegion = region;
        }
    }
    
    // Si no se encontró una región de transición cercana (pasamos la útima y debe encontrar la siguiente al iniciar el bucle) usa la primera disponible.
    if (closestRegion == -1 && !movement.transitionRegions.empty()) {
        // Usa la primera región de transición disponible
        closestRegion = movement.transitionRegions[0];
    }
    
    //*** DEFINE LA REGIÓN DE TRANSICIÓN ENCONTRADA ***//
    // Si se encontró una región de transición válida
    if (closestRegion != -1) {
        // Establece la región objetivo para la transición
        targetRegion = closestRegion;
        
        // Indica que se está esperando una transición
        waitingForTransition = true;
        
        // --- ACTUALIZACIÓN LÓGICA ANTICIPADA ---
        InputState intent = inputManager->getState();

        // Si el jugador quiere moverse a una dirección distinta a la que mira: es un GIRO
        if ((intent.wantsRight && !getIsFacingRight()) || (intent.wantsLeft && getIsFacingRight())) {
            currentState = MovementState::TURNING;
            std::cout << ">>> Estado cambiado a TURNING (esperando frame de salida)" << std::endl;
        }
        // Si no quiere moverse: es una PARADA
        else if (!intent.wantsRight && !intent.wantsLeft) {
            currentState = MovementState::STOPPING;
            std::cout << ">>> Estado cambiado a STOPPING (esperando frame de salida)" << std::endl;
        }
    }
    
    
    // !!! HAY QUE VER SI ESTO ES NECESARIO !!!
    // No debería llamar a playMovement aquí, a menos que el movimiento actual sea tan corto que no tenga puntos de salida.
    // Si no hay transiciones disponibles, reproduce el movimiento "IDLE"
    else {
        /*
        std::cout << "!!! CASO IDLE 2 MOV: "  << currentMovementName << "!!!" << std::endl;
        
        if(currentMovementName == "WALK_TURN_1"){
            playMovement("WALK", 3);
            //handleControlKeys();
        }
        else if(currentMovementName == "WALK_TURN_2"){
            playMovement("WALK");
           // handleControlKeys();
        }
        else if(currentMovementName == "RUN_TURN_1"){
            playMovement("RUN", 10);
            //handleControlKeys();
        }
        else if(currentMovementName == "RUN_TURN_2"){
            playMovement("RUN", 4);
           // handleControlKeys();
        }
        else {
            playMovement("IDLE");
        }
        
        */
    }
}



// Maneja la lógica de actualización de frames y transiciones
void MovementManager::updateRegion() {
    // Incrementa la región actual
    currentRegion++;
    
    //OBTENEMOS EL INPUT ACTUAL (Necesario para decidir en las transiciones)
    InputState intent = inputManager->getState(); //
    
    // 1. COMPROBAR SALIDA (Antes de incrementar)
    // Si esperamos una transicion y estamos en el frame exacto de salida ANTES de avanzar.
    // Esto permite que las frenadas que salen en el frame 0 funcionen.
    if (waitingForTransition && currentRegion == targetRegion) {
        waitingForTransition = false;
        
        std::cout << "!!! DISPARAR TRANSICION : !!!" << std::endl;
        
        if (intent.wantsRight || intent.wantsLeft) {
            playMovement(currentMovement->turn_transitions[targetRegion]);
        } else {
            playMovement(currentMovement->transitions[targetRegion]);
        }
        return;
    }
    


    //*** TRANSICIÓN COMPLETADA ***//
    // Si el movimiento actual es una transición y se ha completado,
    if (currentMovement->isTransition && currentRegion >= currentMovement->numRegions) {
                
        std::cout << "!!! TRANSICION COMPLETADA : !!!" << std::endl;
        
        

        //Era un giro, cambiamos la dirección lógica
        if (currentMovementName.find("TURN") != std::string::npos) {
            std::cout << "!!! ERA UN GIRO : !!!" << std::endl;
            toggleIsFacingRight(); // Cambia la dirección lógica
        }
        

        //2. ¿QUÉ HACEMOS AHORA? Consultamos el mando para no parar si no es necesario
        InputState intent = inputManager->getState();

        if (intent.wantsRight || intent.wantsLeft) {
            // Si el jugador sigue pulsando una dirección...
            if (intent.wantsRun) {
                // ... y pulsa correr: Directo a RUN
                playMovement("RUN");
            } else {
                // ... y no pulsa correr: Sigue caminando
                playMovement("WALK");
            }
        }
        else {
            // Era un giro desde WALK
            if(currentMovementName.find("WALK_TURN") != std::string::npos){
                std::cout << ">>> Giro en movimiento WALK terminado: Paso de transición antes de frenar." << std::endl;
                playMovement("WALK");
            }
            // Era un giro desde RUN
            else if (currentMovementName.find("RUN_TURN") != std::string::npos){
                std::cout << ">>> Giro en movimiento RUN terminado: Paso de transición antes de frenar." << std::endl;
                playMovement("RUN");
            }
            else {
                // ... si no, vuelve a IDLE
                playMovement("IDLE");
            }
        }
    
    return;
    }
    
    
    //*** MOVIMIENTO BUCLE ***//
    // Si la región actual alcanza el último frame,
    if (currentRegion >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       currentRegion = 0;
    }
}

    
    

// Actualiza los intervalos de frame desde la GUI.
void MovementManager::updateFrameIntervalFromGUI() {
    // Lógica para actualizar los intervalos de frame desde la GUI (a completar)
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

// Obtiene la siguiente región a la que debe llegar (Siguiente Punto de Salida).
int MovementManager::getNextOutRegion() const {
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

//Obtiene la dirección del personaje
bool MovementManager::getIsFacingRight() {
    return isFacingRight;
}

//*** GETS GUI ***
bool MovementManager::isWaitingForTransition() const {
    return waitingForTransition;
}

std::string MovementManager::getCurrentState() const {
    switch (currentState) {
        case MovementState::IDLE:
            return "IDLE";
        case MovementState::WALKING:
            return "WALKING";
        case MovementState::RUNNING:
            return "RUNNING";
        case MovementState:: TURNING:
            return "TURNING";
        case MovementState:: STOPPING:
            return "STOPPING";
        default:
            return "UNKNOWN";
    }
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

// Establece la dirección del personaje
void MovementManager::toggleIsFacingRight() {
    isFacingRight = !isFacingRight;
}





//*** ENGRANAJES (Aquí irá updateRegion() ***

// Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
bool MovementManager::shouldUpdateRegion(float currentTime, float interval) const {
    return currentMovement && (currentTime - lastUpdateTime >= interval);
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
        
        // Convertir el array JSON a un set de strings
        for (const auto& key : jsonMovement["keys"]) {
            movement.keys.insert(key.asString());
        }

        // Carga las regiones de transición del movimiento
        for (const auto& region : jsonMovement["transitionRegions"]) {
            movement.transitionRegions.push_back(region.asInt());
        }

        // Carga las transiciones del movimiento
        for (const auto& transition : jsonMovement["transitions"].getMemberNames()) {
            movement.transitions[std::stoi(transition)] = jsonMovement["transitions"][transition].asString();
        }
        
        // Carga las transiciones del movimiento
        for (const auto& transition : jsonMovement["transitions"].getMemberNames()) {
            movement.turn_transitions[std::stoi(transition)] = jsonMovement["turn_transitions"][transition].asString();
        }

        // Añade el movimiento al mapa de movimientos
        movements[movement.name] = movement;
    }
}


