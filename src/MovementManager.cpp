#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "Utilities.h"


// *** CONFIGURACIÓN Y CONEXIÓN ***
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

// *** CICLO DE VIDA ***
// Actualiza el estado del MovementManager basado en el tiempo actual.
void MovementManager::update(float currentTime) {
    
    // Primero el cerebro decide qué hacer
    updateIntent();

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



// !!!!!!! TRADUCTOR !!!!!!!
void MovementManager::updateIntent() {
    
    // Si estamos esperando o ejecutando una transición comprometida, no tomamos nuevas decisiones
    if (waitingForTransition || currentState == MovementState::TURNING || currentState == MovementState::STOPPING) {
        return;
    }
    
    // 1. Obtenemos los "deseos" del jugador desde el InputManager
    InputState intent = inputManager->getInputState();
    
    // 2. DEFINICIÓN DE LA VARIABLE LOCAL
    MovementState targetState = MovementState::IDLE; // Por defecto

    // 3. TRADUCCIÓN LÓGICA
    if (intent.hasAnyDirection) {
        
        // Variables para comprobar si hay que girar primero
        bool lookingRight = getIsFacingRight();
        bool movingRight = intent.wantsRight;
        
        // Si hay que Girar
        if (lookingRight != movingRight) {
            targetState = MovementState::TURNING;
        }
        // Si no hay que Girar
        else {
            // Quiere CORRER
            if (intent.wantsRun) {
                targetState = MovementState::RUNNING;
            // Quiere Caminar
            } else {
                targetState = MovementState::WALKING;
            }
        }
    // Si no hay dirección, la intención es estar en IDLE
    } else {
        targetState = MovementState::IDLE;
    }

    // 4. LLAMADA AL JUEZ
    // Le pasamos nuestra intención. Él decidirá si se puede cumplir y cómo.
    updateState(targetState);
    
}


// !!!!!!! JUEZ !!!!!!! //
// Switch de estados y llama a playMovement o handleTransition según convenga.
void MovementManager::updateState(MovementState targetState) {
    
    //Bloqueo 1
    if (currentState == targetState) return; // Ya estamos en el estado deseado
    
    // Si estamos esperando o ejecutando una transición comprometida, no tomamos nuevas decisiones
    if (waitingForTransition || currentState == MovementState::TURNING || currentState == MovementState::STOPPING) {
        return;
    }
    
    //Si el targetState es diferente al currentState, el Juez decide cómo proceder.
    // 2. Decidimos según el estado actual
    switch (currentState) {
        
        case MovementState::IDLE:
            
            if (targetState == MovementState::TURNING) {
                            
                std::cout << "!!! ESTA IDLE, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // EJECUTAMOS GIRO (COMPROMETIDO)
                playMovement("TURN");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::TURNING;
            }
            
            else if (targetState == MovementState::WALKING) {
                
                std::cout << "!!! ESTA IDLE, VAMOS A CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // EJECUTAMOS FELXIBLE
                playMovement("WALK");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
            }
            
            else if (targetState == MovementState::RUNNING) {
                            
                std::cout << "!!! ESTA IDLE, VAMOS A CORRER DESDE: "  << currentMovementName << "!!!" << std::endl;

                // EJECUTAMOS FELXIBLE
                playMovement("RUN");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
            }
            
        break;


        case MovementState::WALKING:

            if (targetState == MovementState::IDLE) {
                
                std::cout << "!!! ESTA WALKING, VAMOS A FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition(); // buscará WALK_TO_IDLE correcto según puntos de salida cercanos
                
            }
            
            else if (targetState == MovementState::TURNING) {
                
                std::cout << "!!! ESTA WALKING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition(); // buscará el WALK_TURN correcto según puntos de salida cercanos
                
            }

            else if (targetState == MovementState::RUNNING) {
                
                std::cout << "!!! ESTA WALKING, VAMOS A CORRER DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // EJECUTAMOS CAMBIO
                handleWalkToRun(); // Cambio según mapa de regiones
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
                
            }
        break;

        case MovementState::RUNNING:
            
            // Si deja de pulsar direcciones
            if (targetState == MovementState::IDLE) {
                
                std::cout << "!!! ESTA RUNNING, VAMOS A FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition(); // buscará el RUN_TO_IDLE correcto según puntos de salida cercanos
                
            }

            else if (targetState == MovementState::WALKING) {
                
                std::cout << "!!! ESTA RUNNING, VAMOS A CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // EJECUTAMOS CAMBIO
                handleRunToWalk(); // Cambio según mapa de regiones
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
                
            }
            
            else if (targetState == MovementState::TURNING) {
                
                std::cout << "!!! ESTA RUNNING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
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


// !!!!!!! UPDATE REGION !!!!!!! // Segun el .h iría más abajo porque es private, pero la dejamos aquí junto a la otra única función que ejecuta playMovement y actualiza el estado.
// Maneja la lógica de actualización de frames y transiciones
void MovementManager::updateRegion() {
    // Incrementa la región actual
    currentRegion++;
    
    //*** DISPARAR TRANSICION ***//
    // Si estamos esperando una transición y hemos llegado al Punto de Salida,
    if (waitingForTransition && currentRegion == targetRegion) {
        waitingForTransition = false;
        
        std::cout << "!!! DISPARAR TRANSICION : !!!" << std::endl;
        
        if (currentState == MovementState::TURNING) {
            // EJECUTAMOS
            playMovement(currentMovement->turn_transitions[targetRegion]);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::TURNING;

            
        } else {
            // EJECUTAMOS
            playMovement(currentMovement->transitions[targetRegion]);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::STOPPING;
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
        InputState intent = inputManager->getInputState();

        if (intent.wantsRight || intent.wantsLeft) {
            // Si el jugador sigue pulsando una dirección...
            if (intent.wantsRun) {
                // ... y pulsa correr: Directo a RUN
                // EJECUTAMOS FELXIBLE
                playMovement("RUN");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
            } else {
                // ... y no pulsa correr: Sigue caminando
                // EJECUTAMOS FELXIBLE
                playMovement("WALK");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
            }
        }
        else {
            // Era un giro desde WALK
            if(currentMovementName.find("WALK_TURN") != std::string::npos){
                std::cout << ">>> Giro en movimiento WALK terminado: Paso de transición antes de frenar." << std::endl;
                // EJECUTAMOS FELXIBLE
                playMovement("WALK");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
            }
            // Era un giro desde RUN
            else if (currentMovementName.find("RUN_TURN") != std::string::npos){
                std::cout << ">>> Giro en movimiento RUN terminado: Paso de transición antes de frenar." << std::endl;
                // EJECUTAMOS FELXIBLE
                playMovement("RUN");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
            }
            else {
                // ... si no, vuelve a IDLE
                playMovement("IDLE");
                currentState = MovementState::IDLE;
            }
        }
    updateIntent();
    return;
    }
    
    
    //*** MOVIMIENTO BUCLE ***//
    // Si la región actual alcanza el último frame,
    if (currentRegion >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       currentRegion = 0;
    }
}


// !!!!!!! EJECUTOR !!!!!!! //
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
    }
}


// !!!!!!! OTEADOR !!!!!!!//
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
    
    //*** DEFINE LA REGIÓN DE TRANSICIÓN Y ACTIVA ESPERA ***//
    // Si se encontró una región de transición válida
    if (closestRegion != -1) {
        // Establece la región objetivo para la transición
        targetRegion = closestRegion;
        
        // Indica que se está esperando una transición
        waitingForTransition = true;
    }

}




    


// *** HANDLE SPECIFIC MOVEMENTS *** //
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
    



//*** GETS REGION ***//
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

//*** GETS FRAME INTERVAL ***//
// Obtiene el intervalo de fotogramas global.
float MovementManager::getFrameInterval() const {
    // Devuelve el intervalo de fotogramas global
    return frameInterval;
}
// Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global.
float MovementManager::getCurrentMovementFrameInterval() const {
    return (currentMovement && currentMovement->frameInterval > 0.0f)
    ? currentMovement->frameInterval
    : frameInterval;
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

//*** GETS DIRECTION ***//
//Obtiene la dirección del personaje
bool MovementManager::getIsFacingRight() {
    return isFacingRight;
}

//*** GETS MOVEMENT ***//
// Obtiene el movimiento actual
const Movement* MovementManager::getCurrentMovement() const {
    //currentMovement es un std::unique_ptr<Movement>
    //.get() extrae el puntero crudo (raw pointer) del unique_ptr
    //Así devolvemos Movement* en lugar de unique_ptr<Movement>
    return currentMovement.get();
}
// Obtiene el nombre del movimiento actual
std::string MovementManager::getCurrentMovementName() const {
    return currentMovementName;
}
// Obtiene el mapa de todos los movimientos disponibles
const std::map<std::string, Movement>& MovementManager:: getMovements() const {
    // movements es un mapa grande (std::map<std::string, Movement>)
    // Copiarlo cada vez sería ineficiente
    // Una referencia const lo deja accesible sin copiarlo
    return movements;
}

//*** GETS STATE ***//
// Obtiene un string con el estado actual del personaje
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
// Retorna true si está esperando una transición, false en caso contrario
bool MovementManager::isWaitingForTransition() const {
    return waitingForTransition;
}

//*** SETS FRAME INTERVAL ***//
// Actualiza los intervalos de frame desde la GUI.
void MovementManager::setFrameIntervalFromGUI() {
    // Lógica para actualizar los intervalos de frame desde la GUI (a completar)
}
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

//*** SETS DIRECTION ***//
// Establece la dirección del personaje
void MovementManager::toggleIsFacingRight() {
    isFacingRight = !isFacingRight;
}


// *** LÓGICA INTERNA (LOS ENGRANAJES) ***
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


