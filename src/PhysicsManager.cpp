#include "PhysicsManager.h"



// -------- NUEVO --------
// !!!!!!!!! JUMP !!!!!!!!!
// *** CONSTRUCTOR ***
PhysicsManager::PhysicsManager()
    :
       /*currentScale(1.0f),
       targetVelocityX(0.0f),
       velocityStepX(0.0f),
       framesRemaining(0),
       delayFramesRemaining(0),
       isVelocityChanging(false)*/

      currentScale(1.0f),
      targetVelocityX(0.0f),
      velocityStepX(0.0f),
      framesRemaining(0),
      delayFramesRemaining(0),
      isVelocityChanging(false),
      // *** JUMP ***
      jumpImpulse(0.0f),
      stopFrames(2),
      delayFramesRemainingJump(0),
      framesRemainingJump(0),
      isImpulsing(false),
      isWaitingJumpImpulse(false),
      // *** JUMP TO FALL ***
      stopStepY(0.0f),
      framesRemainingJumpStop(0),
      isStartingToFall(false),
      isHanging(false),
      gravityOverride(false)
{
    // Inicializamos los vectores a cero
    position.set(0, 0);
    velocity.set(0, 0);
    baseGravity.set(0, 0);
    gravity.set(0, 0);
    
    // Inicializamos velocidades base
    baseMaxSpeedWalk = 0.0f;
    baseMaxSpeedRun = 0.0f;
    maxSpeedWalk = 0.0f;
    maxSpeedRun = 0.0f;

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
    
    // Escala Inicial
    currentScale = 1.0f;
    
    // Inicialización de control
    targetVelocityX = 0.0f;
    velocityStepX = 0.0f;
    framesRemaining = 0;
    isVelocityChanging = false;
    
    // Debemos dar valor a las variables de trabajo con la escala inicial
    updateScaledPhysics();
}


void PhysicsManager::update() {
    // Aquí irían fuerzas continuas como la gravedad si el títere saltara.
    // De momento, la velocidad horizontal la controla el "Latido".
}


// *** EL LATIDO ***
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
void PhysicsManager::updateVelocityStepY() {
    
}


// *** CAMBIOS DE VELOCIDAD ***
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




// -------- NUEVO --------
// !!!!!!!!! JUMP !!!!!!!!!
// *** GESTIÓN SALTO ***

// impulseFrames, frames de impulso 2 = frames - delay (7-5)
// frames de salto máximo jumpFrames = 3
// frames de parada (JUMP_TO_FALL) stopFrames = 2 (para el máximo)
void PhysicsManager::startVelocityJump(int frames, int delay) {

    framesRemainingJumpStop = stopFrames;
    // totalJumpFrames: frames de IDLE_TO_JUMP (dos últimos, (frames - delay), 3 de JUMP y 2 de JUMP_TO_FALL
    framesRemainingJump = (frames - delay) + maxJumpFrames + framesRemainingJumpStop;
    ofLogNotice("PhysicsManager") << "FRAMES IDLE_TO_JUMP = " << frames;
    ofLogNotice("PhysicsManager") << "DELAY = " << delay;
    ofLogNotice("PhysicsManager") << "USADO MAXJUMPFRAMES = " << maxJumpFrames;
    ofLogNotice("PhysicsManager") << "FRAMES STOP = " << maxJumpFrames;
    ofLogNotice("PhysicsManager") << "FRAMES RESTANTES INICIO SALTO = " << framesRemainingJump;

    
    // El impulso debe ser suficiente para que la gravedad nos lleve a una velocidad
    // que en 'stopFrames' frames pueda llegar a 0.
    
    // Es decir, en (totalJumpFrames - stopFrames) frames la gravedad habrá consumido:
    // (totalJumpFrames - stopFrames) * gravity.y
    
    // Lo que queda para que JUMP_TO_FALL lo lleve a 0 en stopFrames:
    // stopFrames * gravity.y
    
    // Por tanto el impulso total es:
    jumpImpulse = framesRemainingJump * gravity.y;
    
    // El impulso se aplica después del delay (en el frame indicado de IDLE_TO_JUMP)
    delayFramesRemainingJump = delay;
    
    // Todavía no estamos impulsando, esperamos el delay
    isImpulsing = false;
    isWaitingJumpImpulse = true;
}

// Aquí podría llegar un parámetro, que si no estamos wantJump, entonces recalcular el impulso, para hacer el salto corto.
void PhysicsManager::updateJumpStep(bool wantJump) {
    
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
        
        // ESPERA DE DELAY COMPLETADA
        isWaitingJumpImpulse = false;
        
        // SI NO QUEREMOS SEGUIR SALTANDO, aplicamos el salto mínimo
        if (!wantJump) {
            isMinJump = true;
            // totalJumpFrames: EL DELAY YA PASÓ, y aplicamos los valores de salto mínimo
            framesRemainingJump = minJumpFrames + framesRemainingJumpStop;
            jumpImpulse = framesRemainingJump * gravity.y;
            ofLogNotice("PhysicsManager") << "------- SALTO CORTO";
        }
        ofLogNotice("PhysicsManager") << "------- IMPULSO APLICDAO";
        // APLICAMOS EL IMPULSO
        velocity.y = -jumpImpulse;
        // Estamos subiendo
        isImpulsing = true;
        return;
    }
    
    // ==========================================
    // FASE 2: CONTANDO FRAMES DE SUBIDA
    // ==========================================
    // Mientras estamos subiendo
    if (isImpulsing) {
        // Quitamos frame de salto
        framesRemainingJump--;
        
        // Si solo quedan los stop frames
        if (framesRemainingJump <= framesRemainingJumpStop) {
            // Inofrmamos el final de impulso, translateIntent nos pondrá en GO_FALL
            isImpulsing = false;
            ofLogNotice("PhysicsManager") << "FRAMES DE SALTO AGOTADOS, a VEL = " << velocity.y;
        }
    }
}


void PhysicsManager::startJumpToFall() {

    // Desde la velocity.y actual, calculamos el step para llegar a 0
    // en exactamente stopFrames frames, sin gravedad activa durante ese proceso.
    // stopStepY será positivo porque velocity.y es negativa (aún subiendo)
    stopStepY = -velocity.y / framesRemainingJumpStop;
    
    // Suspendemos la gravedad — ella interferiría con el cálculo lineal
    gravityOverride = true;
    
    // Empezamos el proceso de Subir a Bajar
    isStartingToFall = true;
    
    // PARA FORZAR ALMENOS UN FRAME PARADO
    isHanging = false;
    ofLogNotice("PhysicsManager") << "------- JUMP TO FALL";

}


void PhysicsManager::updateJumpToFallStep() {
    
    // ==========================================
    // FASE 1: FRENADO (stopFrames frames)
    // ==========================================
    // Estamos en Proceso de Subir a Bajar
    if (isStartingToFall) {
        
        // La Gravedad está inhabilitada
        // Aplicamos el cambio de velocidad, segun los frames definidos para llegar a 0
        velocity.y += stopStepY;
        
        // Es stopFrames
        // QUitamos uno porque hemos aplicado el paso
        framesRemainingJumpStop--;
        
        // Si no quedan stopFrames
        if (framesRemainingJumpStop <= 0) {
            // Forzamos la velocidad a 0, evitamos floats residuales
            velocity.y = 0.0f;
            // Ya hemos pasado de Subir a Bajar, justo estamos en 0
            isStartingToFall = false;
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // CUANDO FUNCIONE, VER SI NOS GUSTA HANGING
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            // ESTO SE USA PARA FORZAR QUE HAYA UN FRAME CON VELOCIDAD 0
            // isHanging = true;
            // SI HACEMOS LO ANTERIOR EL GRAVITYOVERRIDE SE PONE A FALSE EN EL HANGING
            gravityOverride = false; // Liberamos la gravedad

            ofLogNotice("PhysicsManager") << "-------  FRENADO COMPLETO";
        }
        return;
    }
    
    
    // ESTO SE USA PARA FORZAR QUE HAYA UN FRAME CON VELOCIDAD 0
    // ==========================================
    // FASE 2: HANGING (1 frame parado)
    // ==========================================
    /*if (isHanging) {
        velocity.y = 0.0f;      // Mantenemos parado
        gravityOverride = false; // Liberamos la gravedad
        isHanging = false;
        ofLogNotice("PhysicsManager") << "Hanging completo, gravedad activa";
    }*/
}


// Después de revisar todo lo anterior tendremos más control para hacer lo siguiente
void PhysicsManager::cutJump() {
    
    ofLogNotice("PhysicsManager") << ")()()()()()()()()( CORTE, quedaban total de !!! " << framesRemainingJump;
    ofLogNotice("PhysicsManager") << "!!!! CORTE, quedaban stopframes !!! " << framesRemainingJumpStop;
    ofLogNotice("PhysicsManager") << "!!!! CORTE, quedaban jumpframes !!! " << framesRemainingJump-framesRemainingJumpStop;

        
    int jumpFramesRemaining = (framesRemainingJump - framesRemainingJumpStop);
    
    // ¿Quedan más frames de subida que el mínimo?
    if ( (framesRemainingJump - framesRemainingJumpStop) > minJumpFrames) {
        ofLogNotice("PhysicsManager") << "·······················RECALUCLO !!! " << minJumpFrames + framesRemainingJumpStop;
        // Forzamos al mínimo
        framesRemainingJump = minJumpFrames + framesRemainingJumpStop;
        
        // Recalculamos la velocity para que sea coherente:
        // En minJumpFrames frames, la gravedad la llevará a una velocidad
        // que en stopFrames frames llegue a 0
        // Eso significa que ahora debe ser: -(framesRemainingJump) * gravity.y
        velocity.y = -(float)framesRemainingJump * gravity.y;
    }
    // Si quedan menos o igual que el mínimo, dejamos correr lo que queda
    //isImpulsing = false; 
}

int PhysicsManager::getFramesRemainingJump() const {
    return framesRemainingJump;
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


// |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
// CUANDO FUNCIONE, REVISAR NECESARIO
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void PhysicsManager::resetJumpState() {
    isImpulsing          = false;
    isWaitingJumpImpulse = false;
    isStartingToFall     = false;
    isHanging            = false;
    gravityOverride      = false;
    framesRemainingJump  = 0;
    framesRemainingJumpStop  = 0;
    delayFramesRemainingJump = 0;
    stopStepY            = 0.0f;
    //ofLogNotice("PhysicsManager") << "$$$$$$$ resetJumpState: isMinJump era " << isMinJump;

    isMinJump            = false;

    stopFrames = 2;
    
    ofLogNotice("PhysicsManager") << "resetJumpState: estado de salto limpiado al aterrizar.";
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
float PhysicsManager::getGravityY() const {
    return gravity.y;
}
float PhysicsManager::getCurrentScale() const{
    return currentScale;
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







// *** MÉTODOS INTERNOS ***
void PhysicsManager::updateScaledPhysics() {
    // Actualizamos la gravedad y las velocidades máximas según la escala actual
    gravity.y = baseGravity.y * currentScale;
    
    // Debemos aplicar un facotr de amoritguación para sentirlo más realista.
    float velocityFactor;
    velocityFactor = sqrt(currentScale);

    maxSpeedWalk = baseMaxSpeedWalk * velocityFactor;
    maxSpeedRun = baseMaxSpeedRun * velocityFactor;
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
