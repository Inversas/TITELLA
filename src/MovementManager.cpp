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
    
    //*** ESPERANDO TRANSICIÓN ***//
    // Si se está esperando una transición
    if (waitingForTransition) {
        
       //*** DISPARAR TRANSICIÓN ***//
       // Si la región actual alcanza el objetivo de transición,
       if (currentRegion == targetRegion) {
           //Ya no espera transición
           waitingForTransition = false;
           
           std::cout << "!!! DISPARAR TRANSICION : !!!" << std::endl;
           std::cout << getControlKeys() << std::endl;

           //EVALUAR CONTROLKEYS
           //Si se mantiene alguna de estas combinaciones la transición será de TURN
           if(   getControlKeys() == "-->"
              || getControlKeys() == "<--"
              || getControlKeys() == "--> + <--"
              || getControlKeys() == "<-- + -->"
              || getControlKeys() == "1 + -->"
              || getControlKeys() == "1 + <--"
              || getControlKeys() == "1 + --> + <--"
              || getControlKeys() == "1 + <-- + -->"
              || getControlKeys() == "<-- + --> OFF"
              || getControlKeys() == "--> + <-- OFF"
              || getControlKeys() == "1 + --> + <-- OFF"
              || getControlKeys() == "1 + <-- + --> OFF"
              ){
               playMovement(currentMovement->turn_transitions[targetRegion]);
           }
           //Si no la transición será de STOP
           else {
               //Realiza la transición
               playMovement(currentMovement->transitions[targetRegion]);
           }
           // Salimos de la función
           return;
       }
    }

    //*** TRANSICIÓN COMPLETADA ***//
    // Si el movimiento actual es una transición y se ha completado,
    if (currentMovement->isTransition && currentRegion >= currentMovement->numRegions) {
                
        std::cout << "!!! TRANSICION COMPLETADA : !!!" << std::endl;
        std::cout << getControlKeys() << std::endl;


        //Si es TURN, se invierte la dirección y evaluamos ControlKeys de nuevo
        if(   currentMovementName == "TURN"
           || currentMovementName == "WALK_TURN_1"
           || currentMovementName == "WALK_TURN_2"
           || currentMovementName == "RUN_TURN_1"
           || currentMovementName == "RUN_TURN_2" ){
            
            // Invierte la dirección
            toggleIsFacingRight();
            
            
            //EVALUAR CONTROLKEYS
            //Si se mantiene alguna de estas combinaciones debemos seguir WALK o RUN
            if(   getControlKeys() == "-->"
               || getControlKeys() == "<--"
               || getControlKeys() == "--> + <--"
               || getControlKeys() == "<-- + -->"
               || getControlKeys() == "1 + -->"
               || getControlKeys() == "1 + <--"
               || getControlKeys() == "1 + --> + <--"
               || getControlKeys() == "1 + <-- + -->"
               || getControlKeys() == "<-- + --> OFF"
               || getControlKeys() == "--> + <-- OFF"
               || getControlKeys() == "1 + --> + <-- OFF"
               || getControlKeys() == "1 + <-- + --> OFF"
               ){
                
                if(currentMovementName == "WALK_TURN_1"){
                    playMovement("WALK", 3);
                }
                if(currentMovementName == "WALK_TURN_2"){
                    playMovement("WALK");
                }
                if(currentMovementName == "RUN_TURN_1"){
                    playMovement("RUN", 10);
                }
                if(currentMovementName == "RUN_TURN_2"){
                    playMovement("RUN", 4);
                }
                //Evalua las teclas de control
                handleControlKeys();
            }
          

        }
        //Si no es TURN, va a IDLE pero después evalua las teclas de control
        else {
            std::cout << "!!! CASO IDLE 1 MOV: "  << currentMovementName << "!!!" << std::endl;

            // Vuelve al movimiento "IDLE"
            playMovement("IDLE");
            
            //Limpia controlKeys
            cleanControlKeys();
        }
        handleControlKeys();

        
        // Salimos de la función
        return;
    }
    
    //*** MOVIMIENTO BUCLE ***//
    // Si la región actual alcanza el último frame,
    if (currentRegion >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       currentRegion = 0;
    }
}





//*** DETERMINAR REGIÓN DE TRANSICIÓN ***//
void MovementManager::handleTransition() {
    
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
    // Si no se encontró una región de transición cercana, usa la primera disponible
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
    }
    // Si no hay transiciones disponibles, reproduce el movimiento "IDLE"
    else {
        std::cout << "!!! CASO IDLE 2 MOV: "  << currentMovementName << "!!!" << std::endl;
        
        if(currentMovementName == "WALK_TURN_1"){
            playMovement("WALK", 3);
            handleControlKeys();
        }
        else if(currentMovementName == "WALK_TURN_2"){
            playMovement("WALK");
            handleControlKeys();
        }
        else if(currentMovementName == "RUN_TURN_1"){
            playMovement("RUN", 10);
            handleControlKeys();
        }
        else if(currentMovementName == "RUN_TURN_2"){
            playMovement("RUN", 4);
            handleControlKeys();
        }
        else {
            playMovement("IDLE");
        }
    }
}

    
//*** MANEJO DE TECLAS DE CONTROL ***//
void MovementManager::handleControlKeys(){

    //*** RESPETA TRANSICIONES ***//
    if(!waitingForTransition){
        
        // Lógica para manejar la tecla -->
        if (getControlKeys() == "-->") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                playMovement("TURN");
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK");
            }
        
        // Lógica para manejar la tecla <--
        } else if (getControlKeys() == "<--") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                playMovement("TURN");
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK");
            }
        
        // Lógica para manejar la tecla 1
        } else if (getControlKeys() == "1") {
            //std::cout << "1" << std::endl;
            
        // Lógica para manejar la combinación 1 + -->
        } else if (getControlKeys() == "1 + -->") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                playMovement("TURN");
            }
            else if(currentMovementName != "RUN"){
                playMovement("RUN");
            }
        
        // Lógica para manejar la combinación 1 + <--
        } else if (getControlKeys() == "1 + <--") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                playMovement("TURN");
            }
            else if(currentMovementName != "RUN"){
                playMovement("RUN");
            }
        
        // Lógica para manejar la combinación <-- + 1
        } else if (getControlKeys() == "<-- + 1") {
            handleWalkToRun();
            
        // Lógica para manejar la combinación --> + 1
        } else if (getControlKeys() == "--> + 1") {
            handleWalkToRun();
        
        // Lógica para manejar la tecla --> OFF
        } else if (getControlKeys() == "--> OFF") {
            handleTransition();
            
        // Lógica para manejar la tecla <-- OFF
        } else if (getControlKeys() == "<-- OFF") {
            handleTransition();
        
        // Lógica para manejar la tecla 1 OFF
        } else if (getControlKeys() == "1 OFF") {
            //std::cout << "1 OFF" << std::endl;
            handleTransition();

        // Lógica para manejar la combinación 1 + --> OFF
        } else if (getControlKeys() == "1 + --> OFF") {
            handleTransition();
        
        // Lógica para manejar la combinación 1 + <-- OFF
        } else if (getControlKeys() == "1 + <-- OFF") {
            handleTransition();
        
        // Lógica para manejar la combinación <-- + 1 OFF
        } else if (getControlKeys() == "<-- + 1 OFF") {
            handleRunToWalk();
            
        // Lógica para manejar la combinación --> + 1 OFF
        } else if (getControlKeys() == "--> + 1 OFF") {
            handleRunToWalk();
            
        // Lógica para manejar la combinación --> + <--
        } else if (getControlKeys() == "--> + <--") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK",currentRegion);
            }
            
        // Lógica para manejar la combinación <-- + -->
        } else if (getControlKeys() == "<-- + -->") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK",currentRegion);
            }
            
        // Lógica para manejar la combinación <-- + --> OFF
        } else if (getControlKeys() == "<-- + --> OFF") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK",currentRegion);
            }
            
        // Lógica para manejar la combinación --> + <-- OFF
        } else if (getControlKeys() == "--> + <-- OFF") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "WALK") {
                playMovement("WALK",currentRegion);
            }
            
        // Lógica para manejar la combinación 1 + --> + <--
        } else if (getControlKeys() == "1 + --> + <--") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "RUN") {
                playMovement("RUN",currentRegion);
            }
            
        // Lógica para manejar la combinación 1 + <-- + -->
        } else if (getControlKeys() == "1 + <-- + -->") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "RUN") {
                playMovement("RUN",currentRegion);
            }
           
        // Lógica para manejar la combinación 1 + --> + <--
        } else if (getControlKeys() == "1 + --> + <-- OFF") {
            //Si el personaje no está mirando hacia la derecha
            if(!getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "RUN") {
                playMovement("RUN",currentRegion);
            }
            
        // Lógica para manejar la combinación 1 + <-- + -->
        } else if (getControlKeys() == "1 + <-- + --> OFF") {
            //Si el personaje está mirando hacia la derecha
            if(getIsFacingRight()){
                handleTransition();
            }
            else if(currentMovementName != "RUN") {
                playMovement("RUN",currentRegion);
            }
            
        // Lógica para manejar "NONE"
        } else  if (getControlKeys() == "NONE") {
            
            std::cout << "!!! CASO IDLE 3 MOV: "  << currentMovementName << "!!!" << std::endl;
                playMovement("IDLE", currentRegion);
          
            
        // ESC
        } else  if (getControlKeys() == "Salir") { {
            std::cout << "Salir" << std::endl;
        }
        // Movimiento no reconocido
        } else {
            std::cout << "Movimiento no reconocido" << std::endl;
        }
        
        
        //Limpia controlKeys
        cleanControlKeys();
    }
}

void MovementManager::cleanControlKeys(){
    if (getControlKeys() == "--> OFF") {
        setControlKeys("NONE");
    } else if (getControlKeys() == "<-- OFF") {
        setControlKeys("NONE");
    } else if (getControlKeys() == "1 OFF") {
        setControlKeys("NONE");
    } else if (getControlKeys() == "1 + --> OFF") {
        setControlKeys("1");
    } else if (getControlKeys() == "1 + <-- OFF") {
        setControlKeys("1");
    } else if (getControlKeys() == "<-- + 1 OFF") {
        setControlKeys("<--");
    } else if (getControlKeys() == "--> + 1 OFF") {
        setControlKeys("-->");
    } else if (getControlKeys() == "<-- + --> OFF") {
        setControlKeys("<--");
    } else if (getControlKeys() == "--> + <-- OFF") {
        setControlKeys("-->");
    } else if (getControlKeys() == "1 + <-- + --> OFF") {
        setControlKeys("1 + <--");
    } else if (getControlKeys() == "1 + --> + <-- OFF") {
        setControlKeys("1 + -->");
    }
}
    
    
//*** INICIA MOVIMIENTO ***//
void MovementManager::playMovement(const std::string& movementName, int region) {

    // !!! PARA MONITORIZAR EN LA GUI !!! //
    // CUANDO NO HAGA FALTA MONITORIZAR LO METEMOS EN EL IF
    // Actualiza el nombre del movimiento actual
    currentMovementName = movementName;
    
    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movements.count(movementName)) {
        
        // Actualiza el movimiento actual utilizando el movimiento especificado
        currentMovement = std::make_unique<Movement>(movements[movementName]);
        
        
        // Actualiza el nombre del movimiento actual
        //currentMovementName = movementName;
        
        
        // Establece la fila inicial del movimiento
        currentRow = currentMovement->row;
        
        // Establece la región inicial del movimiento
        currentRegion = region;
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
// Implementación de handleRun
void MovementManager::handleRun() {
    // Si RUN no es el movimiento actual
    if (currentMovementName != "RUN") {
        //std::cout << "RUN" << std::endl;
        // Activar RUN
        playMovement("RUN");
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

    
    

// Actualiza los intervalos de frame desde la GUI.
void MovementManager::updateFrameIntervalFromGUI() {
    // Lógica para actualizar los intervalos de frame desde la GUI (a completar)
}

// Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
bool MovementManager::shouldUpdateRegion(float currentTime, float interval) const {
    return currentMovement && (currentTime - lastUpdateTime >= interval);
}



// Set ControlKeys
void MovementManager::setControlKeys(const std::string& value) {
    controlKeys = value;
}
// Get ControlKeys
std::string MovementManager::getControlKeys() const {
    return controlKeys;
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

//Obtiene la dirección del personaje
bool MovementManager::getIsFacingRight() {
    return isFacingRight;
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


