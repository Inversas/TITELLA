#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

// Estructura para definir un movimiento
struct Movement {
    string name; // Nombre del movimiento (ej. "WALK", "IDLE")
    int row;     // Fila en el sprite sheet donde están los fotogramas de este movimiento
    int numRegions; // Número de regiones o fotogramas en esta fila (cuántos cuadros animan el movimiento)
    vector<int> transitionRegions; // Regiones específicas para iniciar transiciones al dejar de presionar la tecla
    map<int, string> transitions;  // Mapa que asocia una región específica con el nombre de una transición
    bool isTransition; // Si el movimiento es una transición (no se reproduce en bucle)
    float frameInterval; // Tiempo entre frames en segundos (específico para el movimiento)
    int key; // Tecla asociada al movimiento (almacena códigos como `OF_KEY_RIGHT`)
    
    // Constructor para inicializar un movimiento
    Movement(
             string name = "",          // Nombre del movimiento
             int row = 0,               // Fila en el sprite sheet
             int numRegions = 1,        // Número de fotogramas
             bool isTransition = false, // Si es transición
             float frameInterval = -0.1f, // Velocidad predeterminada
             int key = 0                // Tecla asociada
             );
};
