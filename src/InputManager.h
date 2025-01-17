#pragma once

#include "MovementManager.h"

// Clase que gestiona la entrada del usuario (por ejemplo, el teclado)
class InputManager {
    
public:
    
    // Inicia el gestor de entradas con una referencia al gestor de movimientos
    void setup(MovementManager& movementManager);
    
    // Maneja la pulsación de una tecla
    void keyPressed(int key);
    
    // Maneja la liberación de una tecla
    void keyReleased(int key);
    
    // Método para obtener la última tecla presionada
    int getLastKeyPressed() const;
    
private:
    // Puntero al gestor de movimientos
    MovementManager* movementManager;
    
    std::set<int> pressedKeys; // Conjunto de teclas actualmente presionadas
    bool isKey1PressedBeforeRight = false; // Para rastrear si la tecla 1 se presionó antes que la flecha derecha
    
    
    // Declaraciones de las funciones auxiliares para manejar transiciones específicas PRESSED
    void activateRunWithKeys();
    void transitionWalkToRun();
    void transitionRunToWalk();
    
    // Declaraciones de las funciones auxiliares para manejar transiciones específicas RELEASED
    void transitionRunToWalkRelease(int key);
    void transitionRun(int key);
    
    
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
