#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"

// *** CICLO DE VIDA ***
void PhysicsManager::setup(float startX, float startY) {
    // Posicion Inicial
    position.set(startX, startY);
    // Velocidad Inicial
    velocity.set(0, 0);
    // Gravedad
    gravity.set(0, 9.8f); // Valor de gravedad (ajustable según necesidades)

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




// *** EL LATIDO (HEARTBEAT) ***
// Aplica el incremento/decremento calculado en cada paso de animación (Frame Data) y gestiona la finalización de la rampa.
void PhysicsManager::updateVelocityStep() {
    // Si hay una transición activa y frames pendientes
    if (isVelocityChanging && framesRemaining > 0) {
        
        // Aplicamos el paso de velocidad (aceleración o frenado)
        velocity.x += velocityStep;
        
        // Actualizamos el contador de frames para la física actual
        framesRemaining--;

        // Si hemos llegado al final de la transición
        if (framesRemaining <= 0) {
            // Forzamos la velocidad al objetivo exacto para evitar floats raros (2.99998 en vez de 3.0).
            velocity.x = targetVelocityX;
            
            // Limpieza de estado: detenemos el proceso y reseteamos el diferencial
            isVelocityChanging = false;
            velocityStep = 0.0f;
        }
    }
}



// Configura una rampa de aceleración o frenado (cambio de velocidad progresivo).
void PhysicsManager::startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight) {

    // Determinar el vector de velocidad objetivo según la orientación del personaje
    float targetSpeedWithDirection = lookingRight ? targetAbsSpeed : -targetAbsSpeed;

    // CASO INSTANTÁNEO: Si no hay frames de transición, aplicamos la velocidad de golpe
    // Para no dividir entre 0
    if (frames <= 0) {
        velocity.x = targetSpeedWithDirection;
        targetVelocityX = targetSpeedWithDirection;
        isVelocityChanging = false;
        framesRemaining = 0;
        return; // Salida temprana para evitar cálculos de rampa
    }

    // CONFIGURACIÓN DE LA RAMPA:
    // Guardamos la meta final y el tiempo (frames) que tenemos para llegar
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

void PhysicsManager::applyVelocity() {
    // El movimiento real: aquí la posición X (e Y más adelante) se actualizan
    position.x += velocity.x;
}

void PhysicsManager::applyGravity() {
        position.y += gravity.y;
}


// *** MÉTODOS INTERNOS ***
/*
void PhysicsManager::limitSpeed() {
    // Si quisiéramos un límite duro por seguridad fuera de las rampas
    if (velocity.x > maxSpeedRun) velocity.x = maxSpeedRun;
    if (velocity.x < -maxSpeedRun) velocity.x = -maxSpeedRun;
}
 */

