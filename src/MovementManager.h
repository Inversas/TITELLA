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
    
public:
    
    // *** CONFIGURACIÓN Y CONEXIÓN ***
    // Inicializa el gestor de movimientos desde un archivo JSON, asociando un gestor de hojas de sprites y un gestor de entradas
    // Más abajo en private definimos el InputManager como puntero, porque necesitamos consultarlo constantemente.
    // Pero no definimos el SpriteSheetManager nos viene de una referencia en ofApp, no necesita guardar su contacto.
    void setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input);
    
    // *** CICLO DE VIDA ***
    // Actualiza el estado del MovementManager basado en el tiempo actual.
    void update(float currentTime);
    
    
    // *** CEREBRO ***
    // Esta será la función estrella que decide qué hacer según el Input
    void updateState();
    
    //*** HANDLE MOVEMENTS ***//
    // Inicia el movimiento especificado por su nombre, con parámetro opcional para la región inicial
    void playMovement(const std::string& movementName, int region = 0);

    // Maneja la transición de un movimiento a otro
    void handleTransition();

    // Funciones específicas para manejar transiciones de movimientos
    void handleWalkToRun();
    void handleRunToWalk();
    
    
    // !!! No necesitamos las funciones ascociadas al controlKeys  !!! //
    /*
    // *** HANDLE CONTROL KEYS *** //
    void handleControlKeys();
    
    void cleanControlKeys();
    // GET para la variable de tipo string
    void setControlKeys(const std::string& value);
    // SET para la variable de tipo string
    std::string getControlKeys() const;
    */
    
    
    
    // Actualiza el intervalo de fotogramas del movimiento desde la GUI
    void updateFrameIntervalFromGUI();
    
    
    //*** GETS ***//
    // Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
    int getCurrentRow() const;
    // Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
    int getCurrentRegion() const;
    // Obtiene la siguiente región a la que debe llegar.
    int getNextRegionToGo() const;
    // Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global. (en public porque la GUI la usa)
    float getCurrentMovementFrameInterval() const;
    // Obtiene el intervalo de fotogramas global
    float getFrameInterval() const;
    // Obtiene el intervalo de fotogramas para un movimiento específico
    float getMovementFrameInterval(const std::string& movementName) const;
    // Obtiene la dirección del personaje
    bool getIsFacingRight();
    
    //Añadir para poder poner el currentName en private
    //std::string getCurrentMovementName() const { return currentMovementName; }
    
    
    //*** SETS ***//
    // Establece el intervalo de fotogramas global
    void setFrameInterval(float interval);
    // Establece el intervalo de fotogramas para un movimiento específico
    void setMovementFrameInterval(const std::string& movementName, float interval);
    // Invierte la dirección del personaje
    void toggleIsFacingRight();

    
    
    //ESTA EN PUBLIC PARA LA GUI?
    //Puede que en el futuro veamos como mantener esto private y hacer sus gets i sets (habrá que buscar donde se usa en otras partes del código
    std::unique_ptr<Movement> currentMovement = nullptr;
    std::map<std::string, Movement> movements;
    std::string currentMovementName;
    // Variable para guardar el estado actual
    MovementState currentState = MovementState::IDLE;
    
    // !!! DEBERÍA IR EN PRIVATE !!!
    // Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
    bool shouldUpdateRegion(float currentTime, float interval) const;
    
    
    
    //Get Nombre del movimiento actual
    //std::string getCurrentMovementName() const;
    //Set Nombre del movimiento actual
    //void setCurrentMovementName(const std::string& movementName);
    
private:
    // *** DATOS "SAGRADOS" (PROTEGIDOS) ***
    //Puede que en el futuro veamos como mantener esto private y hacer sus gets i sets (habrá que buscar donde se usa en otras partes del código
    //std::unique_ptr<Movement> currentMovement = nullptr;
    //std::map<std::string, Movement> movements;
    
    // Puntero al gestor de entradas
    InputManager* inputManager = nullptr;

    
    
    // *** VARIABLES DE ESTADO INTERNO ***
    int currentRegion = 0; // Región actual en la hoja de sprites
    int currentRow = 0; // Fila actual en la hoja de sprites
    bool waitingForTransition = false; // Indica si se está esperando una transición
    int targetRegion = -1; // Región objetivo para la transición
    float lastUpdateTime = 0.0f; // Última vez que se actualizó el movimiento
    float frameInterval = 0.1f; // Intervalo de fotogramas general
    
    //Dirección del personaje
    bool isFacingRight = true;
    
    
    // !!! No necesitamos el controlKeys  !!! //
    // Representación de teclas actualmente presionadas
    //std::string controlKeys;

    
    // *** LÓGICA INTERNA (LOS ENGRANAJES) ***
    // Carga los movimientos desde un archivo JSON
    void loadMovements(const std::string& filename);
    
    //Es pprivate porqu No quieres que desde ofApp.cpp alguien llame por error a movementManager.updateRegion().
    //Si lo hicieran, los frames podrían avanzar el doble de rápido o saltar cuando no deben, rompiendo la animación.
    //Como es privado, sabes que nadie fuera de esa clase depende de esa función.
    //"Public" es lo que la clase hace por los demás.
    //"Private" es cómo la clase se organiza a sí misma para lograrlo.
    // Maneja la lógica de actualización de frames y transiciones
    void updateRegion();
    
    
    //AÑADIR EN EL FUTURO
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
