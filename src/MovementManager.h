#pragma once

#include "Movimiento.h"
#include "SpriteSheetManager.h"
#include "ofxJSON.h"
#include <memory>
#include <map>
#include <string>
#include <unordered_map>


// Clase que gestiona los movimientos en el juego
class MovementManager {
    
public:
    // Inicializa el gestor de movimientos desde un archivo JSON y asociando un gestor de hojas de sprites
    void setup(const std::string& filename, SpriteSheetManager& spriteSheetManager);
    
    // Actualiza el estado del MovementManager basado en el tiempo actual.
    void update(float currentTime);
    
    // Inicia el movimiento especificado por su nombre
    // Con parámetro opcional para la región inicial
    void playMovement(const std::string& movementName, int region = 0);

    // Maneja la transición de un movimiento a otro
    void handleTransition();
    
    // Actualiza el intervalo de fotogramas del movimiento desde la GUI
    void updateFrameIntervalFromGUI();
    
    // Determina si se debe actualizar la región en función del tiempo transcurrido y el intervalo de frame
    bool shouldUpdateRegion(float currentTime, float interval) const;
    
    //GETS
    // Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
    int getCurrentRow() const;
    // Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
    int getCurrentRegion() const;
    // Obtiene la siguiente región a la que debe llegar.
    int getNextRegionToGo() const;
    // Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global.
    float getCurrentMovementFrameInterval() const;
    // Obtiene el intervalo de fotogramas global
    float getFrameInterval() const;
    // Obtiene el intervalo de fotogramas para un movimiento específico
    float getMovementFrameInterval(const std::string& movementName) const;
    
    //SETS
    // Establece el intervalo de fotogramas global
    void setFrameInterval(float interval);
    // Establece el intervalo de fotogramas para un movimiento específico
    void setMovementFrameInterval(const std::string& movementName, float interval);
    
    //Puede que en el futuro veamos como mantener esto private y hacer sus gets i sets (habrá que buscar donde se usa en otras partes del código
    std::unique_ptr<Movement> currentMovement = nullptr;
    std::string currentMovementName = "IDLE";
    std::map<std::string, Movement> movements;
    
 
    
private:
    //Puede que en el futuro veamos como mantener esto private y hacer sus gets i sets (habrá que buscar donde se usa en otras partes del código
    //std::unique_ptr<Movement> currentMovement = nullptr;
    //std::string currentMovementName = "IDLE";
    //std::map<std::string, Movement> movements;
    
    int currentRegion = 0; // Región actual en la hoja de sprites
    int currentRow = 0; // Fila actual en la hoja de sprites
    bool waitingForTransition = false; // Indica si se está esperando una transición
    int targetRegion = -1; // Región objetivo para la transición
    float lastUpdateTime = 0.0f; // Última vez que se actualizó el movimiento
    float frameInterval = 0.1f; // Intervalo de fotogramas general
    
    // Carga los movimientos desde un archivo JSON
    void loadMovements(const std::string& filename);
        
    // Maneja la lógica de actualización de frames y transiciones
    void updateRegion();
    
};
