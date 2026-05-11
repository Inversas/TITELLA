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
    // Esta función será llamada por MovementManager cada vez que cambie un frame de animación, se encarga del eje Y
    void updateVelocityStepY();
    
    // *** CAMBIOS DE VELOCIDAD ***
    // Calcula cuánto debe cambiar la velocidad en cada frame de la animación actual.
    // targetSpeedX: Velocidad final deseada.
    // frames: Cuántos frames de animación dura el cambio.
    // delay: si hay un numero de frames iniciales donde no aplicamos velocidad.
    void startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight, int delay = 0);
    void startVelocityTurnChange(int frames);
    
    // -------- NUEVO --------
    // !!!!!!!!! JUMP !!!!!!!!!
    // *** GESTIÓN SALTO ***
    void startVelocityJump (int frames, int delay);
    void updateJumpStep(bool wantJump);
    void startJumpToFall();
    void updateJumpToFallStep();
    void cutJump();
    
    int getFramesRemainingJump() const;
    bool getIsImpulsing() const;
    bool getGravityOverride() const;
    bool getIsMinJump() const;
    void resetJumpState();
    

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
    void setGravityY(float newY);
    void resetGravityY();

    
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
    float targetVelocityX;         // A qué velocidad queremos llegar (ej: 200.0 o 0.0)
    float targetVelocityY;         // A qué velocidad queremos llegar (ej: 200.0 o 0.0) en el eje Y


    float velocityStepX;           // Cuánta velocidad sumamos/restamos en cada tick de animación en el eje X
    float velocityStepY;           // Cuánta velocidad sumamos/restamos en cada tick de animación en el eje Y
    int framesRemaining;           // Cuántos "pasos" quedan para terminar el cambio de velocidad
    int delayFramesRemaining = 0;  // Cuántos "pasos" quedan para empezar el cambio de velocidad
    bool isVelocityChanging;       // Flag de seguridad para saber si hay un cambio de velocidad en curso
    
    
    
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // CREAR VALROES DE TRABAJO Y VALORES BASE PARA EL ESCALADO DE LA GESTION DE SALTO
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    
    // -------- NUEVO --------
    // !!!!!!!!! JUMP !!!!!!!!!
    float jumpImpulse = 0.0f;
    
    
    // *** JUMP FRAMES ***
    // FRAMES movimiento _TO_JUMP, menos DELAY
    int impulseFrames;
    
 
    
    // Frames que quedan para completar el salto
    int framesRemainingJump;
    
    // Frames parada gradual en JUMP_TO_FALL
    int framesRemainingJumpStop;

    // Frames a esperar para iniciar el salto en _TO_JUMP
    int delayFramesRemainingJump;
    
    // Siempre usamos 2 frames de JUMP_TO_FALL // (Controlable por GUI) (de 2 a 4)
    int stopFrames = 2;
    
    // SALTO MÍNIMO
    int minJumpFrames = 3; // (Controlable por GUI) (de 1 a 3)
    
    // SALTO MÁXIMO
    int maxJumpFrames = 6; //(Controlable por GUI) (de 3 a 6)

    
    
   

    
    // *** VARIABLES DE CONTROL ***
    bool isImpulsing = false;
    bool isWaitingJumpImpulse = false;
    bool isStartingToFall = false;
    bool isHanging = false;
    bool gravityOverride = false;
    bool isMinJump = false; 

    
    
    
   

    
    // *** JUMP TO FALL ***
    float stopStepY = 0.0f;

    
    
    
    


    // *** MÉTODOS INTERNOS ***
    // Actualiza los valores de trabajo segun la escala
    void updateScaledPhysics();
    
    // Redondea a un decimal limpio
    float cleanFloat(float value);
};

