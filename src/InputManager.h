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
    
    
private:
    // Puntero al gestor de movimientos
    MovementManager* movementManager;
    
    std::set<int> pressedKeys; // Conjunto de teclas actualmente presionadas
};
