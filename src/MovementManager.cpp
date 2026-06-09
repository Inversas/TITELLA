#include "MovementManager.h"

#include "SpriteSheetManager.h"
//#include "InputManager.h"
//#include "MovementUtilites.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"

// ##########################################
//             ESTADO ACTUAL
// ##########################################

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//             ESTADO OJETIVO
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
//              MOVIMIENTO
// ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈


// *** CONSTRUCTOR ***
MovementManager::MovementManager()
    : inputManager(nullptr),
      physicsManager(nullptr),
      spriteSheetManager(nullptr),
      collisionManager(nullptr),
      currentState(MovementState::IDLE),
      targetState(MovementState::IDLE),
      previousState(MovementState::IDLE),
      currentMovementName("IDLE"),
      waitingForTransition(false),
      isGrounded(true),
      flag_turn_to_run(false)
{
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // El constructor inicializa los estados básicos y asegura que los
    // punteros externos empiecen en un estado seguro (nullptr).
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
}

// *** DESTRUCTOR ***
MovementManager::~MovementManager() {
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // NOTA DE MEMORIA:
    // 0. En elsetup, se recibenlos managers por referencia y guardamos su dirección (this->inputManager = &input).
    // Al ser "prestados", el destructor nunca debe borrarlos (no debemos hacerles 'delete',
    // solo olvidarlos (ponerlos a nullptr). No debemos hacerles 'delete', solo limpiamos por seguridad.
    
    // 1. 'currentMovement' es un std::unique_ptr, por lo que C++ lo borrará
    //    automáticamente de la memoria al destruir esta clase. No hace falta 'delete'.
    
    // 2. Smart Pointers (std::unique_ptr): Al usar std::make_unique<Movement> en el setup, te ahorras el delete.
    // Es la forma moderna y segura de C++ para evitar fugas de memoria (Memory Leaks).
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬

    
    inputManager = nullptr;
    physicsManager = nullptr;
    spriteSheetManager = nullptr;
    collisionManager = nullptr;
    
    // movementUtilities se destruirá automáticamente al salir de aquí.
    // movementCrono se destruirá automáticamente al salir de aquí.


    ofLogNotice("MovementManager") << "Destructor ejecutado: Punteros externos limpiados.";
}



// *** CONFIGURACIÓN Y CONEXIÓN ***
// Inicializa el MovementManager cargando los movimientos desde un archivo JSON y configurando el movimiento actual a "IDLE".
void MovementManager::setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input, PhysicsManager& physics, CollisionManager& collision) {
    
    // Guardamos las referencias de los managers para usarlas luego
    this->inputManager = &input;
    this->physicsManager = &physics;
    this->spriteSheetManager = &spriteSheetManager;
    this->collisionManager = &collision;
    
    // Carga los movimientos desde el archivo JSON proporcionado
    movementUtilities.loadMovements("movements.json");
    

    // Obtenemos el mapa por referencia constante para no copiarlo
    // !!!! VERSIÓN OF 0.12 !!!!
    // Ahora, el Manager le pregunta a Utilities qué archivos de imagen necesita
    // para decirle al SpriteSheetManager que los prepare.
    for (auto const& [name, mov] : movementUtilities.getAllMovements()) {
        spriteSheetManager.loadSpriteSheet(mov.spriteSheetName);
    }
    
    // !!!! VERSIÓN OF 0.10 !!!!
    // Iteramos usando la sintaxis de par (pair)
    /*for (auto const& item : movementUtilities.getAllMovements()) {
        // item.first  sería el name (IDLE, WALK...)
        // item.second sería el objeto Movement
        spriteSheetManager.loadSpriteSheet(item.second.spriteSheetName);
    }*/
    
    // Establece el movimiento actual en "IDLE" utilizando el movimiento cargado desde el JSON
    if (movementUtilities.exists("IDLE")) {
        currentMovement = std::make_unique<Movement>(movementUtilities.getMovement("IDLE"));
        currentMovementName = "IDLE";
    }
    
    // Inicializamos todos los ScaleFactor
    setScaleFactor(1.0f);
}

// *** CICLO DE VIDA ***
// Actualiza el estado del MovementManager basado en el tiempo actual.
void MovementManager::update() {
    
    // Primero el cerebro decide qué hacer
    updateIntent();
    
    // Tiempo que ha pasado desde el último frame
    float dt = ofGetLastFrameTime();
    // Obtiene el intervalo de frame del movimiento actual o GLOBAL si no está definido
    float currentMovementFrameInterval = getCurrentMovementFrameInterval();
    
    // DEBO ACTUALIZAR REGION SEGUN FRAME INTERVAL?
    if (movementCrono.updateRegion(dt, currentMovementFrameInterval)) {
        // Si se ha actualizado la region llamamos a updateFrame
        updateFrame();
    }
}


// ==========================================
//     TRADUCTORES (Intención --> Estado)
// ==========================================
// Intención --> Comando
void MovementManager::translateIntent(){
    
    // Reseteamos el flag IDLE_TURN_TO_RUN
    flag_turn_to_run = false;
    
    // RECUPERAR "deseos" del jugador desde el InputManager
    InputState intent = inputManager->getInputState();
    
    
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                   TURN
    //      QUIERE IR A LA DIRECCIÓN CONTRARIA
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if ( (intent.wantsRight && !isFacingRight) || (intent.wantsLeft && isFacingRight) ) {
            
        // !!!!!!!!! GIRO AIRE !!!!!!!!
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //                AIR TURN
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // ············································································
        // NO GROUNDED
        // ············································································
        if ( !isGrounded ) {
            // ············································································
            // IS JUMP MOVEMENT
            // ············································································
            if( currentMovementName == "JUMP") {
                ofLogNotice("MovementManager") << "-> JUMP en curso, framesRemainingJump= "
                                               << physicsManager->getFramesRemainingJump()
                                               <<  " ->framesRemainingJump - stopFrames = "
                                               << physicsManager->getFramesRemainingJump() - physicsManager->getStopFramesJump();
                // ············································································
                // FRAMES DE SUBIDA RESTANTES =>  frames en JUMP_TURN (ahora 4)
                // ············································································
                // Si de subida quedan almenos los frames de JUMP TURN
                if(physicsManager->checkTurnJump(movementUtilities.getMovement("JUMP_TURN").numRegions)) {
                    currentCommand = MovementCommand::GO_TURN;
                }
                // ············································································
                // FRAMES DE SUBIDA RESTANTES < frames en JUMP_TURN (ahora 4)
                // ············································································
                else {
                    // No puedo girar
                    ofLogNotice("MovementManager") << "GIRO DENEGADO";
                    // ············································································
                    // QUEDAN FRAMES
                    // ············································································
                    if(physicsManager->getFramesRemainingJump() - physicsManager->getStopFramesJump() > 0) {
                        currentCommand = MovementCommand::GO_JUMP;
                    }
                    // ············································································
                    // SALTO CORTADO O SALTO MÍNIMO O SALTO TERMINADO NATURALMENTE
                    // ············································································
                    else {
                        // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO - TO DO ANIMATION] |||||||||||||||
                        // SI ES POR SALTO CORTO, crear animación JUMP_TO_FALL_TURN
                        // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
                        currentCommand = MovementCommand::GO_FALL;
                    }
                }
            }
            // ············································································
            // IS FALL MOVEMENT
            // ············································································
            else if (currentMovementName == "FALL") {
                
                // ············································································
                // CHOQUE CON SUELO EN <= farmes de FALL_TURN (ahora 4)
                // ············································································
                if(collisionManager->checkTurnFall(physicsManager->getPosition(), physicsManager->getVelocity(), physicsManager->getGravityY(), getIsFacingRight(),movementUtilities.getMovement("FALL_TURN").numRegions)) {
                    currentCommand = MovementCommand::GO_TURN;
                }
            }
            return;
        }
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //                GORUND TURN
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // ············································································
        // IS GROUNDED / NO ES TO_JUMP
        // ············································································
        else if(currentMovementName != "IDLE_TO_JUMP") {
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //           flag IDLE_TURN_TO_RUN
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            // Si el giro desde IDLE se da con 1 presionado
            if (intent.wantsRun) {
                flag_turn_to_run = true;
            }
            currentCommand = MovementCommand::GO_TURN;
            return;
        }
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //               TO_FALL
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // ············································································
    // NO DIRECTION (DE MOMENTO HASTA TENER MOVIMIENTO X EN AIRE) / NO GROUNDED / NO IMPULSING
    // ············································································
    if (!intent.hasAnyDirection && !isGrounded && !physicsManager->getIsImpulsing()) {
        currentCommand =  MovementCommand::GO_FALL;
        return;
    }


    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //              SALTO EN CURSO
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // ············································································
    // CURRENT STATE JUMPING / NO GROUND
    // ············································································
    if (currentState == MovementState::JUMPING  && !isGrounded) {
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //               SALTO CORTADO
        //         SUELTA SPACE MIENTRAS SUBÍA
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // ············································································
        // SPACE OFF / IS IMPULSING / NO MINJUMP / NO JUMP CUTTED
        // ············································································
        if (!intent.wantsJump && physicsManager->getIsImpulsing() && !physicsManager->getIsMinJump() && !physicsManager->getIsJumpCutted()) {
            physicsManager->cutJump();
            // No ponemos GO FALL, porque tiene que terminar sus frames de JUMP
            // currentCommand = MovementCommand::GO_FALL;
            
            // !!!!!!!!! GIRO AIRE !!!!!!!!
            // En cambio forzamos JUMP, por si hemos cortado el salto durante un giro
            currentCommand = MovementCommand::GO_JUMP;
            return;
        }
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //               SALTO AGOTADO
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // ············································································
        // NO IMPULSING
        // ············································································
        if (!physicsManager->getIsImpulsing() ) {
            currentCommand = MovementCommand::GO_FALL;
            return;
        }
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //               SIGUE SUBIENDO
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Mantener Estado
        currentCommand = MovementCommand::GO_JUMP;
        return;
        
        
        
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //              TO_JUMP
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // ············································································
    // SPACE ON / IS GROUNDED
    // ············································································
    // debería ser solo para IDLE / WALK / RUN -> TO_JUMP
    if (intent.wantsJump && isGrounded) {
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Para el doble salto??
    /*if (intent.wantsJump && !isGorunded) {
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }*/
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO IDLE
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // (VENIMOS DE NO GIRO) / (grounded true o false)
    // NO DIRECTION
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    //  WANT RIGHT / IS WALLED RIGHT
    //  WANT LEFT / IS WALLED LEFT
    if (!intent.hasAnyDirection || (intent.wantsRight && isWalledRight) || (intent.wantsLeft && isWalledLeft) ) {
        currentCommand =  MovementCommand::GO_STOP;
        return;
    }

    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 HAY DIRECCIÓN
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // WANT DIRECTION
    if (intent.wantsRight || intent.wantsLeft) {
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //               TO RUNNING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // WANT RUN (AVANZAR RÁPIDO)
        if (intent.wantsRun) {
            currentCommand = MovementCommand::GO_FORWARD_FAST;
            return;
            
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //              TO WALKING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // WANT DIRECTION (AVANZAR)
        } else {
            currentCommand = MovementCommand::GO_FORWARD;
            return;
        }
    }

    
    // Por seguridad, si algo fallara, devolvemos parada.
    currentCommand = MovementCommand::GO_STOP;
    return;
}
// Comando --> Estado Objetivo
void MovementManager::updateIntent() {
    
    // TRADUCCIÓN DE INTENCIÓN A COMANDO
    translateIntent();
    
    // TRADUCCIÓN DE COMANDO A ESTADO
    switch (currentCommand) {
        case MovementCommand::GO_TURN:
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::TURNING;
            break;
            
        case MovementCommand::GO_FORWARD:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO WALKING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::WALKING;
            break;
            
        case MovementCommand::GO_JUMP:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::JUMPING;
            break;
        
        case MovementCommand::GO_FALL:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO FALLING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::FALLING;
            break;
            
            
        case MovementCommand::GO_FORWARD_FAST:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO RUNNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::RUNNING;
            break;
            
        case MovementCommand::GO_STOP:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO STOPPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            targetState = MovementState::IDLE;
            break;
    }
    
    
    
    // LLAMADA AL JUEZ
    // Le pasamos nuestra intención. Él decidirá si se puede cumplir y cómo.
    updateState(targetState);
}



// ==========================================
//      UPDATE STATE ( JUEZ )
// ==========================================
// Switch de estados y llama a playMovement o handleTransition según convenga.
void MovementManager::updateState(MovementState targetState) {
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Tendremos que estudiar los casos en que se pierde el suelo
    
    // Se propone algo así, pero todo lo que sea romper de emergencia es peligroso para el proyecto, no podemos perder las transiciones
    
    /*if (!isGrounded && currentState != MovementState::AIRBORNE) {
        forceTransitionToAir();
        return;
    }*/
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    
    
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Hemo añadido el control para no cortar nunca una transición.
    // TENERLO EN CUENTA PARA EL FUTURO
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // NO SE PUEDE CORTAR UNA TRANSICION
    if(!currentMovement->isTransition){
        // SUB MAQUINAS DE ESTADOS
        if (isGrounded) {
            updateGroundedState(targetState);
            return;
        }
        else {
            updateAirState(targetState);
            return;
        }
    }
}
// !!!!!!! JUEZ DE SUELO !!!!!!! //
void MovementManager::updateGroundedState(MovementState targetState) {
    
    // Bloqueo si Ya estamos en el estado deseado
    if (currentState == targetState) return;
    
    
    // ##############################################################################################################################
    //                              CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES (ARREPENTIMIENTOS)
    // ##############################################################################################################################
    
    // Si estamos esperando llegar a un Punto de Salida
    if(waitingForTransition){
        // Gestionamos arrepentimientos y redirecciones
        // Aunque haya actuado, ya ha hecho la gestión
        handleWaitingInterrupt();
        // Ha entrado por Waiting For Transition
        return;
    }

    // PARA ARREPENTIMIENTOS Y REDIRECCIONES guardamos el previousState
    previousState = currentState;

    // MÁQUINA DE ESTADOS (Lógica de Transición)
    switch (currentState) {
        
        case MovementState::IDLE:    handleIdleState(targetState, MovementMoment::CHANGE); break;

        case MovementState::WALKING: handleWalkingState(targetState, MovementMoment::CHANGE); break;
            
        case MovementState::RUNNING: handleRunningState(targetState, MovementMoment::CHANGE); break;
            
        // En principio si entra desde aquí solo debería gestionar (IDLE, WALK Y RUN) _TO_JUMP)
        // de moento está vacío porque los ultimos frames de los movimientos _TO_JUMP acaban ya en el aire
        case MovementState::JUMPING: handleJumpingState(targetState, MovementMoment::CHANGE); break;
            
        //PARA LAND
        case MovementState::FALLING:
            // ofLogNotice("MovementManager") << "!!! ESTA FALLING, VAMOS A ATERRIZAR DESDE: " << currentMovementName << "!!!";
            // currentState = MovementState::FALLING;
            playMovement("LAND");
            break;
            
        // ESTOS DOS CASE NO HARÍAN FALTA POR EL CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES
        case MovementState::TURNING:  handleTurningState(targetState, MovementMoment::CHANGE); break;
        case MovementState::STOPPING: handleStoppingState(targetState, MovementMoment::CHANGE); break;
        
        // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
        // Si un case no tiene nada (en este caso UNO) "comparte" el código con el siguiente case que sí lo tenga (en esta caso DOS)
        // case MovementState::UNO:
        // case MovementState:: DOS:
            // lo que haya
        // break;
        // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    }
}
// !!!!!!! JUEZ DE AIRE !!!!!!! //
void MovementManager::updateAirState(MovementState targetState) {

    // Bloqueo si Ya estamos en el estado deseado
    if (currentState == targetState) return;
    
    
    // ##############################################################################################################################
    //                              CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES (ARREPENTIMIENTOS)
    // ##############################################################################################################################
    
    // Si estamos esperando llegar a un Punto de Salida
    if(waitingForTransition){
        ofLogNotice("MovementManager") << "ENTRAMOS INTERRUPT en AIR (NO GROUNDED)";
        // Gestionamos arrepentimientos y redirecciones
        // Aunque haya actuado, ya ha hecho la gestión
        handleWaitingInterrupt();
        // Ha entrado por Waiting For Transition
        return;
    }
    
    // PARA ARREPENTIMIENTOS Y REDIRECCIONES guardamos el previousState
    previousState = currentState;
    
    // MÁQUINA DE ESTADOS (Lógica de Transición)
    switch (currentState) {
            
        case MovementState::IDLE:
        // handleIdleStateAir(targetState, MovementMoment::CHANGE);
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO FALLING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (targetState == MovementState::FALLING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  FALL
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A CAER DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::FALLING;
                
                // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO - TO DO ANIMATION] |||||||||||||||
                // NO TENEMOS MOVIMIENTO IDLE TO FALL
                // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
                
                // EJECUTAMOS TRANSICION A FLEXIBLE (SERÁ A TRANSICIÓN IDLE_TO_FALL)
                playMovement("FALL");
            }
        break;
            
        case MovementState::JUMPING:
        // handleJumpingStateAir(targetState, MovementMoment::CHANGE);
            
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO FALLING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if(targetState == MovementState::FALLING){
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  FALL
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA SALTANDO, VAMOS A CAER DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::FALLING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("JUMP_TO_FALL");
            }
            
            
            // !!!!!!!!! GIRO AIRE !!!!!!!!
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if(targetState == MovementState::TURNING){
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  JUMP
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA SALTANDO, VAMOS A GIRAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::TURNING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("JUMP_TURN");
            }
            
            
        break;
            
        case MovementState::FALLING:
        // handleFallingStateAir(targetState, MovementMoment::CHANGE);
            
            // !!!!!!!!! GIRO AIRE !!!!!!!!
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if(targetState == MovementState::TURNING){
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  FALL
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA CAYENDO, VAMOS A GIRAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::TURNING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("FALL_TURN");
            }
        break;
            

            
        // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
        // ESTAS DOS PODRíAN SER SI DESAPARECE EL SUELO, pero si caminamos y termina el suelo deberíamos ver como lo solucionamos:
        // o da un salto para que cuando acabe este en el aire o pararlo como si fuera una pared
        // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        case MovementState::WALKING:
            // handleWalkingState(targetState, MovementMoment::CHANGE);
            break;
        case MovementState::RUNNING:
            // handleRunningState(targetState, MovementMoment::CHANGE);
            break;
            
            
       // SE PUEDE ENTRAR EN ESTOS CASOS?
       // O SOLO SE GESTIONA EN FINISHED TRANSITION?
        case MovementState::TURNING:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!  HEMOS ENTRADO EN CURRENT STATE TURNING en AIRE!!!";
            // handleTurningState(targetState, MovementMoment::CHANGE);
            break;
        case MovementState::STOPPING:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!  HEMOS ENTRADO EN CURRENT STATE STOPPING en AIRE!!!";
            // handleStoppingState(targetState, MovementMoment::CHANGE);
            break;
    }
}

// ==========================================
//     ARREPENTIMIENTOS y REDIRECCIONES
// ==========================================
bool MovementManager::handleWaitingInterrupt() {
    
    // !!!!!!!!! JUMP !!!!!!!!
    // ESTE ARREPENTIMIENTO CONTEMPLA SI ESTOY EN WALKING, entonces mientras esperamos transicion (el target es otro diferente por ejemplo RUNNING, o JUMPING)
    // pero entonces volvemos al target WALKING antes de PS, volvemos al estadoa anterior.

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         ARREPENTIMIENTO
    // El jugador quiere volver al estado del que venía.
    // Cancelamos la espera y restauramos previousState.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
   if (targetState == previousState) {
        waitingForTransition = false;
        currentState = previousState;
        return true;
    }
    

    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: STOPPING → TURNING
    // Estaba esperando PS para frenar, pero ahora quiere girar.
    // No cancelamos la espera, la reconfiguramos con turn_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::STOPPING && targetState == MovementState::TURNING) {
        currentState = MovementState::TURNING;
        handleTransition(); // Reconfigura buscando PS en turn_transitions
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: TURNING → RUNNING
    // Estaba esperando PS para girar (venía de WALK), pero ahora quiere correr.
    // No cancelamos la espera, la reconfiguramos con change_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //if (currentState == MovementState::TURNING && previousState == MovementState::WALKING && targetState == MovementState::RUNNING) {
    if (currentState == MovementState::TURNING && targetState == MovementState::RUNNING) {
        currentState = MovementState::RUNNING;
        handleTransition(); // Reconfigura buscando PS en change_transitions
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: STOPPING → WALKING
    // Estaba esperando PS para frenar (venía de RUN), pero ahora quiere caminar.
    // No cancelamos la espera, la reconfiguramos con change_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //if (currentState == MovementState::STOPPING && previousState == MovementState::RUNNING && targetState == MovementState::WALKING) {
    if (currentState == MovementState::STOPPING && targetState == MovementState::WALKING) {
        currentState = MovementState::WALKING;
        handleTransition(); // Reconfigura buscando PS en change_transitions
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: TURNING → WALKING
    // Estaba esperando PS para girar (venía de RUN), pero ahora quiere caminar.
    // No cancelamos la espera, la reconfiguramos con change_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //if (currentState == MovementState::TURNING && previousState == MovementState::RUNNING && targetState == MovementState::WALKING) {
    if (currentState == MovementState::TURNING && targetState == MovementState::WALKING) {
        currentState = MovementState::WALKING;
        handleTransition(); // Reconfigura buscando PS en change_transitions
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: STOPPING → RUNNING
    // Estaba esperando PS para frenar (venía de WALK), pero ahora quiere correr.
    // No cancelamos la espera, la reconfiguramos con change_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //if (currentState == MovementState::STOPPING && previousState == MovementState::WALKING && targetState == MovementState::RUNNING) {
    if (currentState == MovementState::STOPPING  && targetState == MovementState::RUNNING) {
        currentState = MovementState::RUNNING;
        handleTransition();
        return true;
    }
    
    // !!!!!!!!! JUMP !!!!!!!!
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: STOPPING → JUMPING
    // Estaba esperando PS para frenar, pero ahora quiere saltar.
    // No cancelamos la espera, la reconfiguramos con jump_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::STOPPING && targetState == MovementState::JUMPING) {
        currentState = MovementState::JUMPING;
        handleTransition(); // Reconfigura buscando PS en jump_transitions
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: TURNING → STOPPING
    // Estaba esperando PS para girar, pero ahora quiere frenar.
    // No cancelamos la espera, la reconfiguramos con stop_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::TURNING && targetState == MovementState::IDLE) {
        currentState = MovementState::STOPPING;
        handleTransition(); // Reconfigura buscando PS en stop_transitions
        return true;
    }
    

    
    // !!!!!!!!! JUMP !!!!!!!!
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: TURNING → JUMPING
    // Estaba esperando PS para girar, pero ahora quiere frenar.
    // No cancelamos la espera, la reconfiguramos con stop_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::TURNING && targetState == MovementState::JUMPING) {
        currentState = MovementState::JUMPING;
        handleTransition(); // Reconfigura buscando PS en jump_transitions
        return true;
    }
    
    
    // !!!!!!!!! JUMP !!!!!!!!
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: JUMPING → STOPPING
    // Estaba esperando PS para saltar, pero ahora quiere frenar.
    // No cancelamos la espera, la reconfiguramos con stop_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::JUMPING && targetState == MovementState::IDLE) {
        currentState = MovementState::STOPPING;
        handleTransition(); // Reconfigura buscando PS en stop_transitions
        return true;
    }
    
    // !!!!!!!!! JUMP !!!!!!!!
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: JUMPING → TURNING
    // Estaba esperando PS para saltar, pero ahora quiere girar.
    // No cancelamos la espera, la reconfiguramos con turn_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::JUMPING && targetState == MovementState::TURNING) {
        currentState = MovementState::TURNING;
        handleTransition(); // Reconfigura buscando PS en turn_transitions
        return true;
    }
    
    
    
    
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // ESTAS DOS TENGAMOSLAS EN MENTE.
    // PODRÍA DAR LA SENSACIÓN de que NO SALTA CUANDO TOCA:
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: from WALKING → wating for JUMPING → now want RUNNING
    // Estaba esperando PS para saltar caminando, pero ahora quiere correr.
    // No cancelamos la espera, la reconfiguramos con change_transitions ????
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::JUMPING && previousState == MovementState::WALKING && targetState == MovementState::RUNNING) {
        currentState = MovementState::RUNNING;
        handleTransition();
        return true;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //         REDIRECCIÓN: from RUNNING → wating for JUMPING → now want WALKING
    // Estaba esperando PS para saltar coriendo, pero ahora quiere caminar.
    // No cancelamos la espera, la reconfiguramos con change_transitions ????
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::JUMPING && previousState == MovementState::RUNNING && targetState == MovementState::WALKING) {
        currentState = MovementState::WALKING;
        handleTransition();
        return true;
    }
    
    // Ningún caso aplicable: bloqueamos hasta llegar al PS
    return false;
}

// ==========================================
//      DISPARAR TRANSICIONES
// ==========================================
void MovementManager::triggerTransition() {
    if (isGrounded) {
        triggerGroundedTransition();
    } else {
        triggerAirTransition();
    }
}
// !!!!!!! TRANSICIONES DE SUELO !!!!!!! //
void MovementManager::triggerGroundedTransition(){

    previousState = currentState;

    switch (currentState) {
            // ##########################################
            //              IDLE TRANS
            // ##########################################
            // EN VERDAD NO TIENE, en principio no la llamamos nunca
        case MovementState::IDLE:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!  HEMOS ENTRADO EN CURRENT STATE IDLE trigger transition!!!";
            handleIdleState(targetState, MovementMoment::TRIGGER);
        break;
            
            // ##########################################
            //              TURNING TRANS
            // ##########################################
        case MovementState::TURNING: handleTurningState(targetState, MovementMoment::TRIGGER); break;
            
            // ##########################################
            //              WALKING  TRANS
            // ##########################################
            // Si Estabamos esperando transicion y el currentState era WALKING: solo puede ser RUN_TO_WALK
        case MovementState::WALKING: handleWalkingState(targetState, MovementMoment::TRIGGER); break;
            
            // ##########################################
            //              RUNNING  TRANS
            // ##########################################
            // Si Estabamos esperando transicion y el currentState era RUNNING: solo puede ser WALK_TO_RUN (por ejemplo para frenar se ha puesto STOPPING)
        case MovementState::RUNNING: handleRunningState(targetState, MovementMoment::TRIGGER); break;
            
            // ##########################################
            //               IDLE TRANS
            // ##########################################
        case MovementState::STOPPING: handleStoppingState(targetState, MovementMoment::TRIGGER); break;
            
            // !!!!!!!!! JUMP !!!!!!!!
            // ##########################################
            //           WALKING JUMP TRANS
            // ##########################################
        case MovementState::JUMPING: handleJumpingState(targetState, MovementMoment::TRIGGER); break;
        break;
            
            
            
        case MovementState::FALLING:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!  HEMOS ENTRADO EN CURRENT STATE FALLING en GROUND para TRIGGER TRANS!!!";
        break;
    }
}
// !!!!!!! TRANSICIONES DE AIRE !!!!!!! //
void MovementManager::triggerAirTransition() {
    ofLogNotice("MovementManager") << "TRANSICION AIRE";
    
    // No hay nada porque en el aire no tenemos PS
    
}


// ##############################################################################################################################
//                                               TRANSICION COMPLETADA
// ##############################################################################################################################
void MovementManager::finishedTransition() {
    
     // Recuperamos el origen de la transicion.
     TransitionOrigin origin = currentMovement->originType;

     
     // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
     //              ANY "TURN"
     // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
     if (origin == TransitionOrigin::IDLE_TURN_TO_IDLE ||
         origin == TransitionOrigin::IDLE_TURN_TO_RUN  ||
         origin == TransitionOrigin::WALK_TURN    ||
         origin == TransitionOrigin::RUN_TURN ||
         origin == TransitionOrigin::JUMP_TURN ||
         origin == TransitionOrigin::FALL_TURN) {
         toggleIsFacingRight();
     }


    // FINISHED TRANSITIONS
    if (isGrounded) {
        finishedGroundedTransition();
    } else {
        finishedAirTransition();
    }
    
    updateIntent();
}

// ##############################################################################################################################
//                                            TRANSICION COMPLETADA SUELO
// ##############################################################################################################################
void MovementManager::finishedGroundedTransition() {
    
    TransitionOrigin origin = currentMovement->originType;
    
    ofLogNotice("MovementManager") << "FINAL TRANSICION SUELO: " << currentMovement->name << " (Origen: " << static_cast<int>(origin) << ")";
    
    // Guardamos el estado actual antes de cambiarlo
    previousState = currentState;
    
    

    switch (origin) {

            // ######################################################
            //   IDLE_TO_TURN: el giro simple desde IDLE terminó
            //   → volvemos a IDLE, luego el sistema decide
            // ######################################################
            case TransitionOrigin::IDLE_TURN_TO_IDLE:
                currentState = MovementState::IDLE;
                playMovement("IDLE");
                break;

            // ######################################################
            //   TURN_TO_RUN: el giro con intención de correr terminó
            //   → arrancamos RUN directamente, luego el sistema decide
            // ######################################################
            case TransitionOrigin::IDLE_TURN_TO_RUN:
                currentState = MovementState::RUNNING;
                playMovement("RUN", currentMovement->target_frame);
                break;

            // ######################################################
            //   TO_WALK: agrupa IDLE_TO_WALK y RUN_TO_WALK
            //   → arrancamos WALK, luego el sistema decide
            // ######################################################
            case TransitionOrigin::TO_WALK:
                currentState = MovementState::WALKING;
                playMovement("WALK", currentMovement->target_frame);
                break;

            // ######################################################
            //   TO_RUN: agrupa IDLE_TO_RUN y WALK_TO_RUN
            //   → arrancamos RUN, luego el sistema decide
            // ######################################################
            case TransitionOrigin::TO_RUN:
                currentState = MovementState::RUNNING;
                playMovement("RUN", currentMovement->target_frame);
                break;

            // ######################################################
            //   WALK_TURN: el giro desde WALK terminó
            //   → volvemos a WALK, luego el sistema decide
            //   (si quiere correr o girar de nuevo, lo verá en el siguiente tick)
            // ######################################################
            case TransitionOrigin::WALK_TURN:
                currentState = MovementState::WALKING;
                playMovement("WALK", currentMovement->target_frame);
                break;

            // ######################################################
            //   RUN_TURN: el giro desde RUN terminó
            //   → volvemos a RUN, luego el sistema decide
            //   (si quiere caminar o girar de nuevo, lo verá en el siguiente tick)
            // ######################################################
            case TransitionOrigin::RUN_TURN:
                currentState = MovementState::RUNNING;
                playMovement("RUN", currentMovement->target_frame);
                break;

            // ######################################################
            //   TO_IDLE: agrupa WALK_TO_IDLE y RUN_TO_IDLE (las frenadas)
            //   → llegamos a IDLE, luego el sistema decide
            // ######################################################
            case TransitionOrigin::TO_IDLE:
                currentState = MovementState::IDLE;
                playMovement("IDLE");
                break;


            // !!!!!!!!! JUMP !!!!!!!!!
            // ESTO NO VA, PORQUE LOS ULTIMOS FRAMES DE JUMP YA SON EN EL AIRE
            // ######################################################
            //   TO_JUMP
            // ######################################################
           /* case TransitionOrigin::TO_JUMP:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!!  HEMOS ENTRADO EN FINISHED TRANSITION TO_JUMP en SUELO!!!";
                currentState = MovementState::JUMPING;
                playMovement("JUMP");
                break;
            */
            
            // ######################################################
            //   LAND
            // ######################################################
            case TransitionOrigin::TO_FALL:
               if (targetState == MovementState::WALKING){
                   currentState = MovementState::WALKING;
                   playMovement("LAND_TO_WALK");
                }
                else if (targetState == MovementState::RUNNING){
                    currentState = MovementState::RUNNING;
                    playMovement("LAND_TO_RUN");
                }
                else {
                    currentState = MovementState::IDLE;
                    playMovement("LAND_TO_IDLE");
                }
            break;
            
            
            
            // ######################################################
            //   NONE u origen desconocido: seguridad, vamos a IDLE
            // ######################################################
            default:
                currentState = MovementState::IDLE;
                playMovement("IDLE");
                break;
        }
}

// ##############################################################################################################################
//                                            TRANSICION COMPLETADA AIRE
// ##############################################################################################################################
void MovementManager::finishedAirTransition(){
    ofLogNotice("MovementManager") << "FINAL TRANSICION AIRE";
    
    TransitionOrigin origin = currentMovement->originType;
    
    // Guardamos el estado actual antes de cambiarlo
    previousState = currentState;

    switch (origin) {

            // ######################################################
            //  TO_JUMP
            // ######################################################
            // Los ultimos frames de IDLE / WALK / RUN -> a TO_JUMP son en el aire
            // UN HIPOTETICO DOBLE SALTO JUMP_TO_JUMP, también llegaría por aquí
            case TransitionOrigin::TO_JUMP:
                currentState = MovementState::JUMPING;
                playMovement("JUMP");
            break;
            
            // ######################################################
            //  TO_FALL
            // ######################################################
            case TransitionOrigin::TO_FALL:
                currentState = MovementState::FALLING;
                playMovement("FALL");
                break;
            
            
            // !!!!!!!!! GIRO AIRE !!!!!!!!
            // ######################################################
            //   JUMP_TURN: el giro desde JUMP (en el aire) terminó
            //   → miramos si debemos ir a JUMP_TO_FALL
            //   → sino volvemos a JUMP
            // ######################################################
            case TransitionOrigin::JUMP_TURN:
                /*if( physicsManager->getFramesRemainingJump()-2 == 0){
                    currentState = MovementState::FALLING;
                    playMovement("JUMP_TO_FALL");
                }
                else{*/
                    currentState = MovementState::JUMPING;
                    playMovement("JUMP");
                //}
                break;
            
            // !!!!!!!!! GIRO AIRE !!!!!!!!
            // ######################################################
            //   FALL_TURN: el giro desde FALL (en el aire) terminó
            //   → volvemos a FALL?
            //   → o miramos si debemos ir a LAND?
            //  de momento aplicamos el concepto "trust the next tick": vamos a FALL i confiamos que el sistema gestionará
            // ######################################################
            case TransitionOrigin::FALL_TURN:
                currentState = MovementState::FALLING;
                playMovement("FALL");
                break;
        
        // ESTOS NO VAN PORQUE SON DE SUELO
        case TransitionOrigin::IDLE_TURN_TO_IDLE:
        case TransitionOrigin::IDLE_TURN_TO_RUN:
        case TransitionOrigin::TO_WALK:
        case TransitionOrigin::TO_RUN:
        case TransitionOrigin::WALK_TURN:
        case TransitionOrigin::RUN_TURN:
        case TransitionOrigin::TO_IDLE:
        default:
            //currentState = MovementState::IDLE;
            //playMovement("IDLE");
        break;
            
        /*
        // ######################################################
        //   NONE u origen desconocido: seguridad, vamos a IDLE
        // ######################################################
        default:
            currentState = MovementState::IDLE;
            playMovement("IDLE");
            break;
         */
        }
}


// ==========================================
//      HANDELERS DE ESTADOS
// ==========================================

// ##############################################################################################################################
//                                                        IDLE
// ##############################################################################################################################
void MovementManager::handleIdleState(MovementState target, MovementMoment moment) {
    
    switch (moment) {
        
        // ========================================================================
        //                                UPDATE STATE
        // ========================================================================
        case MovementMoment::CHANGE:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (targetState == MovementState::TURNING) {
                
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //             IDLE TURN TO IDLE
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // Si no quiere correr desde IDLE
                if(!flag_turn_to_run){
                    // ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A GIRAR DESDE: " << currentMovementName << "!!!";
                    // ACTUALIZAMOS ESTADO
                    currentState = MovementState::TURNING;
                    // EJECUTAMOS GIRO (COMPROMETIDO)
                    playMovement("IDLE_TURN_TO_IDLE");

                }
                
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //             IDLE TURN TO RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // Si quiere correr desde IDLE
                else {
                    // ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A GIRAR DESDE: " << currentMovementName << "!!!";
                    // ACTUALIZAMOS ESTADO
                    currentState = MovementState::TURNING;
                    // EJECUTAMOS GIRO (COMPROMETIDO)
                    playMovement("IDLE_TURN_TO_RUN");
          
                }
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO WALKING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::WALKING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  WALK
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A EMPEZAR A CAMINAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("IDLE_TO_WALK");
             
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO RUNNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::RUNNING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A EMPEZAR A CORRER DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("IDLE_TO_RUN");
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  JUMP
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::JUMPING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("IDLE_TO_JUMP");
            }
        break;
            
            
            
            
        
        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            // NO HACE NADA
            // ofLogNotice("MovementManager") << "!!! NO HACE NADA" << "!!!";

        break;
    }
}

// ##############################################################################################################################
//                                                       WALKING
// ##############################################################################################################################
void MovementManager::handleWalkingState(MovementState target, MovementMoment moment) {
    
    switch (moment) {
            
        // ========================================================================
        //                                UPDATE STATE
        // ========================================================================
        case MovementMoment::CHANGE:
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO IDLE
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (targetState == MovementState::IDLE) {
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //               STOPPING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                // ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A FRENAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::TURNING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //             WALK TO TURN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO RUNNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::RUNNING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //              WALK TO RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A CORRER DESDE: "  << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::RUNNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // !!!!!!!!! JUMP !!!!!!!!
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //              WALK TO JUMP
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                ofLogNotice("MovementManager") << "!!! CAMBIAMOS ESTADO A JUMPING " << "!!!";
                currentState = MovementState::JUMPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                ofLogNotice("MovementManager") << "!!! BUSCAR PS Y ACTIVAR ESPERA PARA: " << "WALK_TO_JUMP" << "!!!";
                handleTransition();
            }
        break;

        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //              WALK TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // ACTUALIZAMOS ESTADO
            if (currentMovementName == "WALK_TO_RUN") {
                currentState = MovementState::RUNNING; // Lo marcamos como RUNNING (aunque ya se ha marcado en updateState, por ser transición)
            }
            // EJECUTAMOS
            playMovement(currentMovement->change_transitions[targetRegion]);
            
            
             // !!!!!!!!! JUMP !!!!!!!!
             // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             //              WALK TO JUMP
             // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             // No va aquí porque el estado ya es JUMPING y no WALKING
        break;
            
            
            
    }
    
    // IF FINISHED (esto no sé, porque va por wants y no por estados...)
    
    /* FINISHED TRANSITION */
    /*
     ???????
     */

}

// ##############################################################################################################################
//                                                       RUNNING
// ##############################################################################################################################
void MovementManager::handleRunningState(MovementState targetState, MovementMoment moment) {
    
    switch (moment) {
            
        // ========================================================================
        //                                UPDATE STATE
        // ========================================================================
        case MovementMoment::CHANGE:
            
            if (targetState == MovementState::IDLE) {
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //               STOPPING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                // ofLogNotice("MovementManager") << "!!! ESTA RUNNING, VAMOS A FRENAR DESDE: "  << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::TURNING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //             RUN TO TURN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ofLogNotice("MovementManager") << "!!! ESTA RUNNING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO WALKING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::WALKING) {
                // ofLogNotice("MovementManager") << "!!! ESTA RUNNING, VAMOS A CAMINAR DESDE: "  << currentMovementName << "!!!";
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //             RUN TO WALK
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::WALKING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
            
            // !!!!!!!!! JUMP !!!!!!!!
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //               RUN TU JUMP
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA RUNNING, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::JUMPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                handleTransition();
            }
        break;
            
        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //             RUN TO WALK
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // ACTUALIZAMOS ESTADO
            if (currentMovementName == "RUN_TO_WALK") {
                currentState = MovementState::WALKING; // Lo marcamos como WALKING (aunque ya se ha marcado en updateState, por ser transición)
            }
            // EJECUTAMOS
            playMovement(currentMovement->change_transitions[targetRegion]);
            
            
            // !!!!!!!!! JUMP !!!!!!!!
            /*
             // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             //              RUN TO JUMP
             // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             // ACTUALIZAMOS ESTADO
             if (currentMovementName == "RUN_TO_JUMP") {
                 currentState = MovementState::JUMPING; // Lo marcamos como RUNNING (aunque ya se ha marcado en updateState, por ser transición)
             }
             // EJECUTAMOS
             playMovement(currentMovement->jump_transitions[targetRegion]);
             */
        break;
    }
    
    
    // IF FINISHED (esto no sé, porque va por wants y no por estados...)
    
    /* FINISHED TRANSITION */
    /*
    ???????
    */

}

// ##############################################################################################################################
//                                                   JUMPING
// ##############################################################################################################################
void MovementManager::handleJumpingState(MovementState targetState, MovementMoment moment) {
    switch (moment) {
            
            // ========================================================================
            //                                UPDATE STATE
            // ========================================================================
        case MovementMoment::CHANGE:
            ofLogNotice("MovementManager") << "!!! !!! !!! !!! !!! !!! !!! !!! !!! !!! " << "LO LLAMA ALGUIEN AQUI?? " << "!!!";
            break;
            
            // ========================================================================
            //                           TRIGGER TRANSITION
            // ========================================================================
        case MovementMoment::TRIGGER:
            // !!!!!!!!! JUMP !!!!!!!!
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //              WALK TO JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // ACTUALIZAMOS ESTADO
            if (currentMovementName == "WALK_TO_JUMP" || currentMovementName == "RUN_TO_JUMP") {
                // Lo marcamos como JUMPING
                currentState = MovementState::JUMPING;
            }
            // EJECUTAMOS
            playMovement(currentMovement->jump_transitions[targetRegion]);
            
            break;
    }
}



// ##############################################################################################################################
//                                                       TURNING
// ##############################################################################################################################
void MovementManager::handleTurningState(MovementState targetState, MovementMoment moment) {
   
    switch (moment) {
            
        // ========================================================================
        //                                UPDATE STATE
        // ========================================================================
        case MovementMoment::CHANGE:
            // ofLogNotice("MovementManager") << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!";
        break;
            
        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::TURNING;
            // EJECUTAMOS
            playMovement(currentMovement->turn_transitions[targetRegion]);
        break;
    }
}

// ##############################################################################################################################
//                                                       STOPPING
// ##############################################################################################################################
void MovementManager::handleStoppingState(MovementState targetState, MovementMoment moment) {
    switch (moment) {
            
        // ========================================================================
        //                                UPDATE STATE
        // ========================================================================
        case MovementMoment::CHANGE:
            // ofLogNotice("MovementManager") << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!";
            break;
            
        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::STOPPING;
            // EJECUTAMOS
            playMovement(currentMovement->stop_transitions[targetRegion]);
        break;
    }
}






// ==========================================
//      MOTOR
// ==========================================
void MovementManager::updateFrame() {
   
// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Suma un pequeño incremento a la velocidad actual, isGrounded
    physicsManager->updateVelocityStep();
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // ESTO VER SI ES PROBLEMA QUE SE EVALUE CADA FRAME
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // RECUPERAR "deseos" del jugador desde el InputManager
    InputState intent = inputManager->getInputState();
    
    
    
   
    // !!!!!!!!! AIR DIRECTION !!!!!!!!
    bool wantsForward = (intent.wantsRight && isFacingRight) || (intent.wantsLeft && !isFacingRight);
    
    // BLOQUEA AVANCE SI SE SALTA EN WALLED
    bool blockedByWall = (intent.wantsRight && isWalledRight) || (intent.wantsLeft && isWalledLeft);

    if (!isGrounded && !blockedByWall) {
        
        // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
        //     TIENE QUE VER CON EL REPLANTEAMIENTO Y FACTORIZACIÓN DE PHYSICS MANAGER
        // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        // Esto hace que al preisonar un avance hasta que no llega a la velocidad máxima no para.
        // Es un tema de concepto, si no lo hacemos así la velocidad no cambia por update solo, sino que se recalcula el paso en cada frame con
        // starteVelocityChange
        // No habíamos encontrado esto, porque siempre usabamos startVelocityChange una sola vez (cuando se ejecuta un movimiento)
        // En AIR direction no funciona así, sino que, si no ponemos el control de isVelocityChanging, cada frame recalcula un nuevo paso, segun la velocidad actual,
        // en lugar de solo actualizarlo en update
        // Sin el control no funciona mal, de hecho tiene sentido, pero las funciones que reutilizamos no estan pensadas para llamarlas en cada Frame.
        if( !physicsManager->getIsVelocityChanging()){

            // QUEREMOS IR HACIA ADELTANTE Y VELOCIDAD en X es 0
            if (wantsForward && physicsManager->getVelocity().x == 0) {
                // Si no coinciden la velocidad BASE en aire escalada con la velocidad actual
                if( (physicsManager->getMaxSpeedAir() != physicsManager->getBaseMaxSpeedAir()*sqrt(physicsManager->getCurrentScale() ))){
                    // Igualamos porque hemos llegado a 0, así volvemos a la original
                    physicsManager->setMaxSpeedAir(physicsManager->getBaseMaxSpeedAir());
                }
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedAir(), physicsManager->getAirForwardFrames(), isFacingRight);
            }
            
            // NO QUEREMOS IR HACIA ADELTANTE Y VELOCIDAD en X NO es 0
            else if (!wantsForward && physicsManager->getVelocity().x != 0) {
                // Evitamos que nos corte la inercia si justo estamos en el último
                // frame de un giro (donde isFacingRight aún no se ha actualizado).
                if (currentState != MovementState::TURNING) {
                    // bajada hacia 0
                    physicsManager->startVelocityChange(0, physicsManager->getAirUnforwardFrames(), isFacingRight);
                }
            }
        }
    }
    // Si está en el aire pero chocamos con pared
    else if(!isGrounded && blockedByWall){
        // Esto queda independiente del valor physicsManager->getIsVelocityChanging(), porque debemos poder ir a 0 desde la velocidad actual, aunque estmaos en medio de una cambio de velocidad
        // Vamos hacia 0
        physicsManager->startVelocityChange(0, physicsManager->getAirUnforwardFrames(), isFacingRight);
    }
    

    // Gestiona el delay, el impulso y el conteo de frames de subida
    physicsManager->updateJumpStep(intent.wantsJump, (intent.wantsRight|| intent.wantsLeft) );
    // Gestiona el frenado y el hanging de JUMP_TO_FALL
    physicsManager->updateJumpToFallStep();
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
    
// >>>>>>>>>>>>>>>>>> COLISIONES >>>>>>>>>>>>>>>>>
    // CHECK COLLISIONS futuro
    CollisionResult col = collisionManager->checkCollisions(
        physicsManager->getPosition(),
        physicsManager->getVelocity(),
        physicsManager->getGravityY(),
        getIsFacingRight()
    );

    // LÓGICA DE ESTADO
    isGrounded = col.isGrounded;
    isWalledLeft = col.isWalledLeft;
    isWalledRight = col.isWalledRight;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    
    
// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    if(!isGrounded){
        // Si no estamos en el suelo, aplicamos gravedad
        physicsManager->applyGravity();
    } else {
        // Si estamos en el suelo, la velocidad vertical es 0
        physicsManager->setVelocityY(0);
        // Subimos media región para que la base del cuadrado de 300px (en escala 1) apoye en el suelo.
        // Usamos la Hitbox de trabajo, por si ha cambiado la escala, entonces no son 300 y cojemos el tamaño escalado.
        float yPerfecta = col.floor->p1.y - (collisionManager->getHitbox().regionH/2);
        
        // Asegura que el personaje no salte ni se pase del suelo
        physicsManager->setPositionY(yPerfecta);
    }
    
    // Aplica velocidad
    physicsManager->applyVelocity();
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
    
    
    // ##############################################################################################################################
    //                                                DISPARAR TRANSICION
    // ##############################################################################################################################
    // Si estamos esperando una transición y hemos llegado al Punto de Salida,
    if (waitingForTransition && movementCrono.getCurrentRegion() == targetRegion) {
        // Reset Waiting
        waitingForTransition = false;
        // ofLogNotice("MovementManager") << "!!! DISPARAR TRANSICION : !!!";
        triggerTransition();
        return;
    }

    // ##############################################################################################################################
    //                                               TRANSICION COMPLETADA
    // ##############################################################################################################################
    // Si el movimiento actual es una transición y se ha completado,
    if (currentMovement->isTransition && movementCrono.getCurrentRegion() >= currentMovement->numRegions) {
        // ofLogNotice("MovementManager") << "!!! TRANSICION COMPLETADA : !!!";
        finishedTransition();
        return;
    }
    
    
    // ##############################################################################################################################
    //                                               MOVIMIENTO BUCLE
    // ##############################################################################################################################
    // Si la región actual alcanza el último frame,
    if (movementCrono.getCurrentRegion() >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       movementCrono.setCurrentRegion(0);
    }
}

// ==========================================
// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
// ==========================================
void MovementManager::handleMovementPhysics(const std::string& name) {
    // Si currentMovement no existe, o no está inicializado, abortamos
    if (!currentMovement) return;

    // Si el nombre del movimiento no existe en nuestro mapa, abortamos
    if (movementUtilities.getAllMovements().find(name) == movementUtilities.getAllMovements().end()) return;
    
    
    // Obtenemos el número de frames (regiones) que dura esta animación
    int frames = movementUtilities.getMovement(name).numRegions;
    
    // ofLogNotice("MovementManager") << "Analizando: " << name;
    switch (currentState) {
            
        // ==========================================
        //     ESTADO IDLE: sin física activa
        // ==========================================
        case MovementState::IDLE:
            // IDLE no aplica físicas, pero:
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // AL DISPARAR LAND TO IDLE el state es IDLE, handleMovementPhyisics va "como después" en terminos de States.
            // EN FINISHED TRANSITION LAND DEBERÍAMOS PONER STOPPING? Y QUE SE GESTIONE ALLÍ?
            // BUENO PARECE NO TENER FÍSICAS, porque LAND ya va a 0
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            if (name == "LAND_TO_IDLE"){
                ofLogNotice("MovementManager") << "-> Detectado LAND_TO_IDLE, aplicar FISICA";
            }
        break;
            
        // ==========================================
        //               STOPPING
        // ==========================================
        case MovementState::STOPPING:
            // ofLogNotice("MovementManager") << "-> Detectado FRENADO (TO_IDLE), aplicar FISICA";
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           RUN TO IDLE 1 i 2
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // RUN TO IDLE debe terminar antes, es una frenada más brusca
            if (name.find("RUN_TO_IDLE") != std::string::npos) {
                // Crea una rampa hacia velocidad 0 usando los frames menos 1 de la animación de frenado
                physicsManager->startVelocityChange(0, frames-1, isFacingRight);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           WALK TO IDLE 1 i 2 (todos los que no son RUN_TO_IDLE
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else {
                // Crea una rampa hacia velocidad 0 usando los frames de la animación de frenado
                physicsManager->startVelocityChange(0, frames, isFacingRight);
            }
        break;
    
        // ==========================================
        //   TURNING: cualquier movimiento de giro
        // ==========================================
        case MovementState::TURNING:
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           IDLE TURN TO IDLE
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             if (name == "IDLE_TURN_TO_IDLE") {
                 // Giro en parado: no hay velocidad que invertir
                 // ofLogNotice("MovementManager") << "-> Detectado GIRO PARADO";
             }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           IDLE TURN TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             else if (name == "IDLE_TURN_TO_RUN") {
                 // ofLogNotice("MovementManager") << "-> Detectado IDLE_TURN_TO_RUN, aplicar FISICA";

                 // isFacingRight ya está cambiado pero esta animación en concreto debe interpretar que aun no lo está
                 // al terminar y pasarle el testigo a otra animación ya estará bien.
                 physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames, !isFacingRight, 3);
             }
            
            // !!!!!!!!! GIRO AIRE !!!!!!!
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //      TURN (WALK / RUN / JUMP / FALL)
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             else{
                 // Si es JUMP_TURN o FALL_TURN)
                 if (name == "JUMP_TURN" || name == "FALL_TURN") {
                     // Rampa para ir a la velocidad maxima en aire en sentido contrario
                     physicsManager->startVelocityTurnChangeAir(physicsManager->getMaxSpeedAir(), frames, !isFacingRight);
                 }
                 // Para el resto de giros que no son desde IDLE, ni son cuando llevamos velocidad en JUMP_TURN y FALL_TURN
                 else {
                     // Inicia la rampa en 'V' para invertir la velocidad en el tiempo de giro
                     physicsManager->startVelocityTurnChange(frames);
                 }
              }
        break;
        
        // ==========================================
        //                  WALKING
        // ==========================================
        case MovementState::WALKING:
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //            RUN TO WALK (1 i 2)
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            if (name.find("RUN_TO_WALK") != std::string::npos) {
                // ofLogNotice("MovementManager") << "-> Detectado RUN_TO_WALK, aplicar FISICA";
                // Baja la velocidad hasta el tope de caminar usando los frames de transición más 3 frames "prestados" del ciclo WALK
                physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames + 3, isFacingRight);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //     WALK / IDLE TO WALK / LAND TO WALK
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else {
                if(name == "LAND_TO_WALK"){
                    // ofLogNotice("MovementManager") << "-> Detectado LAND_TO_WALK, aplicar FISICA";
                    physicsManager->startVelocityChange(physicsManager->getMaxSpeedWalk()/2, frames, isFacingRight);
                }
                else {
                    // ofLogNotice("MovementManager") << "-> Detectado WALK o IDLE_TO_WALK, aplicar FISICA";
                    // Mantiene (WALK) o ajusta la velocidad al máximo de caminar durante el primer ciclo de IDLE_TO_WALK
                    physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames, isFacingRight );
                }
            }
        break;
            
        // ==========================================
        //                  RUNNING
        // ==========================================
        case MovementState::RUNNING:
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //          WALK TO RUN (1 i 2)
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            if (name.find("WALK_TO_RUN") != std::string::npos) {
                // ofLogNotice("MovementManager") << "-> Detectado WALK_TO_RUN, aplicar FISICA";
                // Acelera hasta correr usando la transición + 3 frames "prestados" del ciclo RUN
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames + 3, isFacingRight);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //               IDLE TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // IDLE TO RUN no debe llegar a su velocidad máxima durante los frames del movimiento, usamos un delay
            else if(name == "IDLE_TO_RUN"){
                // ofLogNotice("MovementManager") << "-> Detectado IDLE_TO_RUN, aplicar FISICA";
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedWalk(), frames, isFacingRight, 2);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //       RUN / IDLE TO RUN / LAND TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else {
                if(name == "LAND_TO_RUN"){
                    // ofLogNotice("MovementManager") << "-> Detectado LAND_TO_RUN, aplicar FISICA";
                    physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun()/2, frames, isFacingRight);
                }
                else {
                    // ofLogNotice("MovementManager") << "-> Detectado RUN o IDLTE_TO_RUN, aplicar FISICA";
                    physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames, isFacingRight);
                }
       
                
            }
        break;
            
            
        // ==========================================
        //                  JMPING
        // ==========================================
        case MovementState::JUMPING:
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //               IDLE TO JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            if(name == "IDLE_TO_JUMP"){
                // ofLogNotice("MovementManager") << "-> Detectado IDLE_TO_JUMP, aplicar FISICA ";
                // totalJumpFrames: frames de IDLE_TO_JUMP (tres últimos) (8-5), 3 de JUMP y 2 de JUMP_TO_FALL (stopFrames)
                // Pasamos el total de frames de IDLE_TO_JUMP, y el delay de cuando debe empezar el impulso
                physicsManager->startVelocityJump(frames, 5, isFacingRight);
            }
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // ES POSIBLE QUE ESTO LO PONGAMOS DONDE EL ANTERIOR, IDLE TO JUMP
            // PERO POS SI ACASO ES OTRA FÍSICA LO MANTENEMOS SEPARADO
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            // !!!!!!!!! JUMP !!!!!!!!!
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //          WALK TO JUMP (1 i 2)
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else if (name.find("WALK_TO_JUMP") != std::string::npos) {
                ofLogNotice("MovementManager") << "-> Detectado WALK_TO_JUMP 1 o 2, aplicar FISICA ";
                // totalJumpFrames: frames de WALK_TO_JUMP (tres últimos) (8-5), 3 de JUMP y 2 de JUMP_TO_FALL (stopFrames)
                // Pasamos el total de frames de WALK_TO_JUMP, y el delay de cuando debe empezar el impulso
                physicsManager->startVelocityJump(frames, 5, isFacingRight, physicsManager->getVelocity().x);
            }
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //                  RUN TO JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else if (name.find("RUN_TO_JUMP") != std::string::npos) {
                ofLogNotice("MovementManager") << "-> Detectado RUN_TO_JUMP 1 o 2, aplicar FISICA ";
                
                physicsManager->startVelocityJump(frames, 5, isFacingRight, physicsManager->getVelocity().x);
            }
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //              JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else if(name == "JUMP") {
                // JUMP no llama a nada — el impulso ya está corriendo
                // desde startVelocityJump, solo continuamos contando
                ofLogNotice("MovementManager") << "-> Detectado JUMP, no aplica física";
            }



        break;
            
        // ==========================================
        //                  FALLING
        // ==========================================
        case MovementState::FALLING:

            if(name == "JUMP_TO_FALL"){
                ofLogNotice("MovementManager") << "-> Detectado JUMP_TO_FALL, aplicar FISICA";
                physicsManager->startJumpToFall();
            }
            else if(name == "FALL"){
                // FALL no llama a nada
                ofLogNotice("MovementManager") << "-> Detectado FALL ";
            }
            
            else if (name == "LAND"){
                ofLogNotice("MovementManager") << "-> Detectado LAND";
                physicsManager->resetJumpVariables();
                
                // !!!!!!!!! AIR DIRECTION !!!!!!!!
                physicsManager->startVelocityChange(0, 3, isFacingRight);
            }
        break;
            
        // ==========================================
        //              NO DEFINIDO
        // ==========================================
        default:
            ofLogNotice("MovementManager") << "ESTADO NO CONTEMPLADO PARA FISICAS";
        break;
            
    }
}



// ==========================================
//      EJECUTOR
// ==========================================
void MovementManager::playMovement(const std::string& movementName, int region) {
    
    // SI ESTO ESTA DENTRO DEL IF EL PERSONAJE NO SE PARA EN IDLE
    currentMovementName = movementName;
    
    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movementUtilities.getAllMovements().count(movementName)) {
        
        // Actualiza el movimiento actual utilizando el movimiento especificado
        currentMovement = std::make_unique<Movement>(movementUtilities.getMovement(movementName));
        
        // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
        // ESTO DEBERÍA IR AQUÍ, o en algun lugar dentro del IF, pero habría que investigar un tema sobre la sincronia de MovementCrono y PhysicsManager.
        // Parece ser que aveces uno llega un frame antes, entonces se dispara:
        // ofLogNotice("MovementManager")  << name << " No tiene fisicas";
        // en handled movement physics.
        // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        //currentMovementName = movementName;

        // Establece la fila inicial del movimiento
        currentRow = currentMovement->row;
        
        // Establece la región inicial del movimiento
        movementCrono.setCurrentRegion(region);
        
        // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
        // ofLogNotice("MovementManager")  << "GO FISICAS de: " << currentMovementName;
        handleMovementPhysics(currentMovementName);
    }
}

// ==========================================
//      OTEADOR (mira al futuro)
// ==========================================
void MovementManager::handleTransition() {
    
    if (waitingForTransition) return;
    
    // Obtiene una referencia al movimiento actual
    Movement& movement = *currentMovement;
    // Inicializa la región de transición más cercana como -1 (no encontrada)
    int closestRegion = -1;
    
    // Variable para guardar el Nombre de la transición a ejecutar
    std::string transitionName;
    
    // *** SELECCIONA TRANSICIONES SEGÚN  ESTADO ***//
    // Usamos un puntero para no copiar el mapa, solo apuntar al que necesitamos
    std::map<int, std::string>* transitionMap = nullptr;
    
    // Transiciones segun ESTADOS
    if (currentState == MovementState::TURNING) {
        transitionMap = &movement.turn_transitions;
    }
    else if (currentState == MovementState::STOPPING) {
        transitionMap = &movement.stop_transitions;
    }
    else if (currentState == MovementState::WALKING || currentState == MovementState::RUNNING) {
        transitionMap = &movement.change_transitions;
    }
    else if (currentState == MovementState::JUMPING) {
        transitionMap = &movement.jump_transitions;
    }

    // Si el mapa no existe o está vacío, no hay transiciones que buscar
    if (!transitionMap || transitionMap->empty()) return;
    
    
    // *** ENCUENTRA LA REGIÓN DE TRANSICIÓN MÁS CERCANA ***//
    // !!!! VERSIÓN OF 0.12 !!!!
    // Iteramos por el transitionMap
    for (auto const& [region, name] : *transitionMap) {
        if (region > movementCrono.getCurrentRegion() && (closestRegion == -1 || region < closestRegion)) {
            closestRegion = region;
        }
    }
    
    // !!!! VERSIÓN OF 0.10 !!!!
    /*for (auto const& pair : *transitionMap) {
        auto const& region = pair.first;
        auto const& name = pair.second;
        
        // ••••••••••••••••••••••••••••••••••••• CRONO •••••••••••••••••••••••••••••••••••••
        if (region > movementCrono.getCurrentRegion() && (closestRegion == -1 || region < closestRegion)) {
            closestRegion = region;
        }
    }*/

    // Si no se encontró una región mayor que la actual, usamos la primera del mapa
    if (closestRegion == -1) {
        // En un std::map, begin() siempre apunta al valor de clave más pequeño
        closestRegion = transitionMap->begin()->first;
    }
    
    // *** DEFINE LA REGIÓN DE TRANSICIÓN Y ACTIVA ESPERA ***//
    // Si se encontró una región de transición válida
    if (closestRegion != -1) {
        // Establece la región objetivo para la transición
        targetRegion = closestRegion;
        
        // Indica que se está esperando una transición
        waitingForTransition = true;
    }
}



// ------------------- GETTERS - SETTERS -------------------

// ==========================================
//      GETS CURRENT
// ==========================================
// Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
int MovementManager::getCurrentRow() const {
    return currentRow;
}
// Obtiene la siguiente región a la que debe llegar (Siguiente Punto de Salida).
int MovementManager::getNextOutRegion() const {
    return targetRegion;
}
// Obtiene el archvio del movimiento actual
string MovementManager::getCurrentFile() const {
    return currentMovement->spriteSheetName;
}
// Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global.
float MovementManager::getCurrentMovementFrameInterval() const {
    return (currentMovement && currentMovement->frameInterval > 0.0f)
    ? currentMovement->frameInterval
    : movementCrono.getFrameInterval();
}
// Obtiene el movimiento actual
const Movement* MovementManager::getCurrentMovement() const {
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // currentMovement es un std::unique_ptr<Movement>
    // .get() extrae el puntero crudo (raw pointer) del unique_ptr
    // Así devolvemos Movement* en lugar de unique_ptr<Movement>
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    return currentMovement.get();
}
// Obtiene el nombre del movimiento actual
std::string MovementManager::getCurrentMovementName() const {
    return currentMovementName;
}


// ==========================================
//      VARIABLES DE PERSONAJE
// ==========================================
// Obtiene la dirección del personaje
bool MovementManager::getIsFacingRight() {
    return isFacingRight;
}
// Establece la dirección del personaje
void MovementManager::toggleIsFacingRight() {
    isFacingRight = !isFacingRight;
}
// Obtiene la escala del personaje
const float MovementManager::getScaleFactor() const {
    return scaleFactor;
}
// Modifica la escala del personaje
void MovementManager::setScaleFactor(float scaleFactor){
    this->scaleFactor = scaleFactor;
    spriteSheetManager->setCurrentScale(scaleFactor);
    physicsManager->setCurrentScale(scaleFactor);
    collisionManager->setCurrentScale(scaleFactor);
}


// ==========================================
//      GETS - SETS: MOVEMENTS CRONO
// ==========================================
// Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
int MovementManager::getCurrentRegion() const {
    return movementCrono.getCurrentRegion();
}
// Obtiene el intervalo de fotogramas global.
float MovementManager::getFrameInterval() const {
    // Devuelve el intervalo de fotogramas global
    return movementCrono.getFrameInterval();
}
// Establece el intervalo de fotogramas global
void MovementManager::setFrameInterval(float interval) {
    movementCrono.setFrameInterval(interval);
}


// ==========================================
//      GETS - SETS: MOVEMENTS UTILITIES
// ==========================================
// Obtiene el mapa de todos los movimientos disponibles
const std::map<std::string, Movement>& MovementManager:: getMovements() const {
    // movements es un mapa grande (std::map<std::string, Movement>)
    // Copiarlo cada vez sería ineficiente
    // Una referencia const lo deja accesible sin copiarlo
    return movementUtilities.getAllMovements();
}
// Obtiene el intervalo de fotogramas para un movimiento específico
float MovementManager::getMovementFrameInterval(const std::string& movementName) const {
    // Verifica si el movimiento especificado existe en el mapa de movimientos
    if (movementUtilities.getAllMovements().count(movementName)) {
        // Devuelve el intervalo de fotogramas del movimiento específico
        return movementUtilities.getAllMovements().at(movementName).frameInterval;
    }
    // Valor por defecto si el movimiento no se encuentra
    return 0.1f;
}
// Establece el intervalo de fotogramas para un movimiento específico
void MovementManager::setMovementFrameInterval(const std::string& movementName, float interval) {
    if (movementUtilities.getAllMovements().count(movementName)) {
        movementUtilities.getMovement(movementName).frameInterval = interval;
    }
}


// ==========================================
//    GETS STATES ( EXCLUSIVOS PARA GUI )
// ==========================================
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
        case MovementState:: JUMPING:
            return "JUMPING";
        case MovementState:: FALLING:
            return "FALLING";
        default:
            return "UNKNOWN";
    }
}
// Retorna un string con el estado objetivo del personaje
std::string MovementManager::getTargetState() const {
    switch (targetState) {
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
        case MovementState:: JUMPING:
            return "JUMPING";
        case MovementState:: FALLING:
            return "FALLING";
        default:
            return "UNKNOWN";
    }
}
// Retorna un string con el estado anterior del personaje
std::string MovementManager::getPreviousState() const {
    switch (previousState) {
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
        case MovementState:: JUMPING:
            return "JUMPING";
        case MovementState:: FALLING:
            return "FALLING";
        default:
            return "UNKNOWN";
    }
}
// Retorna true si está esperando una transición, false en caso contrario
bool MovementManager::isWaitingForTransition() const {
    return waitingForTransition;
}
