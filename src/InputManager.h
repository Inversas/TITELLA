#pragma once

#include "MovementManager.h"

//Esto comentado y funcionaba igual
//#include <set>      // Necesario para std::set

//De momento lo comento ya veremos si hace falta
//#include <vector> // Necesario para la pila de direcciones


//*** ESTRUCTURA DE ESTADO DE ENTRADA (WANT / INTENCION) ***//
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
    
    // Inicia el Gestor de Entradas
    void setup();
    
    // Maneja la pulsación de una tecla (llamado desde ofApp)
    void keyPressed(int key);
    
    // Maneja la liberación de una tecla (llamado desde ofApp)
    void keyReleased(int key);
    
    //*** GET ESTADO DE ENTRADA ACTUAL (WANT) ***
    // Devuelve el estado de entrada actual (WANT) de las intenciones (lo consultará el MovementManager)
    InputState getInputState() const;
    
    
    
    //*** GETS EXCLUSIVOS PARA GUI ***
    //Consultar teclas presionadas
    std::string getPressedKeysAsString() const;
    //Consultar Intenciones (WANT)
    std::string getIntentsAsString() const;
    
private:
   
  
    
    // Conjunto de todas las teclas físicas pulsadas actualmente (no sabe quien llego ultimo)
    //Si el sistema operativo te envía 10 veces la señal de que la tecla '1' está pulsada (porque la dejas mantenida), el set solo guarda una.
    std::set<int> pressedKeys;
        
    // Pila cronológica de direcciones para decidir cuál es la última pulsada (es tonto buscando, pero sabe quien llego último)
    std::vector<int> directionStack;
    
    // Almacena las intenciones traducidas (la "caja" de estados)
    InputState currentInputState;
    
    // Traduce las teclas físicas y la pila a intenciones (InputState)
    void updateInputState();
};
