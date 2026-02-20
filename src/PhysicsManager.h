#pragma once


#include "ofMain.h" // Necesario para ofVec2f y funciones matemáticas básicas
#include <memory>

/*
 * PhysicsManager: El "Motor de Impulso"
 * Se encarga de la simulación de fuerzas, gravedad e inercia.
 * No conoce el escenario, solo sabe cómo debería moverse el cuerpo en el vacío.
*/


// Clase que gestiona las fisicas en el juego
class PhysicsManager {
    
//"Public" es lo que la clase hace por los demás.
public:
    
    
    // *** Ciclo de Vida *** //
    
    // Inicializa los valores por defecto (posición inicial, gravedad, etc.)
    void setup(float startX, float startY);
    
    // Calcula la física de cada frame: Gravedad -> Fricción -> Velocidad -> Posición
    void update();


    //*** EL LATIDO  ***
    // Esta función será llamada por MovementManager cada vez que cambie un frame de animación.
    void updateVelocityStep();

    
    // *** INTERFAZ DE CONTROL (Órdenes del MovementManager) ***
    // Calcula cuánto debe cambiar la velocidad en cada frame de la animación actual.
    // targetSpeedX: Velocidad final deseada.
    // frames: Cuántos frames de animación dura el cambio.
    void startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight);
    void startVelocityTurnChange(int frames);
    
    

    
    
    // Aplicación final del movimiento
    void applyVelocity();
    
    // Aplica la gravedad al personaje, con un límite de suelo (ground)
    void applyGravity();
    
    
    // Nota: Los Getters y Setters que definimos en el .h como "inline"
    // (dentro de la llave { }) no necesitan escribirse en el .cpp
    
    // *** Getters *** //
    ofVec2f getPosition() const { return position; }
    ofVec2f getVelocity() const { return velocity; }
    float getMaxSpeedWalk() const { return maxSpeedWalk; }
    float getMaxSpeedRun() const { return maxSpeedRun; }
    float getGravityY() const { return gravity.y; }
    
    // *** Setters *** //
    void setPositionY(float newY) { position.y = newY; }
    
    // !!!!!! ESTO SE DISPARAAA !!!! no puede estar bieeen !!!
    void setMaxSpeedWalk(float maxSpeed) { maxSpeedWalk = maxSpeed * currentScale; }
    void setMaxSpeedRun(float maxSpeed) { maxSpeedRun = maxSpeed * currentScale; }
    
    
    void setCurrentScale(float scale) { currentScale = scale; }
    
    
    

//"Private" es cómo la clase se organiza a sí misma para hacer su trabajo.
private:
    
    // *** VARIABLES DE ESTADO (Dinámica actual) ***
        ofVec2f position;      // Ubicación actual del títere
        ofVec2f velocity;      // Rapidez y dirección actual (píxeles/frame)
        ofVec2f gravity;       // Fuerza de gravedad aplicada (píxeles/frame^2)
        
        float maxSpeedWalk;    // Límite de velocidad para caminar
        float maxSpeedRun;     // Límite de velocidad para correr
        
        // Escala actual del personaje (1.0 = 100%)
        float currentScale = 1.0f;
        
        // *** CONTROL DE TRANSICIÓN DE VELOCIDAD ***
        float targetVelocityX;  // A qué velocidad queremos llegar (ej: 200.0 o 0.0)
        float velocityStep;    // Cuánta velocidad sumamos/restamos en cada tick de animación
        int framesRemaining;    // Cuántos "pasos" quedan para terminar el cambio de velocidad
        bool isVelocityChanging;   // Flag de seguridad para saber si hay un cambio de velocidad en curso

        // *** MÉTODOS INTERNOS ***
        // Asegura que la velocidad no supere los límites configurados.
        void limitSpeed();
};
