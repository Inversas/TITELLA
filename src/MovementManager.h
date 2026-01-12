#pragma once

#include "Movimiento.h"
#include "ofxJSON.h"
#include <memory>
#include <map>
#include <string>
#include <unordered_map>


class SpriteSheetManager;
class InputManager;


// Estados lógicos del personaje
// Estos estados representan lo que el personaje está haciendo lógicamente, independientemente de la animación exacta.
// La diferencia entre Estado y Movimiento es como la diferencia entre "Querer ir a comprar" y "Caminar hacia la tienda".
enum class MovementState {
    IDLE,
    WALKING,
    RUNNING,
    TURNING,    // Cuando está ejecutando una animación de giro
    STOPPING    // Cuando está en una transición hacia IDLE
};



// Clase que gestiona los movimientos en el juego
class MovementManager {

//"Public" es lo que la clase hace por los demás.
public:
    
    // *** CONFIGURACIÓN Y CONEXIÓN ***
    // Inicializa el gestor de movimientos desde un archivo JSON, asociando un gestor de hojas de sprites y un gestor de entradas
    // Más abajo en private definimos el InputManager como puntero, porque necesitamos consultarlo constantemente.
    // Pero no definimos el SpriteSheetManager nos viene de una referencia en ofApp, no necesita guardar su contacto.
    void setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input);
    
    // *** CICLO DE VIDA ***
    // Actualiza el estado del MovementManager basado en el tiempo actual.
    void update(float currentTime);
    
    
    
    
    // !!! CEREBRO !!!
    // Esta será la función estrella que decide qué hacer según el Input
    void updateState();
    
    // !!! PLAY MOVEMENTS !!! //
    // Inicia el movimiento especificado por su nombre, con parámetro opcional para la región inicial
    void playMovement(const std::string& movementName, int region = 0);

    // !!! HANDLE TRANSITIONS !!! //
    // Maneja la transición de un movimiento a otro
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
    // Puntero al gestor de entradas
    InputManager* inputManager = nullptr;
    
    
    // *** LÓGICA INTERNA (LOS ENGRANAJES) ***
    // Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
    bool shouldUpdateRegion(float currentTime, float interval) const;
    // Carga los movimientos desde un archivo JSON
    void loadMovements(const std::string& filename);
 
    
    
    
    
    // !!! UPDATE REGION !!! //
    //Es private porque No quieres que desde ofApp.cpp alguien llame por error a movementManager.updateRegion().
    //Si lo hicieran, los frames podrían avanzar el doble de rápido o saltar cuando no deben, rompiendo la animación.
    //Como es privado, sabes que nadie fuera de esa clase depende de esa función.

    // Maneja la lógica de actualización de frames y transiciones
    void updateRegion();
    
   
    
    // !!! AÑADIR EN EL FUTURO !!! SI HACE FALTA
    //bool canInterrupt() const; // Para saber si estamos en un estado flexible
    
    
    
    
    // *** MAPAS DE TRANSICIÓN (ESTÁTICOS) ***
    // Mapa de transiciones de regiones
    std::map<int, int> runToWalkRegionMap = {
        {0, 5},
        {1, 6},
        {2, 7},
        {3, 7},
        {4, 0},
        {5, 1},
        {6, 1},
        {7, 3},
        {8, 3},
        {9, 4},
        {10, 4},
        {11, 5}
    };
    
    // Mapa de transiciones de regiones para WALK a RUN
    std::map<int, int> walkToRunRegionMap = {
        {0, 6},
        {1, 7},
        {2, 10},
        {3, 11},
        {4, 0},
        {5, 1},
        {6, 4},
        {7, 5}
    };
};
