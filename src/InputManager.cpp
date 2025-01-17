#include "InputManager.h"

// Iniciar el InputManager
void InputManager::setup(MovementManager& movementManager) {
     // Asigna el gestor de movimientos al puntero miembro de la clase
    this->movementManager = &movementManager;

}

void InputManager::keyPressed(int key) {
    
    //pressedKeys: Se utiliza principalmente para verificar el estado de varias teclas en cualquier momento, especialmente cuando se necesita saber si una combinación de teclas está activa.
    
    // Agrega la tecla al conjunto de teclas presionadas
    pressedKeys.insert(key);

    
    // Si la tecla presionada es la tecla 1 y la flecha no está ya presionada, establecer isKey1PressedBeforeRight a true
    if (key == 49 && !pressedKeys.count(OF_KEY_RIGHT)) {
        isKey1PressedBeforeRight = true;
    }
    
    // Activar RUN con combinación de teclas
    activateRunWithKeys();
    
    // Transición de WALK a RUN
    transitionWalkToRun();

    // Si se está ejecutando RUN o alguna de sus transiciones
    //if (movementManager->currentMovementName == "RUN" || movementManager->currentMovement->isTransition) {
    // Si se está ejecutando RUN
    if (movementManager->currentMovementName == "RUN") {
        // Transición de RUN a WALK
        transitionRunToWalk();
        return;
    }

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
    
    //key: Se utiliza para reaccionar a la acción inmediata de presionar o liberar una tecla específica.
    
    // Elimina la tecla del conjunto de teclas presionadas
    pressedKeys.erase(key);
    
    // Si se libera la tecla 1, restablecer isKey1PressedBeforeRight
    if (key == 49) {
        isKey1PressedBeforeRight = false;
    }
 
    // Verifica:
        //si hay un movimiento actual
        //si no es una transición
        //Y si la tecla liberada coincide con la tecla del movimiento actual
    if (movementManager->currentMovement && !movementManager->currentMovement->isTransition && movementManager->currentMovement->key == key) {
        // Maneja la transición del movimiento
        movementManager->handleTransition();
    }
    
    // Si se libera la tecla 1 mientras se está en RUN y la flecha derecha sigue presionada, pasar a WALK
    transitionRunToWalkRelease(key);

    // Si se libera la flecha derecha mientras se está en RUN pero no la tecla 1, manejar la transición de RUN
    transitionRun(key);
}



//MANEJOS ESPECIALES DE TECLAS Y MOVIMIENTOS PRESSED

//Activa el movimiento "RUN" con combinación 1 y OF_KEY_RIGHT
void InputManager::activateRunWithKeys() {
    // Verificar si ambas teclas (1 y OF_KEY_RIGHT) están presionadas y si la tecla 1 se presionó antes que la flecha derecha
    if (pressedKeys.count(49) && pressedKeys.count(OF_KEY_RIGHT) && isKey1PressedBeforeRight) {
       // Si RUN no es el movimiento actual
        if (movementManager->currentMovementName != "RUN") {
            //Activa RUN
            movementManager->playMovement("RUN");
        }
    }
}

//Gestiona Transción de WALK a RUN
void InputManager::transitionWalkToRun() {
    // Verificar si el movimiento actual es WALK y se presiona la tecla 1
    if (movementManager->currentMovementName == "WALK" && pressedKeys.count(49)) {
        // Obtener la región actual de WALK
        int currentRegion = movementManager->getCurrentRegion();
        // Obtener la región de RUN correspondiente
        int runRegion = walkToRunRegionMap[currentRegion];
        // Activar el movimiento RUN con la región correspondiente
        movementManager->playMovement("RUN", runRegion);
    }
}

//Gestiona transición de RUN a WALK
void InputManager::transitionRunToWalk() {
    // Verificar si el movimiento actual es RUN y se presiona la flecha derecha sin la tecla 1
    if (movementManager->currentMovementName == "RUN" && !pressedKeys.count(49) && pressedKeys.count(OF_KEY_RIGHT)) {
        // Obtener la región actual de RUN
        int currentRegion = movementManager->getCurrentRegion();
        // Obtener la región de WALK correspondiente
        int walkRegion = runToWalkRegionMap[currentRegion];
        // Activar el movimiento WALK con la región correspondiente
        movementManager->playMovement("WALK", walkRegion);
    }
}


//MANEJOS ESPECIALES DE TECLAS Y MOVIMIENTOS RELEASED

//Gestiona transición de RUN a WALK
void InputManager::transitionRunToWalkRelease(int key) {
    // Si se libera la tecla 1 mientras se está en RUN y la flecha derecha sigue presionada
    if (key == 49 && movementManager->currentMovementName == "RUN" && pressedKeys.count(OF_KEY_RIGHT)) {
        // Obtener la región actual de RUN
        int currentRegion = movementManager->getCurrentRegion();
        // Obtener la región de WALK correspondiente
        int walkRegion = runToWalkRegionMap[currentRegion];
        // Activar el movimiento WALK con la región correspondiente
        movementManager->playMovement("WALK", walkRegion);
        // Restablecer isKey1PressedBeforeRight
        isKey1PressedBeforeRight = false;
    }
}

// Salir de RUN, en situación de tecla concreta
void InputManager::transitionRun(int key) {
    // Si se libera la flecha derecha mientras se está en RUN pero no la tecla 1
    if (key == OF_KEY_RIGHT && movementManager->currentMovementName == "RUN" && pressedKeys.count(49)) {
        // Maneja la transición del movimiento RUN
        movementManager->handleTransition();
    }
}
