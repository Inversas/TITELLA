#include "MovementCrono.h"

// *** CONSTRUCTOR ***
MovementCrono::MovementCrono()
: timer(0.0f),
currentRegion(0)
{
    // Estado inicial limpio
}

// *** DESTRUCTOR ***
MovementCrono::~MovementCrono() {
    // Limpieza automática por el compilador
}


// *** UPDATE REGION ***
// En lugar de comparar marcas de tiempo externas, sumamos el paso del tiempo (dt)
// y comprobamos si hemos superado el 'interval'.
bool MovementCrono::updateRegion(float dt, float interval) {
    
    // Acumulamos el tiempo transcurrido
    timer += dt;
    
    // Aquí entra en juego el Frame Interval
    // Si el timer llega a ese valor, significa que visualmente ya toca cambiar de dibujo.
    if (timer >= interval) {
        
        // AVANZAMOS REGION
        currentRegion++;
        
        // EL REINICIO INTELIGENTE (Consumo de tiempo)
        // !!! IMPORTANTE !!!: Usamos 'timer -= interval' en lugar de 'timer = 0'.
        // MOTIVO: Si el intervalo era 0.10s y el timer llegó a 0.11s, ese 0.01s
        // de "sobrante" se queda para el siguiente frame.
        // Esto evita que la animación se "arrastre" o pierda fluidez con el tiempo.
        timer -= interval;
        
        // Seguridad: Si por un lag brutal el timer fuera negativo (raro), lo saneamos.
        if(timer < 0) timer = 0;
        
        // Avisamos al Movement Manager de que el dibujo ha cambiado
        return true;
    }
    // No ha habido cambios en este frame
    return false;
}


// ------------------- GETTERS - SETTERS -------------------

int MovementCrono::getCurrentRegion() const {
    return currentRegion;
}

void MovementCrono::setCurrentRegion(int region) {
    currentRegion = region;
}

float MovementCrono::getFrameInterval() const {
    return frameInterval;
}
void MovementCrono::setFrameInterval(float interval){
    frameInterval = interval;
}