#include "PhysicsManager.h"

// *** CONSTRUCTOR ***
PhysicsManager::PhysicsManager()
    :
        currentScale(1.0f),
        targetVelocityX(0.0f),
        velocityStepX(0.0f),
        framesRemaining(0),
        delayFramesRemaining(0),
        isVelocityChanging(false),

        baseMaxSpeedRun(0.0f),
        baseMaxSpeedWalk(0.0f),
        maxSpeedRun(0.0f),
        maxSpeedWalk(0.0f),

        baseMaxSpeedAir(0.0f),
        maxSpeedAir(0.0f),
        airForwardFrames(0),
        airUnforwardFrames(0),

        jumpImpulse(0.0f),
        stopStepY(0.0f),
        stopFrames(0),
        minJumpFrames(0),
        maxJumpFrames(0),

        framesRemainingJump(0),
        impulseFrames(0),
        framesRemainingJumpStop(0),
        delayFramesRemainingJump(0),

        isWaitingJumpImpulse(false),
        isImpulsing(false),
        isMinJump(false),
        isJumpCutted(false),
        isStartingToFall(false),
        gravityOverride(false),

        isHanging(false)
{
    // Inicializamos los vectores a cero (aquí por el uso de set)
    position.set(0, 0);
    velocity.set(0, 0);
    baseGravity.set(0, 0);
    gravity.set(0, 0);
    
    // Nota: Aunque setup() les dará los valores reales (7.0, 18.0, etc.),
    // el constructor garantiza que si alguien intenta leer la velocidad
    // antes de tiempo, recibirá un 0 y no un número aleatorio gigante.
}

// *** DESTRUCTOR ***
PhysicsManager::~PhysicsManager() {
    // En esta clase no hay memoria dinámica (new) ni punteros externos.
    // La limpieza es automática para los objetos ofVec2f y floats.
    
    //ofLogNotice("PhysicsManager") << "Destructor ejecutado: Recursos liberados.";
}


// *** SETUP ***
void PhysicsManager::setup(float startX, float startY) {
    // Posicion Inicial
    position.set(startX, startY);
    // Velocidad Inicial
    velocity.set(0, 0);
    
    // Gravedad
    baseGravity.set(0, 5.8f); // Valor de gravedad
    // Valores iniciales por defecto (se pueden cambiar con los setters)
    baseMaxSpeedWalk = 7.0f;
    baseMaxSpeedRun = 18.0f;
    baseMaxSpeedAir = 4.0f;
    
    // Escala Inicial
    currentScale = 1.0f;
    
    // Inicialización de control
    targetVelocityX = 0.0f;
    velocityStepX = 0.0f;
    framesRemaining = 0;
    isVelocityChanging = false;
    
    // *** VALORES BASE FRAMES *** (modificables desde GUI)
    // FRAMES DE PARADA GRADUAL
    stopFrames = 2;
    // SALTO MÍNIMO
    minJumpFrames = 3;
    // SALTO MÁXIMO
    maxJumpFrames = 6;
    // FRAMES DE AVANCE EN EL AIRE
    airForwardFrames = 4;
    // FRAMES DE CESE DE AVANCE EN EL AIRE
    airUnforwardFrames = 4;
    
    
    // Está bien resetear las variables
    resetJumpVariables();
    
    // Debemos dar valor a las variables de trabajo con la escala inicial
    updateScaledPhysics();
}


void PhysicsManager::update() {
    // Aquí irían fuerzas continuas como la gravedad si el títere saltara.
    // De momento, la velocidad horizontal la controla el "Latido".
}


// *** EL LATIDO en X ***
// Aplica el incremento/decremento calculado en cada paso de animación (Frame Data) y gestiona la finalización de la rampa.
void PhysicsManager::updateVelocityStep() {
    
    if (!isVelocityChanging) return;

    // Algunos movimientos tienen un delay
    if(delayFramesRemaining > 0) {
        delayFramesRemaining--; // Gastamos un frame de espera
        framesRemaining--;      // El tiempo total del movimiento también corre
    }
    // Si hay una transición activa y frames pendientes
    else if (framesRemaining > 0) {
        
        // APLICAMOS el paso de velocidad (aceleración o frenado)
        velocity.x += velocityStepX;
        
        // Actualizamos el contador de frames para la física actual
        framesRemaining--;

        // Si hemos llegado al final de la transición
        if (framesRemaining <= 0) {
            // Para evitar floats raros (2.99998 en vez de 3.0), usamos la función cleanFloat para redondear a un decimal limpio.
            velocity.x = cleanFloat(velocity.x);
            
            // Limpieza de estado: detenemos el proceso y reseteamos el diferencial
            isVelocityChanging = false;
            velocityStepX = 0.0f;
        }
    }
}

// *** CAMBIOS DE VELOCIDAD en X ***
// Configura una rampa de aceleración o frenado (cambio de velocidad progresivo).
void PhysicsManager::startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight, int delay) {
    
    // Determinar el vector de velocidad objetivo según la orientación del personaje
    float targetSpeedWithDirection = lookingRight ? targetAbsSpeed : -targetAbsSpeed;

    // CASO INSTANTÁNEO: Si no hay frames de transición y no hay delay, aplicamos la velocidad de golpe
    // Para no dividir entre 0
    if (frames <= 0 && delay <= 0) {
        velocity.x = targetSpeedWithDirection;
        targetVelocityX = targetSpeedWithDirection;
        isVelocityChanging = false;
        framesRemaining = 0;
        return; // Salida temprana para evitar cálculos de rampa
    }

    // CONFIGURACIÓN DE LA RAMPA:
    // Actualizamos si tiene delay
    delayFramesRemaining = delay;
    // Guardamos la meta final y el tiempo (frames, incluido el delay) que tenemos para llegar
    targetVelocityX = targetSpeedWithDirection;
    framesRemaining = frames;
    isVelocityChanging = true;

    
    // CÁLCULO DEL DIFERENCIAL (Step):
    // Determinamos cuánto debe cambiar la velocidad en cada frame de animación.
    // (Esto funciona tanto para acelerar como para frenar.
    velocityStepX = (targetVelocityX - velocity.x) / (float)framesRemaining;
}

// Configura una rampa de cambio de dirección (giro), que es un caso especial donde la velocidad final es la misma pero en sentido opuesto.
void PhysicsManager::startVelocityTurnChange(int frames) {
    // Seguridad: si no hay tiempo de animación, cancelamos el proceso
    if (frames <= 0) return;

    // Registramos cuántos frames de animación durará el giro completo
    framesRemaining = frames;
    // Activamos la bandera para que el motor empiece a aplicar la rampa
    isVelocityChanging = true;
    
    // El objetivo final es la misma velocidad actual pero en dirección opuesta
    targetVelocityX = -velocity.x;

    // Calculamos el incremento necesario por cada frame para completar la "V"
    // (Velocidad Final - Velocidad Inicial) / Total de pasos
    velocityStepX = (targetVelocityX - velocity.x) / (float)framesRemaining;
}



// *** EL LATIDO DEL SALTO ***
void PhysicsManager::updateJumpStep(bool wantJump, bool hasAnyDirection) {
    

    // ==========================================
    // FASE 1: ESPERANDO EL DELAY
    // ==========================================
    if (isWaitingJumpImpulse) {
        // Quedan de DELAY
        if (delayFramesRemainingJump > 0) {
            // Quitamos uno
            delayFramesRemainingJump--;
            // Salimos, segimos esperando
            return;
        }
        // ==========================================
        // FASE 1.1: COMPLETADO DELAY
        // ==========================================
        isWaitingJumpImpulse = false;
        
        // JUMP STARTED, pero no presionada tecla SPACE
        if (!wantJump) {
            
            // SALTO MÍNIMO
            isMinJump = true;
            
            // DEFINIMOS Total de frames de salto:
            // Delay ya ha pasado, no lo sumamos
            // Frames de salto mínimo
            // Frames de parada (JUMP_TO_FALL)
            framesRemainingJump = minJumpFrames + framesRemainingJumpStop;
            
            // RECALCULO impulso
            jumpImpulse = framesRemainingJump * gravity.y;
        }

        // APLICAMOS EL IMPULSO (negativo para subir)
        velocity.y = -jumpImpulse;
        isImpulsing = true;
        
        
        
        // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
        //              GESTIÓN X SALTO CON VELOCIDAD ANTERIOR
        // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
        // Si hemos guardado una velocidad anterior al salto
        
        
        // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
        //     TIENE QUE VER CON EL REPLANTEAMIENTO Y FACTORIZACIÓN DE PHYSICS MANAGER
        // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        // TENEMOS EL MISMO ERROR CONCEPTUAL DE LLAMAR CADA FRAME A startVelocityCHange
        // la función startVelocityCHange que reutilizamos no estan pensadas para llamarlas en cada Frame.
        //if (!isVelocityChanging) return;
        
        if(beforeJumpVelocityX !=0 ){
            // Si hay intención de dirección
            if(hasAnyDirection){
                 // Recuperamos la dirección original mirando el signo de la velocidad guardada
                 bool wasLookingRight = (beforeJumpVelocityX > 0);
                        
                 // Aplicamos rampa para recuperar esa velocidad máxima con la dirección correcta
                 startVelocityChange(maxSpeedAir, airForwardFrames, wasLookingRight);
            }
            else {
                // Resetamos la maxSpeedAir con su valor base
                setMaxSpeedAir(baseMaxSpeedAir);
            }
        }
        // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
        //
        // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
        
        
        return;
    }
    
    // ==========================================
    // FASE 2: CONTANDO FRAMES DE SUBIDA
    // ==========================================
    // Mientras estamos impulsando
    if (isImpulsing) {
        // Quitamos frame de salto
        framesRemainingJump--;
        
        // Si solo quedan los stop frames
        if (framesRemainingJump <= framesRemainingJumpStop) {
            // Informamos el final de impulso, translateIntent de MovementManager nos pondrá en GO_FALL
            isImpulsing = false;
        }
    }
}

// *** EL LATIDO DE JUMP_TO_FALL ***
void PhysicsManager::updateJumpToFallStep() {
    
    // ==========================================
    // FASE 1: FRENADO (stopFrames frames)
    // ==========================================
    // Estamos en Proceso de Subir a Bajar
    if (isStartingToFall) {
        
        // La Gravedad está inhabilitada
        // Aplicamos nuevo paso de velocidad, para llegar a 0 en STOP_FRAMES
        velocity.y += stopStepY;
        
        // QUitamos uno porque hemos aplicado el paso
        framesRemainingJumpStop--;
        
        // ==========================================
        // FASE 1.1: FRENADO COMPLETADO
        // ==========================================
        if (framesRemainingJumpStop <= 0) {
            // Debemos estar cerca de 0
            // Forzamos la velocidad a 0, evitamos floats residuales
            velocity.y = 0.0f;
            // Ya hemos pasado de Subir a Bajar, justo estamos en 0
            isStartingToFall = false;
            
            // ESTO SE USA PARA FORZAR QUE HAYA UN FRAME CON VELOCIDAD 0, queda más fino
            isHanging = true;
        }
        return;
    }
    
    // ==========================================
    // FASE 2: HANGING (FORZAR QUE HAYA UN FRAME CON VELOCIDAD 0)
    // ==========================================
    if (isHanging) {
        velocity.y = 0.0f;      // Mantenemos parado
        gravityOverride = false; // Liberamos la gravedad
        isHanging = false;
    }
}


// *** CAMBIOS DE VELOCIDAD en Y ***
// Se dispara con los MOVIMIENTOS TO_JUMP
void PhysicsManager::startVelocityJump(int frames, int delay, bool lookingRight, float jumpVelocityX) {

    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
    //              GESTIÓN X SALTO CON VELOCIDAD ANTERIOR
    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
    // Si lleva una velocidad al querer saltar (por ejemplo viene caminando o correindo)
    if(velocity.x !=0){
        
        // AQUÍ PROBLEMAS CON LA ESCALA, SIEMPRE SERÍA 10 O PUEDE LLEGAR UN VALOR DIFERENTE AL BASE QUE SEA MAYOR QUE 10 PERO IGUALMENTE LENTO
        if(jumpVelocityX > 10){
            jumpVelocityX = 10;
        }
        // No escalamos porque ya viene escalada
        beforeJumpVelocityX = jumpVelocityX;
        // Sobreescribimos la maxSpeedAir, (baseMaxSpeedAir, no se toca)
        maxSpeedAir =  abs(beforeJumpVelocityX);
        // Vamos hacia 0 en los frames antes del impulso
        startVelocityChange(0, delay, lookingRight);
    }
    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
    //
    // ≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈
    
    
    
    
    // stopFrames se guarda en framesRemainingJumpStop
    framesRemainingJumpStop = stopFrames;
    // Frames de dentro de movimientos TO_JUMP donde ya se eleva
    impulseFrames = frames - delay;

    // DEFINIMOS Total de frames de salto:
    // Frames del movimiento menos el delay
    // Frames de salto máximo
    // Frames de parada (JUMP_TO_FALL)
    framesRemainingJump = impulseFrames + maxJumpFrames + framesRemainingJumpStop;
    
    
    // *** CÁLCULO DEL IMPULSO INICIAL ***
    // Cada frame de subida, la gravedad suma +gravity.y a velocity.y.
    // Para que velocity.y llegue a exactamente 0 al final de la subida,
    // el impulso inicial debe ser igual a lo que la gravedad va a consumir
    // durante todos los frames de subida:
    //     jumpImpulse = N * gravity.y   (donde N = framesRemainingJump)
    jumpImpulse = framesRemainingJump * gravity.y;
    
    
    // El impulso se aplicará después del delay (en el frame indicado de IDLE_TO_JUMP)
    delayFramesRemainingJump = delay;
    isImpulsing = false;
    isWaitingJumpImpulse = true;
}

// Configura una rampa de cambio de dirección (giro),en el aire es busca llegar a la máxima, no a la que llevaba
void PhysicsManager::startVelocityTurnChangeAir(float targetAbsSpeed, int frames, bool lookingRight) {
    if (frames <= 0) return;

    framesRemaining = frames;
    isVelocityChanging = true;

    // Si queremos mirar a la derecha, target es positivo. Si a la izquierda, negativo.
    targetVelocityX = lookingRight ? targetAbsSpeed : -targetAbsSpeed;
    
    velocityStepX = (targetVelocityX - velocity.x) / (float)framesRemaining;
}


// Se dispara con el MOVIMIENTO JUMP_TO_FALL
void PhysicsManager::startJumpToFall() {

    // *** CÁLCULO DEL STEP DE PARADA GRADUAL ***
    // Con gravityOverride=true la gravedad NO actúa durante esta fase.
    // Solo actúa stopStepY, que cada frame suma una cantidad fija.
    // Usamos la fórmula de step lineal: (destino - origen) / pasos
    //     = (0 - velocity.y) / framesRemainingJumpStop
    //     = -velocity.y / framesRemainingJumpStop
    // Como velocity.y es negativa en este momento, el resultado es positivo:
    // cada frame sumamos ese valor positivo a velocity.y hasta llegar a 0.
    stopStepY = -velocity.y / framesRemainingJumpStop;
    
    // Suspendemos la gravedad (sino interferiría con el cálculo lineal)
    gravityOverride = true;
    
    // Empezamos el proceso de Subir a Bajar
    isStartingToFall = true;
    
    // PARA FORZAR ALMENOS UN FRAME PARADO
    isHanging = false;
}

// *** RECALCULO FRAMES y VELOCIDAD para CORTE SALTO ***
// Se dispara mientras estamos en JUMP, pero se ha soltado la tecla SPACE (lo dispara translateIntent en MovementManager)
void PhysicsManager::cutJump() {
    
    isJumpCutted = true;
    
    // SWITCH LOG EXPLICATIVO
    /*
    switch (framesRemainingJump) {
        case 11:
            ofLogNotice("Physics") << "[FRAME -2 (IDLE_TO_JUMP)] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 10:
            ofLogNotice("Physics") << "[FRAME -1 (IDLE_TO_JUMP)] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 9:
            ofLogNotice("Physics") << "[FRAME 0 (IDLE_TO_JUMP)] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 8: // Frame 1 del salto real
            ofLogNotice("Physics") << "[FRAME 1] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 7: // Frame 2 del salto real
            ofLogNotice("Physics") << "[FRAME 2] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 6: // Frame 3 del salto real
            ofLogNotice("Physics") << "[FRAME 3] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 5: // Frame 4 del salto real
            ofLogNotice("Physics") << "[FRAME 4] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 4: // Frame 5 del salto real
            ofLogNotice("Physics") << "[FRAME 5] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 3: // Frame 6 del salto real
            ofLogNotice("Physics") << "[FRAME 6] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 2: // Penúltimo frame (Frenado gradual)
            ofLogNotice("Physics") << "[FRAME 7] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;
            
        case 1: // Último frame (Frenado gradual)
            ofLogNotice("Physics") << "[FRAME 8] Quedan: " << framesRemainingJump
                                   << " | Parada: " << stopFrames
                                   << " | Subida Pura (Totales - Parada): " << (framesRemainingJump - stopFrames);
            break;

        default:
            ofLogNotice("Physics") << "[ALERTA] cutJump() llamado con valor inesperado: " << framesRemainingJump;
            break;
    }
     */
    
    // Si quedan más que el mínimo, lo convertimos al mínimo, si quedan menos lo termina
    if ( (framesRemainingJump - framesRemainingJumpStop) > minJumpFrames) {
        // Forzamos al mínimo
        framesRemainingJump = minJumpFrames + framesRemainingJumpStop;
        
        // *** RECÁLCULO DE velocity.y AL CORTAR EL SALTO ***
        // La gravedad sola es la que lleva velocity.y hasta 0, frame a frame.
        // En N frames habrá sumado: N * gravity.y
        // Para que velocity.y llegue a exactamente 0 al final de esos N frames:
        //     velocity.y + (N * gravity.y) = 0
        //     velocity.y = -(N * gravity.y)
        velocity.y = -(float)framesRemainingJump * gravity.y;
    }
}

// REINICIO DE VARIABLES DE CONTORL y StopStep
void PhysicsManager::resetJumpVariables() {
    
    isWaitingJumpImpulse = false;
    isImpulsing = false;
    isMinJump = false;
    isJumpCutted = false;

    isStartingToFall = false;
    isHanging = false;
    gravityOverride = false;
    
    stopStepY = 0.0f;
    
    maxSpeedAir = 4.0f;
    beforeJumpVelocityX = 0;
}


// !!!!!!!!! GIRO AIRE !!!!!!!!
// Mira si quedan almenos TURN JUMP FRAMES o más frames de subida antes de JUMP_TO_FALL
bool PhysicsManager::checkTurnJump(int requiredFrames) {
    int framesSubida = framesRemainingJump - framesRemainingJumpStop;
    // Si todavía está en impulso activo (botón pulsado)
    if (isImpulsing) {
        ofLogNotice("MovementManager") << "CHECK MIENTRAS IMPULSING";
        // Frames de subida pura: total menos los frames de parada gradual
        
        if(!(framesSubida >= requiredFrames)){
            ofLogNotice("MovementManager") << "DENEGADOOOOOO POOOOOR      " << framesSubida << "     FRAMES DE SUBIDA RESTANTES (NECESITA 4 O MÁS)";

        }
        return (framesSubida >= requiredFrames);
    }
    // Si el salto ya ha sido cortado, pero sigue flotando hacia arriba en el frenado gradual
    else {
        ofLogNotice("PhysicsManager") << "is Impulsing FALSE, pero SUBIENDO";
        // Mientras la velocidad sea hacia arriba (negativa en Y) y queden frames de frenado
        return (velocity.y < 0 && framesSubida >= requiredFrames);
    }
}

// *** APLICAR FISICAS ***
void PhysicsManager::applyVelocity() {
    // El movimiento real: aquí la posición X (e Y más adelante) se actualizan
    position.x += velocity.x;
    position.y += velocity.y;
}
void PhysicsManager::applyGravity() {
    
    if (gravityOverride) return;
    velocity.y += gravity.y;
}





// ------------------- GETTERS - SETTERS -------------------

// *** GETTERS ***
ofVec2f PhysicsManager::getPosition() const {
    return position;
}
ofVec2f PhysicsManager::getVelocity() const {
    return velocity;
}
float PhysicsManager::getMaxSpeedWalk() const {
    return maxSpeedWalk;
}
float PhysicsManager::getMaxSpeedRun() const {
    return maxSpeedRun;
}
float PhysicsManager::getMaxSpeedAir() const {
    return maxSpeedAir;
}
float PhysicsManager::getBaseMaxSpeedAir() const {
    return baseMaxSpeedAir;
}

float PhysicsManager::getGravityY() const {
    return gravity.y;
}
float PhysicsManager::getCurrentScale() const{
    return currentScale;
}

bool PhysicsManager::getIsVelocityChanging() const {
    return isVelocityChanging;
}

// *** GETTERS JUMP ***
int PhysicsManager::getFramesRemainingJump() const {
    return framesRemainingJump;
}
int PhysicsManager::getFramesRemainingJumpStop() const {
    return framesRemainingJumpStop;
}
int PhysicsManager::getStopFramesJump() const {
    return stopFrames;
}
int PhysicsManager::getImpulseFrames() const {
    return impulseFrames;
}
bool PhysicsManager::getIsImpulsing() const {
    return isImpulsing;
}
bool PhysicsManager::getGravityOverride() const {
    return gravityOverride;
}
bool PhysicsManager::getIsMinJump() const {
    return isMinJump;
}
bool PhysicsManager::getIsJumpCutted() const {
    return isJumpCutted;
}
int PhysicsManager::getAirForwardFrames() const {
    return airForwardFrames;
}
int PhysicsManager::getAirUnforwardFrames() const {
    return airUnforwardFrames;
}

// *** SETTERS ***
void PhysicsManager::setPositionX(float newX) {
    position.x = newX;
}
void PhysicsManager::setPositionY(float newY) {
    position.y = newY;
}
void PhysicsManager::setCurrentScale(float scale) {
    currentScale = scale;
    // Debemos recalcular las físicas si la escala cambia.
    updateScaledPhysics();
}
void PhysicsManager::setVelocityY(float newY) {
    velocity.y = newY;
}
void PhysicsManager::setGravityY(float newY) {
    gravity.y = newY;
}
void PhysicsManager::resetGravityY() {
    gravity.y = baseGravity.y * currentScale;;
}


// *** SETTERS FOR GUI ***
void PhysicsManager::setStopFrames(int value) {
    stopFrames = value;
}
void PhysicsManager::setMinJumpFrames(int value) {
    minJumpFrames = value;
}
void PhysicsManager::setMaxJumpFrames(int value) {
    maxJumpFrames = value;
}

void PhysicsManager::setAirForwardFrames(int value) {
    airForwardFrames = value;
}
void PhysicsManager::setAirUnforwardFrames(int value) {
    airUnforwardFrames = value;
}

void PhysicsManager::setMaxSpeedWalk(float maxSpeed) {
    // Guardamos el valor puro que viene del GUI
    baseMaxSpeedWalk = maxSpeed;
    // Sincronizamos para que el valor de trabajo (maxSpeedWalk) se actualice
    updateScaledPhysics();
}
void PhysicsManager::setMaxSpeedRun(float maxSpeed) {
    // Guardamos el valor puro que viene del GUI
    baseMaxSpeedRun = maxSpeed;
    // Sincronizamos para que el valor de trabajo (maxSpeedRun) se actualice
    updateScaledPhysics();
}
void PhysicsManager::setMaxSpeedAir(float maxSpeed) {
    // Guardamos el valor puro que viene del GUI
    baseMaxSpeedAir = maxSpeed;
    // Sincronizamos para que el valor de trabajo (maxSpeedRun) se actualice
    updateScaledPhysics();
}







// *** MÉTODOS INTERNOS ***
void PhysicsManager::updateScaledPhysics() {
    // Actualizamos la gravedad y las velocidades máximas según la escala actual
    gravity.y = baseGravity.y * currentScale;
    
    // Debemos aplicar un facotr de amoritguación para sentirlo más realista.
    float velocityFactor;
    velocityFactor = sqrt(currentScale);

    maxSpeedWalk = baseMaxSpeedWalk * velocityFactor;
    maxSpeedRun = baseMaxSpeedRun * velocityFactor;
    maxSpeedAir = baseMaxSpeedAir * velocityFactor;
}

float PhysicsManager::cleanFloat(float value) {
    // Multiplicamos por 10 para trabajar con el primer decimal como unidad
    float multi = value * 10.0f;
    
    // Si la diferencia con el redondeo es minúscula (error de float)
    if (abs(multi - round(multi)) < 0.01f) {
        return round(multi) / 10.0f;
    }
    
    return value; // Si no hay "ruido", devolvemos el original
}
