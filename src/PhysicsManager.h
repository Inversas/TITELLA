#pragma once
#include <memory>

#include "ofMain.h" // Necesario para ofVec2f y funciones matemáticas básicas

// ****************************************** CLASE PHYSICS MANAGER ****************************************** //
// Clase que gestiona las fisicas
class PhysicsManager {
    
public:
    // !!! MOTIVO CONSTR / DESTR !!!
    // Por RELIGIÓN. No es necesario. Solo usa valores primitivos y no usa Memoria
    // Pero sin él, las variables float o bool contienen "basura" de la RAM (valores aleatorios), así evitamos esto
    // *** CONSTRUCTOR ***
    PhysicsManager();
    // *** DESTRUCTOR ***
    ~PhysicsManager();
    
    // *** SETUP ***
    // Inicializa los valores por defecto (posición inicial, gravedad, etc.)
    void setup(float startX, float startY);
    
    // Calcula la física de cada frame: Gravedad -> Fricción -> Velocidad -> Posición
    void update();


    // *** EL LATIDO  ***
    // Esta función será llamada por MovementManager cada vez que cambie un frame de animación.
    void updateVelocityStep();

    
    // *** CAMBIOS DE VELOCIDAD ***
    // Calcula cuánto debe cambiar la velocidad en cada frame de la animación actual.
    // targetSpeedX: Velocidad final deseada.
    // frames: Cuántos frames de animación dura el cambio.
    // delay: si hay un numero de frames iniciales donde no aplicamos velocidad.
    void startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight, int delay = 0);
    void startVelocityTurnChange(int frames);
    

    // *** APLICAR FISICAS ***
    // Aplicación final del movimiento
    void applyVelocity();
    // Aplica la gravedad al personaje, con un límite de suelo (ground)
    void applyGravity();
    
    

    
    // ------------------- GETTERS - SETTERS -------------------

    // *** GETTERS ***
    ofVec2f getPosition() const;
    ofVec2f getVelocity() const;
    float getMaxSpeedWalk() const;
    float getMaxSpeedRun() const;
    float getGravityY() const;
    float getCurrentScale() const;
    
    // *** SETTERS ***
    void setPositionX(float newX);
    void setPositionY(float newY);
    void setCurrentScale(float scale);
    void setVelocityY(float newY);
    
    void setMaxSpeedWalk(float maxSpeed);
    void setMaxSpeedRun(float maxSpeed);



    
private:
    
    // *** VARIABLES DE ESTADO (Dinámica actual) ***
    ofVec2f position;         // Ubicación actual del títere
    ofVec2f velocity;         // Rapidez y dirección actual (píxeles/frame)
    
    // *** VALORES BASE ***
    ofVec2f baseGravity;      // Gravedad base, sin escalar
    float baseMaxSpeedWalk;   // Velocidad base para caminar, sin escalar
    float baseMaxSpeedRun;    // Velocidad base para correr, sin escalar
    
    // *** VALORES DE TRABAJO ***
    ofVec2f gravity;          // Fuerza de gravedad aplicada (píxeles/frame^2), escalada
    float maxSpeedWalk;       // Límite de velocidad para caminar, escalada
    float maxSpeedRun;        // Límite de velocidad para correr, escalada
    
    // Escala actual del personaje (1.0 = 100%)
    float currentScale = 1.0f;
    
    // *** CONTROL DE TRANSICIÓN DE VELOCIDAD ***
    float targetVelocityX;        // A qué velocidad queremos llegar (ej: 200.0 o 0.0)
    float velocityStep;           // Cuánta velocidad sumamos/restamos en cada tick de animación
    int framesRemaining;          // Cuántos "pasos" quedan para terminar el cambio de velocidad
    int delayFramesRemaining = 0; // Cuántos "pasos" quedan para empezar el cambio de velocidad
    bool isVelocityChanging;      // Flag de seguridad para saber si hay un cambio de velocidad en curso

    // *** MÉTODOS INTERNOS ***
    // Actualiza los valores de trabajo segun la escala
    void updateScaledPhysics();
    
    // Redondea a un decimal limpio
    float cleanFloat(float value);
};

