#pragma once

#include "MovementManager.h"

//yo no lo tenía y funcionaba igual
//#include <set>      // Necesario para std::set

//de momento lo comento ya veremos si hace falta
//#include <vector> // Necesario para la pila de direcciones

//*** ESTRUCTURA DE ESTADO DE ENTRADA ***//
// Contiene la "foto" de las intenciones del jugador en un momento dado.
struct InputState {
    bool wantsLeft = false;         // El jugador quiere ir a la izquierda
    bool wantsRight = false;        // El jugador quiere ir a la derecha
    bool wantsRun = false;          // El jugador quiere correr
    bool hasAnyDirection = false;   // Indica si hay alguna dirección activa (útil para STAND)
};


//*** CLASE INPUT MANAGER ***//
// Clase que gestiona la entrada del usuario (por ejemplo, el teclado)
class InputManager {
    
public:
    
    
    // !!! No necesitamos la referencia al MovementManager !!! //
    /*
    // Inicia el gestor de entradas con una referencia al gestor de movimientos
    void setup(MovementManager& movementManager);
    */
    
    // Inicia el gestor de entradas
    void setup();
    
    // Maneja la pulsación de una tecla (llamado desde ofApp)
    void keyPressed(int key);
    
    // Maneja la liberación de una tecla (llamado desde ofApp)
    void keyReleased(int key);
    
    // Devuelve el estado actual de las intenciones (lo consulta MovementManager)
    InputState getState() const;
    
    
    
    //*** GETS GUI ***
    //Consultar teclas presionadas
    std::string getPressedKeysAsString() const;
    //Consultar Intenciones
    std::string getIntentsAsString() const;
    
private:
    
    /*
    // Puntero al gestor de movimientos
    MovementManager* movementManager;
    */
    
    // Traduce las teclas físicas y la pila a intenciones (InputState)
    void updateInputState();
    
    
    // Conjunto de todas las teclas físicas pulsadas actualmente (no sabe quien llego ultimo)
    //Si el sistema operativo te envía 10 veces la señal de que la tecla '1' está pulsada (porque la dejas mantenida), el set solo guarda una.
    std::set<int> pressedKeys;
        
    // Pila cronológica de direcciones para decidir cuál es la última pulsada (es tonto buscando, pero sabe quien llego último)
    std::vector<int> directionStack;
    
    // Almacena las intenciones traducidas (la "caja" de estados)
    InputState currentInputState;
};
