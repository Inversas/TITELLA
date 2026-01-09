#include "InputManager.h"

// Iniciar el InputManager
void InputManager::setup() {
    // Limpiamos el conjunto de teclas y el conjunto de direcciones por seguridad al iniciar
    pressedKeys.clear();
    directionStack.clear();
}


//*** EVENTOS DE TECLADO (DESDE OFAPP) ***//
void InputManager::keyPressed(int key) {
    
    // 1. Gestión del Vector de Direcciones
    
    //Si se ha pulsado una tecla de dirección la añadimos al vector
    if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
        // Solo añadimos al vector si no estaba ya (evita repeticiones del SO)
        if (pressedKeys.find(key) == pressedKeys.end()) {
            // La metemos al final del vector
            directionStack.push_back(key);
        }
    }

    // 2. Se inserta la tecla en el conjunto de teclas presionadas
    pressedKeys.insert(key);
    
    // 3. Actualizamos la "foto" de intenciones o "cajita" de estados lógicos
    updateInputState();
}


void InputManager::keyReleased(int key) {
    
    // 1. Limpieza del Vector de Direcciones
    // Si se ha liberado una tecla de dirección la eliminamos del vector
    if (key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
        // Buscamos y eliminamos la tecla específica del vector
        // Para buscar en un vector hay que recorrerlo
        for (auto it = directionStack.begin(); it != directionStack.end(); ) {
            // Es lo que buscamos: borramos
            // Al ponerle el asterisco delante, le dices al programa: "No me des la posición, dame el VALOR que hay dentro de esa posición"
            if (*it == key) {
                //El comando erase te dice: "Oye, he borrado lo que me pediste; ahora el siguiente elemento está en esta misma posición".
                it = directionStack.erase(it);
            // No es lo que buscamos: seguimos
            // Sigue aunque ya la haya encontrado, así evitamos posibles duplicados
            } else {
                ++it;
            }
        }
    }
    
    // 2. Se elimina la tecla del conjunto al liberarla
    pressedKeys.erase(key);
    
    // 3. Actualizamos la "foto" de intenciones o la "cajita" de estados lógicos
    updateInputState();
}


//*** LÓGICA DE TRADUCCIÓN DE ENTRADA ***//
void InputManager::updateInputState() {
    
    // 1. RESETEO: Ponemos todo a false antes de evaluar
    currentInputState.wantsLeft = false;
    currentInputState.wantsRight = false;
    currentInputState.wantsRun = false;
    currentInputState.hasAnyDirection = false;

    
    // 2. EVALUACIÓN DE DIRECCIÓN: La última tecla en el vector tiene la prioridad
    
    //Si hay algo en el vector de direcciones
    if (!directionStack.empty()) {
        int lastKey = directionStack.back(); // Miramos el final del vector, lo ultimo que ha entrado
        
        if (lastKey == OF_KEY_LEFT) {
            currentInputState.wantsLeft = true;
        }
        else if (lastKey == OF_KEY_RIGHT) {
            currentInputState.wantsRight = true;
        }
        
        //Si entramos aquí es que hay dirección
        currentInputState.hasAnyDirection = true;
    }
    
    // 3. TRADUCCIÓN: Mapeamos teclas físicas a intenciones lógicas
    if (pressedKeys.count(49)) {
        currentInputState.wantsRun = true;
    }


}


//*** ACCESO AL ESTADO DESDE EL EXTERIOR ***//
InputState InputManager::getState() const {
    // Retornamos la estructura con los booleanos para que el MovementManager los lea
    return currentInputState;
}




