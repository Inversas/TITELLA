#pragma once

#include "Movimiento.h"
#include "ofxJSON.h"
#include <memory>
#include <map>
#include <string>
#include <unordered_map>


class SpriteSheetManager;
class InputManager;
class PhysicsManager;
class CollisionManager;

// Estados lógicos del personaje
// Estos estados representan lo que el personaje está haciendo lógicamente, independientemente de la animación exacta.
// La diferencia entre Estado y Movimiento es como la diferencia entre "Querer ir a comprar" y "Caminar hacia la tienda".
enum class MovementState {
    IDLE,       // [ | ]
    WALKING,    // [ ~ ]
    RUNNING,    // [ ≈ ]
    TURNING,    // [ ↻ ] Cuando está ejecutando una animación de giro
    STOPPING    // [ ! ] Cuando está en una transición hacia IDLE
};



// Clase que gestiona los movimientos en el juego
class MovementManager {

//"Public" es lo que la clase hace por los demás.
public:
    
    // *** CONFIGURACIÓN Y CONEXIÓN ***
    // Inicializa el gestor de movimientos desde un archivo JSON, asociando un gestor de hojas de sprites y un gestor de entradas
    // Más abajo en private definimos el InputManager como puntero, porque necesitamos consultarlo constantemente.
    // Pero no definimos el SpriteSheetManager nos viene de una referencia en ofApp, no necesita guardar su contacto.
    void setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input, PhysicsManager& physics, CollisionManager& collision);
    
    // *** CICLO DE VIDA ***
    // Actualiza el estado del MovementManager basado en el tiempo actual.
    void update(float currentTime);
    
    
    

    // !!! TRADUCTOR !!!
    // Input --> Intención
    void updateIntent();
    
    // !!! JUEZ !!!
    // Switch de estados y llama a playMovement o handleTransition según convenga. Es la única que actualiza el esado.
    void updateState(MovementState targetState);
    
    
    // !!! EJECUTOR !!! //
    // Inicia el movimiento especificado por su nombre, con parámetro opcional para la región inicial. No decide estados
    void playMovement(const std::string& movementName, int region = 0);

    // !!! OTEADOR (mira al futuro) !!! //
    // Mira el JSON del movimiento actual.
    // Busca el transitionRegion (punto de salida) más cercano.
    // Activa la bandera waitingForTransition = true.
    // No hace nada más. Se queda esperando a que el "reloj" la avise.
    void handleTransition();
    
    
    

    // *** HANDLE SPECIFIC MOVEMENTS *** //
    // Funciones específicas para manejar transiciones de movimientos
    void handleWalkToRun();
    void handleRunToWalk();

    
    //*** GETS REGION ***//
    // Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
    int getCurrentRow() const;
    // Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
    int getCurrentRegion() const;
    // Obtiene la siguiente región a la que debe llegar.
    int getNextOutRegion() const;
    
    //*** GETS FRAME INTERVAL ***//
    // Obtiene el intervalo de fotogramas global
    float getFrameInterval() const;
    // Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global. (en public porque la GUI la usa)
    float getCurrentMovementFrameInterval() const;
    // Obtiene el intervalo de fotogramas para un movimiento específico
    float getMovementFrameInterval(const std::string& movementName) const;
    
    //*** GETS DIRECTION ***//
    // Obtiene la dirección del personaje
    bool getIsFacingRight();
    
    //*** GETS MOVEMENT ***//
    // Obtiene el movimiento actual
    // El primer const significa: "El puntero que devuelvo es constante (no puedes modificar lo que apunta)."
    // El último const significa: "Esta función no modifica el estado interno del MovementManager."
    const Movement* getCurrentMovement() const;
    // Obtiene el movimiento actual
    std::string getCurrentMovementName() const;
    // Obtiene el mapa de todos los movimientos disponibles
    const std::map<std::string, Movement>& getMovements() const;
    
    //*** GETS STATE ***//
    // Retorna un string con el estado actual del personaje
    std::string getCurrentState() const;
    // Retorna un string con el estado objetivo del personaje
    std::string getTargetState() const;
    // Retorna true si está esperando una transición, false en caso contrario
    bool isWaitingForTransition() const;
    

    //*** SETS FRAME INTERVAL ***//
    // Actualiza el intervalo de fotogramas del movimiento desde la GUI (NO IMPEMENTADA
    void setFrameIntervalFromGUI();
    // Establece el intervalo de fotogramas global
    void setFrameInterval(float interval);
    // Establece el intervalo de fotogramas para un movimiento específico
    void setMovementFrameInterval(const std::string& movementName, float interval);
    
    //*** SETS DIRECTION ***//
    // Invierte la dirección del personaje
    void toggleIsFacingRight();
    
    //*** SETS STATE ***//
    //void setCurrentState(MovementState state);
    //void setTargetState(MovementState state);
    
//"Private" es cómo la clase se organiza a sí misma para hacer su trabajo.
private:
    // *** VARIABLES DE ESTADO INTERNO ***
    int currentRegion = 0; // Región actual en la hoja de sprites
    int currentRow = 0; // Fila actual en la hoja de sprites
    bool waitingForTransition = false; // Indica si se está esperando una transición
    int targetRegion = -1; // Región objetivo para la transición
    float lastUpdateTime = 0.0f; // Última vez que se actualizó el movimiento
    float frameInterval = 0.1f; // Intervalo de fotogramas general
    bool isFacingRight = true;  //Dirección del personaje
    
    
    // *** CONTEXTOS DE MOVIMIENTO, ESTADO y INPUT ***
    // Puntero único que almacena el movimiento actual (null por defecto).
    std::unique_ptr<Movement> currentMovement = nullptr;
    // Mapa que contiene todos los movimientos disponibles. Las claves son strings representando los nombres.
    std::map<std::string, Movement> movements;
    // Variable para almacenar el nombre del movimiento actual.
    std::string currentMovementName;
    // Variable para guardar el estado actual (inicializado en IDLE).
    MovementState currentState = MovementState::IDLE;
    // Variable para guardar el estado objetivo (inicializado en IDLE)
    MovementState targetState = MovementState::IDLE;
    
    // Puntero al gestor de entradas
    InputManager* inputManager = nullptr;
    // Puntero al gestor de físicas
    PhysicsManager* physicsManager = nullptr;
    // Puntero al gestor de Sprites
    SpriteSheetManager* spriteSheetManager = nullptr;
    // Puntero al gestor de Colisiones
    CollisionManager* collisionManager = nullptr;
    
    // *** LÓGICA INTERNA (LOS ENGRANAJES) ***
    // Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
    bool shouldUpdateRegion(float currentTime, float interval) const;
    // Carga los movimientos desde un archivo JSON
    void loadMovements(const std::string& filename);
 
    
    
    // !!! MOTOR !!! //
    //Es private porque No quieres que desde ofApp.cpp alguien llame por error a movementManager.updateRegion().
    //Si lo hicieran, los frames podrían avanzar el doble de rápido o saltar cuando no deben, rompiendo la animación.
    //Como es privado, sabes que nadie fuera de esa clase depende de esa función.
    // Es el motor, como un cronometro.
    void updateRegion();
    void triggerTransition();    
    void finishedTransition();
   
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    void handleMovementPhysics(const std::string& name);
    
    
    // !!! AÑADIR EN EL FUTURO !!! SI HACE FALTA
    //bool canInterrupt() const; // Para saber si estamos en un estado flexible
    
    
    // TURN_TO_RUN solo se puede dar desde IDLE, pero el estado es TURNING
    // Para no añadir un estado nuevo, que además entraría en conflicto con RUN_TO_TURN
    // Creamos este flag que solo se evaluará en currentState IDLE con targetState RUN.
    bool flag_turn_to_run = false;
    
    // Si esto empieza a darse tendremo que crear el Objeto Peticióni modificar el TRADUCTOR y el JUEZ en consecuencia.
    
    /*
     struct MovementRequest {
         MovementState state;
         std::string variant = ""; // Aquí el traductor puede decir "TO_RUN"
     };
     */
    
};
