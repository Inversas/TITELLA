#include "InputManager.h"

// Iniciar el InputManager
void InputManager::setup(MovementManager& movementManager) {
     // Asigna el gestor de movimientos al puntero miembro de la clase
    this->movementManager = &movementManager;
}

void InputManager::keyPressed(int key) {
    // Recorre todos los movimientos gestionados por movementManager
    for (auto& pair : movementManager->movements) {
        
        // Obtiene la referencia al movimiento actual
        Movement& movement = pair.second;
        
        // Verifica:
            //si el movimiento no es una transición
            //si la tecla presionada coincide
            //Y si el movimiento actual no es el mismo
        if (!movement.isTransition && movement.key == key && movementManager->currentMovementName != movement.name) {
            // Reproduce el movimiento correspondiente
            movementManager->playMovement(movement.name);
            // Finaliza la función después de encontrar y reproducir un movimiento
            return;
        }
    }
    
    // Aquí puede ir Lógica adicional para manejo de teclas si es necesario

}

void InputManager::keyReleased(int key) {
    // Verifica:
        //si hay un movimiento actual
        //si no es una transición
        //Y si la tecla liberada coincide con la tecla del movimiento actual
    if (movementManager->currentMovement && !movementManager->currentMovement->isTransition && movementManager->currentMovement->key == key) {
        // Maneja la transición del movimiento
        movementManager->handleTransition();
    }
}
