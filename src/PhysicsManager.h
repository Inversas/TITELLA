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


    // *** EL LATIDO en X ***
    // Esta función será llamada por MovementManager cada vez que cambie un frame de animación.
    void updateVelocityStep();
    
    // *** CAMBIOS DE VELOCIDAD en X, llamadas en handleMovementPhysics ***
    // Calcula cuánto debe cambiar la velocidad en cada frame de la animación actual.
    // targetSpeedX: Velocidad final deseada.
    // frames: Cuántos frames de animación dura el cambio.
    // delay: si hay un numero de frames iniciales donde no aplicamos velocidad.
    void startVelocityChange(float targetAbsSpeed, int frames, bool lookingRight, int delay = 0);
    void startVelocityTurnChange(int frames);
    

    // *** EL LATIDO DEL SALTO ***
    void updateJumpStep(bool wantJump);
    void updateJumpToFallStep();
    
    // *** CAMBIOS DE VELOCIDAD en Y, llamadas en handleMovementPhysics ***
    void startVelocityJump (int frames, int delay);
    void startJumpToFall();
    // *** RECALCULO FRAMES y VELOCIDAD para CORTE SALTO ***
    void cutJump();
    // REINICIO DE VARIABLES DE CONTORL y StopStep
    void resetJumpVariables();
    

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
    
    // *** GETTERS JUMP ***
    int getFramesRemainingJump() const;
    int getFramesRemainingJumpStop() const;
    int getImpulseFrames() const;
    bool getIsImpulsing() const;
    bool getGravityOverride() const;
    bool getIsMinJump() const;
    bool getIsJumpCutted() const;
    
    
    // *** SETTERS ***
    void setPositionX(float newX);
    void setPositionY(float newY);
    void setCurrentScale(float scale);
    void setVelocityY(float newY);
    void setGravityY(float newY);
    void resetGravityY();
    
    // *** SETTERS FOR GUI ***
    void setMaxSpeedWalk(float maxSpeed);
    void setMaxSpeedRun(float maxSpeed);
    
    // *** SETTERS JUMP FOR GUI ***
    void setStopFrames(int value);
    void setMinJumpFrames(int value);
    void setMaxJumpFrames(int value);

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
    
    // *** CONTROL DE TRANSICIÓN DE VELOCIDAD en X ***
    float targetVelocityX;         // A qué velocidad queremos llegar (ej: 200.0 o 0.0)
    float velocityStepX;           // Cuánta velocidad sumamos/restamos en cada tick de animación en el eje X
    int framesRemaining;           // Cuántos "pasos" quedan para terminar el cambio de velocidad
    int delayFramesRemaining = 0;  // Cuántos "pasos" quedan para empezar el cambio de velocidad
    bool isVelocityChanging;       // Flag de seguridad para saber si hay un cambio de velocidad en curso
    
 
    
    // LA GESTIÓN de SALTO no necesita valores escalados:
    // jumpImpulse = framesRemainingJump * gravity.y    // gravity.y ya está escalada
    // velocity.y  = -jumpImpulse                       // hereda la escala
    // cutJump: velocity.y = -(N * gravity.y)           // hereda la escala
    // stopStepY se calcula a partir de velocity.y en ese momento, que ya tiene la escala incorporada.
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Pero tenerlo en cuenta si nunca hacemos un cambio de escala DURANTE el salto
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    
    // *** CONTROL DE TRANSICIÓN DE VELOCIDAD en Y ***
    // Impulso aplicado para iniciar salto
    float jumpImpulse = 0.0f;
    // Cuánta velocidad sumamos/restamos en el eje Y en cada frame para PARADA GRADUAL
    float stopStepY = 0.0f;
    
    // *** VALORES BASE FRAMES *** (modificables desde GUI)
    // FRAMES DE PARADA GRADUAL
    int stopFrames;
    // SALTO MÍNIMO
    int minJumpFrames;
    // SALTO MÁXIMO
    int maxJumpFrames;
    
    // *** JUMP FRAMES ***
    // Frames restantes TOTALES para completar el salto (el delay, la subida, y la parada gradual)
    int framesRemainingJump;
    // Frames de dentro de movimientos TO_JUMP donde ya se eleva
    int impulseFrames;
    // Frames restantes PARADA gradual en JUMP_TO_FALL
    int framesRemainingJumpStop;
    // Frames restantes DELAY para iniciar el salto en _TO_JUMP
    int delayFramesRemainingJump;
    
    // *** VARIABLES DE CONTROL ***
    bool isWaitingJumpImpulse = false;
    bool isImpulsing = false;
    bool isMinJump = false;
    bool isJumpCutted = false;

    bool isStartingToFall = false;
    bool gravityOverride = false;
    
    bool isHanging = false;

    
    // *** MÉTODOS INTERNOS ***
    // Actualiza los valores de trabajo segun la escala
    void updateScaledPhysics();
    
    // Redondea a un decimal limpio
    float cleanFloat(float value);
};

