#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"

// ##########################################
//             ESTADO ACTUAL
// ##########################################

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//             ESTADO OJETIVO
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ==========================================
//              MOVIMIENTO
// ==========================================


// *** CONFIGURACIÓN Y CONEXIÓN ***
// Inicializa el MovementManager cargando los movimientos desde un archivo JSON y configurando el movimiento actual a "IDLE".
void MovementManager::setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input, PhysicsManager& physics, CollisionManager& collision) {
    
    // Guardamos las referencias para usarlas luego
    this->inputManager = &input;
    this->physicsManager = &physics;
    this->spriteSheetManager = &spriteSheetManager;
    this->collisionManager = &collision;
    
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
    // Reseteamos el flag TURN_TO_RUN
    flag_turn_to_run = false;
    
    // 1. Obtenemos los "deseos" del jugador desde el InputManager
    InputState intent = inputManager->getInputState();
    
    
    // 2. CÁLCULO DE LA ORIENTACIÓN LÓGICA
    // Dirección Actual (true = derecha, false = izquierda)
    bool lookingRight = getIsFacingRight();
    
    // pulsa IZQUIERDA (intent.wantsLeft) y mira a la DERECHA (lookingRight)
    // pulsa DERECHA (intent.wantsRight) y mira a la IZQUIERDA (!lookingRight)
    bool wantsTurn = (intent.wantsLeft && lookingRight) || (intent.wantsRight && !lookingRight);

    
    // 3. TRADUCCIÓN DE INTENCIÓN A ESTADO

    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //               TO TURNING
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //Si quiere girar
    if (wantsTurn) {
        targetState = MovementState::TURNING;
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //           flag TURN TO RUN
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //Si el giro desde IDLE se da con 1 presionado
        if (intent.wantsRun) {
            flag_turn_to_run = true;
        }
    }
    // Si hay alguna direccion activa
    else if (intent.hasAnyDirection) {
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //              TO RUNNING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Si quiere correr
        if (intent.wantsRun) {
            targetState = MovementState::RUNNING;
            
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //              TO WALKING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Si no, quiere caminar
        } else {
            targetState = MovementState::WALKING;
        }
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //                 TO IDLE
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //No hay GIRO y no hay DIRECCIÓN
    else {
        targetState = MovementState::IDLE;
    }
    
    // 4. LLAMADA AL JUEZ
    // Le pasamos nuestra intención. Él decidirá si se puede cumplir y cómo.
    updateState(targetState);
}


// !!!!!!! JUEZ !!!!!!! //
// Switch de estados y llama a playMovement o handleTransition según convenga.
void MovementManager::updateState(MovementState targetState) {
    
    //Bloqueo si Ya estamos en el estado deseado
    if (currentState == targetState) return;
    
    
    // ##############################################################################################################################
    //                              CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES (ARREPENTIMIENTOS)
    // ##############################################################################################################################
    
    //Si estamos esperando llegar a un Punto de Salida
    if(waitingForTransition){
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //         ARREP. --> WALK TO RUN
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        if ( currentState == MovementState::RUNNING && targetState == MovementState::WALKING) {
            // Desactivamos la espera de transición
            waitingForTransition = false;
            // REPONEMOS ESTADO
            currentState = MovementState::WALKING;
            return;
        }
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //         ARREP. --> RUN TO WALK
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        if ( currentState == MovementState::WALKING && targetState == MovementState::RUNNING) {
            // Desactivamos la espera de transición
            waitingForTransition = false;
            // REPONEMOS ESTADO
            currentState = MovementState::RUNNING;
            return;
        }
        
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        //         ARREP. --> STOPPING TO TURNING
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // Si estamos esperando o ejecutando una transición comprometida, COMPROVAMOS ARREPENTIMIENTOS EN LAS ESPERAS DE TRANSICION
        if (currentState == MovementState::TURNING || currentState == MovementState::STOPPING) {
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            // Se ha marcado STOPPING por un insante de teclas nulas, pero mientras esperamos se pide TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (currentState == MovementState::STOPPING && targetState == MovementState::TURNING){
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition(); // buscará el WALK_TURN correcto según puntos de salida cercanos
            }
            
            // Cualquier otro caso de bloqueo,
            return;
        }
        
        // Solo ha entrado por Waiting For Transition
        return;
    }


    // MÁQUINA DE ESTADOS (Lógica de Transición)
    switch (currentState) {
        
// ##############################################################################################################################
//                                                          IDLE
// ##############################################################################################################################
        case MovementState::IDLE:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (targetState == MovementState::TURNING) {
                
                // ==========================================
                //                  TURN
                // ==========================================
                // Si no quiere correr desde IDLE
                if(!flag_turn_to_run){
                    //std::cout << "!!! ESTA IDLE, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                    // EJECUTAMOS GIRO (COMPROMETIDO)
                    playMovement("TURN");
                    // ACTUALIZAMOS ESTADO
                    currentState = MovementState::TURNING;
                }
                
                // ==========================================
                //              TURN TO RUN
                // ==========================================
                // Si quiere correr desde IDLE
                else {
                    // EJECUTAMOS GIRO (COMPROMETIDO)
                    playMovement("TURN_TO_RUN");
                    // ACTUALIZAMOS ESTADO
                    currentState = MovementState::TURNING;
                }
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO WALKING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::WALKING) {
                // ==========================================
                //                  WALK
                // ==========================================
                //std::cout << "!!! ESTA IDLE, VAMOS A CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // EJECUTAMOS FELXIBLE
                playMovement("WALK");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //                TO RUNNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::RUNNING) {
                // ==========================================
                //                  RUN
                // ==========================================
                //std::cout << "!!! ESTA IDLE, VAMOS A CORRER DESDE: "  << currentMovementName << "!!!" << std::endl;
                // EJECUTAMOS FELXIBLE
                playMovement("RUN");
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
            }
        break;
        
// ##############################################################################################################################
//                                                       WALKING
// ##############################################################################################################################
        case MovementState::WALKING:
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //              TO IDLE
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            if (targetState == MovementState::IDLE) {
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //               STOPPING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //std::cout << "!!! ESTA WALKING, VAMOS A FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::TURNING) {
                // ==========================================
                //             WALK TO TURN
                // ==========================================
                //std::cout << "!!! ESTA WALKING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition();
            }

            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO RUNNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::RUNNING) {
                // ==========================================
                //              WALK TO RUN
                // ==========================================
                //std::cout << "!!! ESTA WALKING, VAMOS A CORRER DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::RUNNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition();
            }
        break;

            
// ##############################################################################################################################
//                                                       RUNNING
// ##############################################################################################################################
        case MovementState::RUNNING:
            if (targetState == MovementState::IDLE) {
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //               STOPPING
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                //std::cout << "!!! ESTA RUNNING, VAMOS A FRENAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::STOPPING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition();
            }
           
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO TURNING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::TURNING) {
                // ==========================================
                //             RUN TO TURN
                // ==========================================
                //std::cout << "!!! ESTA RUNNING, VAMOS A GIRAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                currentState = MovementState::TURNING;
                // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                handleTransition();
            }
            
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            //               TO WALKING
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            else if (targetState == MovementState::WALKING) {
                //std::cout << "!!! ESTA RUNNING, VAMOS A CAMINAR DESDE: "  << currentMovementName << "!!!" << std::endl;
                // ==========================================
                //             RUN TO WALK
                // ==========================================
                 // ACTUALIZAMOS ESTADO primero por ser TRANSICION
                 currentState = MovementState::WALKING;
                 // BUSCAR PUNTO DE SALIDA y ACTIVAR ESPERA (lo ejecutará updateRegion())
                 handleTransition();
            }
            
        break;

            
        // ESTOS DOS CASE NO HARÍAN FALTA POR EL CONTROL DE ESTADOS COMPROMETIDOS Y TRANSICIONES
        case MovementState::TURNING:
            //std::cout << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!" << std::endl;
            break;
        case MovementState::STOPPING:
            //std::cout << "!!! NO HACEMOS NADA POR ESPERA DE: "  << currentMovementName << "!!!" << std::endl;
            break;
    }
}





// ##############################################################################################################################
//                                                DISPARAR TRANSICION
// ##############################################################################################################################
void MovementManager::triggerTransition() {
    
    switch (currentState) {
        // ##########################################
        //              TURNING TRANS
        // ##########################################
        case MovementState::TURNING:
            // EJECUTAMOS
            playMovement(currentMovement->turn_transitions[targetRegion]);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::TURNING;
        break;

        // ##########################################
        //           RUNNING / WALKING   TRANS
        // ##########################################
        //Si Estabamos esperando transicion y el currentState era RUNNING: solo puede ser WALK_TO_RUN (por ejemplo para frenar se ha puesto STOPPING)
        //Si Estabamos esperando transicion y el currentState era WALKING: solo puede ser RUN_TO_WALK
        case MovementState::RUNNING:
        case MovementState::WALKING:
            // ==========================================
            //            WALK TO RUN / RUN TO WALK
            // ==========================================
            // EJECUTAMOS
            playMovement(currentMovement->change_transitions[targetRegion]);
            // ACTUALIZAMOS ESTADO
            
            if (currentMovementName == "WALK_TO_RUN") {
                currentState = MovementState::RUNNING; // Lo marcamos como RUNNING (aunque ya se ha marcado en updateState, por ser transición)
            }
            else if (currentMovementName == "RUN_TO_WALK") {
                currentState = MovementState::WALKING; // Lo marcamos como WALKING (aunque ya se ha marcado en updateState, por ser transición)
            }
        break;
        
        // ##########################################
        //               IDLE TRANS
        // ##########################################
        // seria case MovementState::STOPPING:
        default:
            // EJECUTAMOS
            playMovement(currentMovement->transitions[targetRegion]);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::STOPPING;
        break;
    }
    
}

// ##############################################################################################################################
//                                               TRANSICION COMPLETADA
// ##############################################################################################################################
void MovementManager::finishedTransition() {
    // ==========================================
    //              ANY "TURN"
    // ==========================================
    //Era un giro, cambiamos la dirección lógica
    if (currentMovementName.find("TURN") != std::string::npos) {
        //std::cout << "!!! ERA UN GIRO : !!!" << std::endl;
        toggleIsFacingRight(); // Cambia la dirección lógica
    }

    // ¿QUÉ HACEMOS AHORA?
    InputState intent = inputManager->getInputState();

    // ##############################################################################################################################
    //                                                        SIGUE
    // ##############################################################################################################################
    if (intent.hasAnyDirection)  {
        // ######################################################
        //     FIN --> WALK_TO_IDLE / RUN_TO_IDLE
        // ######################################################
        if (currentMovementName.find("TO_IDLE") != std::string::npos) {
            playMovement("IDLE");
            currentState = MovementState::IDLE;
        }
        // Si NO era un TO_IDLE
        else {
            
            // ####################################################################################################
            //     FIN --> RUN TURN / WALK TO RUN
            // ####################################################################################################
            // Si pulsa correr
            if (intent.wantsRun) {
                // Algunas transiciones necesitan empezar en un frame concreto
                int startFrame = 0;
                if(currentMovementName == "WALK_TO_RUN_1") startFrame = 10;
                else if (currentMovementName == "WALK_TO_RUN_2") startFrame = 4;
                
                // ==========================================
                //                   RUN
                // ==========================================
                // EJECUTAMOS FELXIBLE
                playMovement("RUN", startFrame);
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::RUNNING;
                
                // ####################################################################################################
                //     FIN --> WALK TURN / RUN TO WALK
                // ####################################################################################################
                // Si no pulsa correr
            } else {
                int startFrame = 0;
                if(currentMovementName == "RUN_TO_WALK_1") startFrame = 4;
                
                // ==========================================
                //                   WALK
                // ==========================================
                // EJECUTAMOS FELXIBLE
                playMovement("WALK", startFrame);
                // ACTUALIZAMOS ESTADO
                currentState = MovementState::WALKING;
            }
        }
    }
    
    // ##############################################################################################################################
    //                                                PARA (PASO ANTES DE FRENADA)
    // ##############################################################################################################################
    else {
        
        // ######################################################
        //      FIN --> WALK TURN / RUN TO WALK
        // ######################################################
        if(currentMovementName.find("WALK_TURN") != std::string::npos || currentMovementName.find("TO_WALK") != std::string::npos){
            //std::cout << ">>> Giro en movimiento WALK o RUN_TO_WALK terminado , pero no hay wants: Paso de transición antes de frenar." << std::endl;
            
            // Algunas transiciones necesitan empezar en un frame concreto
            int startFrame = 0;
            if(currentMovementName == "RUN_TO_WALK_1") startFrame = 4;
            
            // ==========================================
            //                   WALK
            // ==========================================
            // EJECUTAMOS FELXIBLE
            playMovement("WALK", startFrame);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::WALKING;
        }

        // ######################################################
        //     FIN --> RUN TURN / WALK TO RUN / TURN TO RUN
        // ######################################################
        else if (currentMovementName.find("RUN_TURN") != std::string::npos || currentMovementName.find("TO_RUN") != std::string::npos ){
            //std::cout << ">>> Giro en movimiento RUN o WALK_TO_RUN terminado , pero no hay wants: Paso de transición antes de frenar." << std::endl;

            // Algunas transiciones necesitan empezar en un frame concreto
            int startFrame = 0;
            if(currentMovementName == "WALK_TO_RUN_1") startFrame = 10;
            else if (currentMovementName == "WALK_TO_RUN_2") startFrame = 4;
            
            // ==========================================
            //                   RUN
            // ==========================================
            // EJECUTAMOS FELXIBLE
            playMovement("RUN", startFrame);
            // ACTUALIZAMOS ESTADO
            currentState = MovementState::RUNNING;
        }
        
        // ######################################################
        //     FIN --> WALK TO IDLE / RUN TO IDLE
        // ######################################################
        else {
            // ... si no, vuelve a IDLE
            playMovement("IDLE");
            currentState = MovementState::IDLE;
        }
    }
    updateIntent();
}






// *** MOTOR *** //
// Segun el .h iría más abajo porque es private, pero la dejamos aquí junto a la otra única función que ejecuta playMovement y actualiza el estado.
// Maneja la lógica de actualización de frames y transiciones
void MovementManager::updateRegion() {
    // Incrementa la región actual
    currentRegion++;
    
    
// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Suma un pequeño incremento a la velocidad actual
    physicsManager->updateVelocityStep();
    
    // 1. Obtenemos la velocidad que la física QUIERE aplicar (ej: 9.8)
    float currentVel = physicsManager->getGravityY();
    
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // CHECK COLLISIONS futuro
    Interactor* hitObject = collisionManager->checkCollisions(
        physicsManager->getPosition(),
        physicsManager->getVelocity(),
        physicsManager->getGravityY(),
        *spriteSheetManager,
        getIsFacingRight()
    );

    if (hitObject != nullptr) {
        // Accedemos a los datos del interactor chocado
        cout << "COLISION CON: " << hitObject->name << " (Tipo: " << (int)hitObject->type << ")" << endl;
        
        if (hitObject->type == InteractorType::SURFACE) {
            isGrounded = true;
        }
            
        
    } else {
        //cout << "NADA COLISIONA - Camino libre" << endl;
    }
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    
    if(!isGrounded){
        // Si no estamos en el suelo, aplicamos gravedad
        physicsManager->applyGravity();
    } else {
        // 700 (Suelo)
            // + 150 (Subimos al borde superior de la region)
            // - 10  (Bajamos el offset Y)
            // - 222 (Subimos la altura de la hitbox)
            
            float yPerfecta = 700 + 150 - 10 - 222;
        
        physicsManager->setPositionY(yPerfecta); // Asegura que el personaje se mantenga en el suelo
    }
    
    
    
// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Aplica velocidad
    physicsManager->applyVelocity();
    
    
    // ##############################################################################################################################
    //                                                DISPARAR TRANSICION
    // ##############################################################################################################################
    // Si estamos esperando una transición y hemos llegado al Punto de Salida,
    if (waitingForTransition && currentRegion == targetRegion) {
        // Reset Waiting
        waitingForTransition = false;
        
        //std::cout << "!!! DISPARAR TRANSICION : !!!" << std::endl;
        triggerTransition();
        return;
    }

    // ##############################################################################################################################
    //                                               TRANSICION COMPLETADA
    // ##############################################################################################################################
    // Si el movimiento actual es una transición y se ha completado,
    if (currentMovement->isTransition && currentRegion >= currentMovement->numRegions) {
        //std::cout << "!!! TRANSICION COMPLETADA : !!!" << std::endl;
        finishedTransition();
        return;
    }
    
    // ##############################################################################################################################
    //                                               MOVIMIENTO BUCLE
    // ##############################################################################################################################
    // Si la región actual alcanza el último frame,
    if (currentRegion >= currentMovement->numRegions) {
       // Vuelve al primer frame (para movimientos en bucle)
       currentRegion = 0;
    }
}

// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
void MovementManager::handleMovementPhysics(const std::string& name) {
    // Si el nombre del movimiento no existe en nuestro mapa, abortamos
    if (movements.find(name) == movements.end()) return;
    
    // Obtenemos el número de frames (regiones) que dura esta animación
    int frames = movements[name].numRegions;

    //std::cout << "Analizando: " << name << std::endl;

// ##############################################################################################################################
//                                           TRANSICIONES (PRIORIDAD MÁXIMA)
// ##############################################################################################################################
    // ==========================================
    //                  TURN
    // ==========================================
    if (name.find("TURN_") != std::string::npos) {
        //std::cout << "-> Detectado GIRO en Movimiento" << std::endl;
        // Inicia la rampa en 'V' para invertir la velocidad en el tiempo de giro
        physicsManager->startVelocityTurnChange(frames);
        return;
    }
    
    // ==========================================
    //                 TO_IDLE
    // ==========================================
    if (name.find("TO_IDLE") != std::string::npos) {
        //std::cout << "-> Detectado FRENADO (TO_IDLE)" << std::endl;
        // Crea una rampa hacia velocidad 0 usando los frames de la animación de frenado
        physicsManager->startVelocityChange(0, frames, isFacingRight);
        return;
    }

    // ==========================================
    //                 RUN_TO_WALK
    // ==========================================
    if (name.find("RUN_TO_WALK") != std::string::npos) {
        //std::cout << "-> Detectado RUN_TO_WALK" << std::endl;
        // Baja la velocidad hasta el tope de caminar usando los frames de transición
        physicsManager->startVelocityChange(physicsManager->getMaxSpeedWalk(), frames, isFacingRight);
        return;
    }

    // ==========================================
    //                  WALK_TO_RUN
    // ==========================================
    if (name.find("WALK_TO_RUN") != std::string::npos) {
        //std::cout << "-> Detectado WALK_TO_RUN" << std::endl;
        // Acelera hasta correr usando la transición + 3 frames "prestados" del ciclo RUN
        physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames + 3, isFacingRight);
        return;
    }

// ##############################################################################################################################
//                                           BUCLES (PRIORIDAD BAJA)
// ##############################################################################################################################
    // ==========================================
    //                   WALK
    // ==========================================
    if (name.find("WALK") != std::string::npos) {
        //std::cout << "-> Detectado WALK simple" << std::endl;
        // Mantiene o ajusta la velocidad al máximo de caminar, durante el primer ciclo de WALK
        physicsManager->startVelocityChange(physicsManager->getMaxSpeedWalk(), frames, isFacingRight);
    }
    // ==========================================
    //                    RUN
    // ==========================================
    else if (name.find("RUN") != std::string::npos) {
        //std::cout << "-> Detectado RUN simple" << std::endl;
        // Mantiene o ajusta la velocidad al máximo de correr, durante el primer ciclo de RUN
        physicsManager->startVelocityChange(physicsManager->getMaxSpeedRun(), frames, isFacingRight);
    }
    // ==========================================
    //            IDLE, por ejemplo
    // ==========================================
    else {
        //std::cout << name << " No tiene fisicas" << std::endl;
    }
}



// ***  EJECUTOR *** //
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
        
        // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
        //std::cout << "GO FISICAS de: " << currentMovementName << std::endl;
        handleMovementPhysics(currentMovementName);
    }
}

// ***  OTEADOR *** //
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
        
        // Carga las regiones de transición del movimiento
        for (const auto& region : jsonMovement["transitionRegions"]) {
            movement.transitionRegions.push_back(region.asInt());
        }

        // Carga las transiciones del movimiento
        for (const auto& transition : jsonMovement["transitions"].getMemberNames()) {
            movement.transitions[std::stoi(transition)] = jsonMovement["transitions"][transition].asString();
        }
        
        // Carga las transiciones de giro del movimiento
        for (const auto& transition : jsonMovement["turn_transitions"].getMemberNames()) {
            movement.turn_transitions[std::stoi(transition)] = jsonMovement["turn_transitions"][transition].asString();
        }
        
        // Carga las transiciones de cambio del movimiento
        for (const auto& transition : jsonMovement["change_transitions"].getMemberNames()) {
            movement.change_transitions[std::stoi(transition)] = jsonMovement["change_transitions"][transition].asString();
        }
        
        // Añade el movimiento al mapa de movimientos
        movements[movement.name] = movement;
    }
}


