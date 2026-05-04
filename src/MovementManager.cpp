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
    
    /*
    // CÁLCULO DE LA ORIENTACIÓN LÓGICA (OBSOLETO)
    // Dirección Actual (true = derecha, false = izquierda)
    bool lookingRight = getIsFacingRight();
    
    // pulsa IZQUIERDA (intent.wantsLeft) y mira a la DERECHA (lookingRight)
    // pulsa DERECHA (intent.wantsRight) y mira a la IZQUIERDA (!lookingRight)
    bool wantsTurn = (intent.wantsLeft && lookingRight) || (intent.wantsRight && !lookingRight);
    */
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //               TO TURNING
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // SI QUIERE IR A LA DIRECCIÓN CONTRARIA
    // Caso: Quiere ir a la derecha pero está mirando a la izquierda
    if ( (intent.wantsRight && !isFacingRight) || (intent.wantsLeft && isFacingRight) ) {
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
    
    
    
    
    // -------- NUEVO --------
    // !!!!!!!!! JUMP !!!!!!!!!
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //               GESTIÓN DE SALTO ACTIVO
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::JUMPING) {
        
        // Salto cortado: jugador soltó SPACE mientras subía
        if (!intent.wantsJump && physicsManager->getIsImpulsing() && !isGrounded) {
            physicsManager->cutJump();
            currentCommand = MovementCommand::GO_FALL;
            return;
        }
        
        // Salto agotado: se acabaron los frames de subida
        if (!physicsManager->getIsImpulsing() && !isGrounded) {
            currentCommand = MovementCommand::GO_FALL;
            return;
        }
        
        // Sigue subiendo: mantenemos el estado
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO JUMP
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // !!!!!!!!! JUMP !!!!!!!!!
    
    // ESTO SERíA SOLO PARA IDLE WALK o RUN TO_JUMP
    
    /*if (intent.wantsJump && isGrounded) {
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }*/
    
    // ESTO SE APLICA CON isGgrounded TANTO true COMO false
  
    if (intent.wantsJump && isGrounded) {
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }
    
    // Para el futuro doble salto??
    /*if (intent.wantsJump && !isGorunded) {
        currentCommand = MovementCommand::GO_JUMP;
        return;
    }*/
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO FALL
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // No hay GIRO y no hay DIRECCIÓN, pero està en el aire i no se está impulsando
    if (!intent.hasAnyDirection && !isGrounded && !physicsManager->getIsImpulsing()) {
        currentCommand =  MovementCommand::GO_FALL;
        return;
    }
    
    // MÁXIMO SALTO
    // Frames Remaining Jump mayor que 0
   /* if(framesRemainingJump !=-1){
        if(framesRemainingJump>0){
            // Bajamos uno
            framesRemainingJump--;
        }
        else {
            currentCommand =  MovementCommand::GO_FALL;
            framesRemainingJump=-1;
        }
        return;
    }*/
    
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO IDLE
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // No hay GIRO y no hay DIRECCIÓN
    if (!intent.hasAnyDirection) {
        currentCommand =  MovementCommand::GO_STOP;
        return;
    }
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO IDLE
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // Si quiere ir a la derecha pero hay pared...
    if (intent.wantsRight && isWalledRight) {
        currentCommand = MovementCommand::GO_STOP;
        return;
    }
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO IDLE
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // Si quiere ir a la izquierda pero hay pared...
    if (intent.wantsLeft && isWalledLeft) {
        currentCommand = MovementCommand::GO_STOP;
        return;
    }

    
    // HAY DIRECCIÓN
    if (intent.wantsRight || intent.wantsLeft) {
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //              TO RUNNING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Quiere correr --> Quiere avanzar rápido
        if (intent.wantsRun) {
            currentCommand = MovementCommand::GO_FORWARD_FAST;
            return;
            
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //              TO WALKING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Si no, simplemente avanzar
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
            
            
            // !!!!!!!!! JUMP !!!!!!!!!
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
            
        // !!!!!!!!! JUMP !!!!!!!!! (En principio si entra desde aquí solo debería gestionar (IDLE, WALK Y RUN) _TO_JUMP)
        case MovementState::JUMPING: handleJumpingState(targetState, MovementMoment::CHANGE); break;
            
            
        // ESTOS DOS CASE NO HARÍAN FALTA POR EL CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES
        case MovementState::TURNING:  handleTurningState(targetState, MovementMoment::CHANGE); break;
        case MovementState::STOPPING: handleStoppingState(targetState, MovementMoment::CHANGE); break;
        
        //PARA LAND
        case MovementState::FALLING:
            currentState = MovementState::IDLE;
            playMovement("LAND_TO_IDLE");
            break;

            
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
    // Aquí podríamos manejar estados específicos para cuando el personaje está en el aire, como saltando o cayendo.
    //ofLogNotice("MovementManager") << "AIRE en currentMovement: " << currentMovementName;
    
    
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
            
        case MovementState::IDLE:
            //handleIdleState(targetState, MovementMoment::CHANGE);
            
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //                TO FALLING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                if (targetState == MovementState::FALLING) {
                    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                    //                  RUN
                    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                    ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A CAER DESDE: " << currentMovementName << "!!!";
                    // ACTUALIZAMOS ESTADO
                    ofLogNotice("MovementManager") << "!!! ACTUALIZAMOS ESTADO A FALLING " << "!!!";
                    currentState = MovementState::FALLING;
                    
                    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
                    // NO TENEMOS MOVIMIENTO IDLE TO FALL
                    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
                    
                    // EJECUTAMOS TRANSICION A FLEXIBLE
                    ofLogNotice("MovementManager") << "!!! TRANSICION A FLEXIBLE: " << "IDLE" << "!!!";
                    playMovement("FALL");
                }

            
            
            break;
        case MovementState::JUMPING:
            
            // handleJumpingState(targetState, MovementMoment::CHANGE);
            
            if(targetState == MovementState::FALLING){
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //                TO FALLING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                    //                  RUN
                    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                    ofLogNotice("MovementManager") << "!!! ESTA SALTANDO, VAMOS A CAER DESDE: " << currentMovementName << "!!!";
                    // ACTUALIZAMOS ESTADO
                    ofLogNotice("MovementManager") << "!!! ACTUALIZAMOS ESTADO A FALLING " << "!!!";
                    currentState = MovementState::FALLING;
                    
                    // EJECUTAMOS TRANSICION A FLEXIBLE
                    ofLogNotice("MovementManager") << "!!! TRANSICION A FLEXIBLE: " << "JUMP" << "!!!";
                    playMovement("JUMP_TO_FALL");
            }
            
            
            break;
            
        //PARA LAND
        case MovementState::FALLING:
            currentState = MovementState::IDLE;
            playMovement("LAND_TO_IDLE");
            break;
            
        case MovementState::WALKING:
            // handleWalkingState(targetState, MovementMoment::CHANGE);
            break;
            
        case MovementState::RUNNING:
            // handleRunningState(targetState, MovementMoment::CHANGE);
            break;
            
        case MovementState::TURNING:
            // handleTurningState(targetState, MovementMoment::CHANGE);
            break;
        case MovementState::STOPPING:
            // handleStoppingState(targetState, MovementMoment::CHANGE);
            break;
            
            

            
            
            // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
            // Si un case no tiene nada (en este caso UNO) "comparte" el código con el siguiente case que sí lo tenga (en esta caso DOS)
            // case MovementState::UNO:
            // case MovementState:: DOS:
            // lo que haya
            // break;
            // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    }

}

// ==========================================
//     ARREPENTIMIENTOS y REDIRECCIONES
// ==========================================
bool MovementManager::handleWaitingInterrupt() {
    
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
    //         REDIRECCIÓN: TURNING → STOPPING
    // Estaba esperando PS para girar, pero ahora quiere frenar.
    // No cancelamos la espera, la reconfiguramos con stop_transitions.
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if (currentState == MovementState::TURNING && targetState == MovementState::IDLE) {
        currentState = MovementState::STOPPING;
        handleTransition(); // Reconfigura buscando PS en stop_transitions
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
        case MovementState::IDLE: handleIdleState(targetState, MovementMoment::TRIGGER); break;
            
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
            

            
            
        case MovementState::JUMPING:
        break;
        case MovementState::FALLING:
        break;
    }
}
// !!!!!!! TRANSICIONES DE AIRE !!!!!!! //
void MovementManager::triggerAirTransition() {
    ofLogNotice("MovementManager") << "TRANSICION AIRE";
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
         origin == TransitionOrigin::RUN_TURN) {
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

            // !!!!!!!!! JUMP !!!!!!!!
            // ######################################################
            //   TO_JUMP
            // ######################################################
            /*case TransitionOrigin::TO_JUMP:
                currentState = MovementState::JUMPING;
                playMovement("JUMP");
                break;*/
            
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // ESTO ESTA AQUÍ POR SI SE SALTA MUY POCO
            // SE PODRÍA PONER UN SALTO MÍNIMO siempre que saltes
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            // (JUMP_TO_FALL)
            case TransitionOrigin::TO_FALL:
                currentState = MovementState::FALLING;
                playMovement("FALL");
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
            /*case TransitionOrigin::TO_JUMP:
                //currentState = MovementState::JUMPING;
                //playMovement("JUMP");
            
                // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
                // DOBLE SALTO? seria en un case de JUMP?
                // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

                break;*/
            
            case TransitionOrigin::TO_JUMP:
                currentState = MovementState::JUMPING;
                playMovement("JUMP");
            break;
            
            case TransitionOrigin::TO_FALL:
                currentState = MovementState::FALLING;
                playMovement("FALL");
                break;
            
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
                //ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A EMPEZAR A CAMINAR DESDE: " << currentMovementName << "!!!";
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
                //ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A EMPEZAR A CORRER DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                playMovement("IDLE_TO_RUN");
            }
            
            // !!!!!!!!! JUMP !!!!!!!!
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA IDLE, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                ofLogNotice("MovementManager") << "!!! ACTUALIZAMOS ESTADO A JUMPING " << "!!!";
                currentState = MovementState::JUMPING;
                // EJECUTAMOS TRANSICION A FLEXIBLE
                ofLogNotice("MovementManager") << "!!! TRANSICION A FLEXIBLE: " << "IDLE TO JUMP" << "!!!";
                playMovement("IDLE_TO_JUMP");
            }
        break;
            
            
            
            
        
        // ========================================================================
        //                           TRIGGER TRANSITION
        // ========================================================================
        case MovementMoment::TRIGGER:
            // NO HACE NADA
            ofLogNotice("MovementManager") << "!!! NO HACE NADA" << "!!!";

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
                //ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A FRENAR DESDE: " << currentMovementName << "!!!";
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
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA WALKING, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                ofLogNotice("MovementManager") << "!!! ACTUALIZAMOS ESTADO A JUMPING " << "!!!";
                //currentState = MovementState::JUMPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                ofLogNotice("MovementManager") << "!!! BUSCAR PS Y ACTIVAR ESPERA PARA: " << "WALK_TO_JUMP" << "!!!";
                //handleTransition();
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
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO JUMPING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::JUMPING) {
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                //                  RUN
                // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
                ofLogNotice("MovementManager") << "!!! ESTA RUNNING, VAMOS A SALTAR DESDE: " << currentMovementName << "!!!";
                // ACTUALIZAMOS ESTADO
                ofLogNotice("MovementManager") << "!!! ACTUALIZAMOS ESTADO A JUMPING " << "!!!";
                //currentState = MovementState::JUMPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateFrame())
                ofLogNotice("MovementManager") << "!!! BUSCAR PS Y ACTIVAR ESPERA PARA: " << "RUN_TO_JUMP" << "!!!";
                // handleTransition();
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
            
    
            break;
    }
    
    
    // IF FINISHED (esto no sé, porque va por wants y no por estados...)
    
    /* FINISHED TRANSITION */
    /*
    ???????
    */

}

// ##############################################################################################################################
//                                                     JUMPING
// ##############################################################################################################################
void MovementManager::handleJumpingState(MovementState targetState, MovementMoment moment) {
    
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
    // Suma un pequeño incremento a la velocidad actual
    physicsManager->updateVelocityStep();
    
    
    // -------- NUEVO --------
    // !!!!!!!!! JUMP !!!!!!!!!
    // Gestiona el delay, el impulso y el conteo de frames de subida
    // Podríamos enviar el want Jump por si hay que recalcular a salto corto.
    physicsManager->updateJumpStep();
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
            // IDLE no aplica físicas, existe por consistencia de código
            // ofLogNotice("MovementManager") << "Disparado Movimiento BASE IDLE ";
        break;
        // ==========================================
        //               STOPPING
        // ==========================================
        case MovementState::STOPPING:
            // ofLogNotice("MovementManager") << "-> Detectado FRENADO (TO_IDLE)";
            
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
                 // ofLogNotice("MovementManager") << "-> Detectado GIRO PARADO ";
             }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           IDLE TURN TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             else if (name == "IDLE_TURN_TO_RUN") {
                 // ofLogNotice("MovementManager") << "-> Detectado IDLE_TURN_TO_RUN ";

                 // isFacingRight ya está cambiado pero esta animación en concreto debe interpretar que aun no lo está
                 // al terminar y pasarle el testigo a otra animación ya estará bien.
                 physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames, !isFacingRight, 3);
             }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //           WALK TURN / RUN TURN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
             else{
                  // Inicia la rampa en 'V' para invertir la velocidad en el tiempo de giro
                  physicsManager->startVelocityTurnChange(frames);
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
                // ofLogNotice("MovementManager") << "-> Detectado RUN_TO_WALK";
                // Baja la velocidad hasta el tope de caminar usando los frames de transición más 3 frames "prestados" del ciclo WALK
                physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames + 3, isFacingRight);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //          WALK / IDLE TO WALK
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else {
                ofLogNotice("MovementManager") << "-> Detectado WALK o IDLE_TO_WALK";
                // Mantiene (WALK) o ajusta la velocidad al máximo de caminar durante el primer ciclo de IDLE_TO_WALK
                physicsManager->startVelocityChange( physicsManager->getMaxSpeedWalk(), frames, isFacingRight );
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
                // ofLogNotice("MovementManager") << "-> Detectado WALK_TO_RUN";
                // Acelera hasta correr usando la transición + 3 frames "prestados" del ciclo RUN
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames + 3, isFacingRight);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //               IDLE TO RUN
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            // IDLE TO RUN no debe llegar a su velocidad máxima durante los frames del movimiento, usamos un delay
            else if(name == "IDLE_TO_RUN"){
                // ofLogNotice("MovementManager") << "-> Detectado IDLE_TO_RUN ";
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedWalk(), frames, isFacingRight, 2);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //                  RUN / IDLE TO WALK
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else {
                // ofLogNotice("MovementManager") << "-> Detectado RUN o IDLTE_TO_RUN";
                physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames, isFacingRight);
            }
        break;
            
            
            
        // -------- NUEVO --------
        // !!!!!!!!! JUMP !!!!!!!!!
        // ==========================================
        //                  JMPING
        // ==========================================
        case MovementState::JUMPING:
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //               IDLE TO JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            if(name == "IDLE_TO_JUMP"){
                ofLogNotice("MovementManager") << "-> Detectado IDLE_TO_JUMP ";
                
                // totalJumpFrames: frames de IDLE_TO_JUMP (dos últimos) (7-5), 3 de JUMP y 2 de JUMP_TO_FALL
                physicsManager->startVelocityJump(7, 5, 2);
            }
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //              JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            if(name == "JUMP"){
                /*
                He dejado de apretar SPACE?
                framesRemainingJump = 1;
                or
                Sigo apretando SPACE?

                framesRemainingJump = 5;*/
                
                ofLogNotice("MovementManager") << "-> Detectado JUMP ";
                /*
                He dejado de apretar SPACE?
                framesRemainingJump = 1;
                or
                Sigo apretando SPACE?

                framesRemainingJump = 5;*/
                
           
                
                
                // JUMP no llama a nada — el impulso ya está corriendo
                // desde startVelocityJump, solo continuamos contando
                ofLogNotice("MovementManager") << "-> JUMP en curso, framesRemainingJump="
                                               << physicsManager->getFramesRemainingJump();

            }
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //          WALK TO JUMP (1 i 2)
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else if (name.find("WALK_TO_JUMP") != std::string::npos) {
                // ofLogNotice("MovementManager") << "-> Detectado WALK_TO_JUMP";
                // Fisicas
            }
            
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            //                  RUN TO JUMP
            // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
            else if (name.find("RUN_TO_JUMP") != std::string::npos) {
                // ofLogNotice("MovementManager") << "-> Detectado RUN_TO_JUMP";
                // Fisicas
            }
        break;
            
        // ==========================================
        //                  FALLING
        // ==========================================
        case MovementState::FALLING:
            if(name == "JUMP_TO_FALL"){
                ofLogNotice("MovementManager") << "-> Detectado JUMP_TO_FALL ";
                /*physicsManager->resetGravityY();
                physicsManager->setVelocityY(-10);
                physicsManager->startVelocityChangeY(0, 2);*/
                physicsManager->startJumpToFall(2);
            }
            
            else if(name == "FALL"){
                ofLogNotice("MovementManager") << "-> Detectado FALL ";
            }
            else if (name == "LAND_TO_IDLE"){
                ofLogNotice("MovementManager") << "-> Detectado LAND_TO_IDLE ";
                physicsManager->resetJumpState();
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
        // ESTO DEBERÍA IR AQUÍ, o en algun lugar dentro del IF, pero habría que investigar un tema sobre la sincronica de MovementCrono y PhysicsManager.
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
        ofLogNotice("MovementManager")  << "GO FISICAS de: " << currentMovementName;
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
// Retorna true si está esperando una transición, false en caso contrario
bool MovementManager::isWaitingForTransition() const {
    return waitingForTransition;
}
