#include "PhysicsManager.h"

// *** CICLO DE VIDA ***
void PhysicsManager::setup(float startX, float startY) {
    // Posicion Inicial
    position.set(startX, startY);
    // Velocidad Inicial
    velocity.set(0, 0);
    // Gravedad
    gravity.set(0, 5.8f); // Valor de gravedad

    // Escala Inicial
    currentScale = 1.0f;
    
    // Valores iniciales por defecto (se pueden cambiar con los setters)
    maxSpeedWalk = 7.0f;
    maxSpeedRun = 18.0f;
    
    // Inicialización de control
    targetVelocityX = 0.0f;
    velocityStep = 0.0f;
    framesRemaining = 0;
    isVelocityChanging = false;
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
        
        // Aplicamos el paso de velocidad (aceleración o frenado)
        velocity.x += velocityStep;
        
        // Actualizamos el contador de frames para la física actual
        framesRemaining--;

        // Si hemos llegado al final de la transición
        if (framesRemaining <= 0) {
            // Para evitar floats raros (2.99998 en vez de 3.0), usamos la función cleanFloat para redondear a un decimal limpio.
            velocity.x = cleanFloat(velocity.x);
            
            // Limpieza de estado: detenemos el proceso y reseteamos el diferencial
            isVelocityChanging = false;
            velocityStep = 0.0f;
        }
    }
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
    velocityStep = (targetVelocityX - velocity.x) / (float)framesRemaining;
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
    velocityStep = (targetVelocityX - velocity.x) / (float)framesRemaining;
}


// *** APLICAR FISICAS ***
void PhysicsManager::applyVelocity() {
    // El movimiento real: aquí la posición X (e Y más adelante) se actualizan
    position.x += velocity.x;
    position.y += velocity.y;
}

void PhysicsManager::applyGravity() {
    velocity.y += gravity.y;
    //position.y += gravity.y;
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


// *** GETTERS ***
void PhysicsManager::setPositionX(float newX) {
    position.x = newX;
}
void PhysicsManager::setPositionY(float newY) {
    position.y = newY;
}
void PhysicsManager::setCurrentScale(float scale) {
    currentScale = scale;
}
void PhysicsManager::setVelocityY(float newY) {
    velocity.y = newY;
}




// |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
// !!!!!! ESTO SE DISPARAAA !!!! no puede estar bieeen !!!
void PhysicsManager::setMaxSpeedWalk(float maxSpeed) {
    // Aplicamos el escalado directamente en la asignación
    maxSpeedWalk = maxSpeed * currentScale;
}
void PhysicsManager::setMaxSpeedRun(float maxSpeed) {
    // Aplicamos el escalado directamente en la asignación
    maxSpeedRun = maxSpeed * currentScale;
}
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||







// *** MÉTODOS INTERNOS ***
float PhysicsManager::cleanFloat(float value) {
    // Multiplicamos por 10 para trabajar con el primer decimal como unidad
    float multi = value * 10.0f;
    
    // Si la diferencia con el redondeo es minúscula (error de float)
    if (abs(multi - round(multi)) < 0.01f) {
        return round(multi) / 10.0f;
    }
    
    return value; // Si no hay "ruido", devolvemos el original
}

