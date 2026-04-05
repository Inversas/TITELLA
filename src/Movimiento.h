#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>


using namespace std;

// +++++++++++++ STRUCT +++++++++++++
// Estructura para definir un movimiento
struct Movement {
    string name;                          // Nombre del movimiento (ej. "WALK", "IDLE")
    string spriteSheetName;               // Nombre del Archivo (Imagen.png) del movimiento
    int row;                              // Fila en el sprite sheet de este movimiento
    int numRegions;                       // Duración en regiones o fotogramas de este movimiento
    map<int, string> stop_transitions;    // Mapa Puntos de Salida con el nombre de la transicion Objetivo
    map<int, string> turn_transitions;    // Mapa Puntos de Salida con el nombre de la transicion Objetivo
    map<int, string> change_transitions;  // Mapa Puntos de Salida con el nombre de la transicion Objetivo
    bool isTransition;                    // Si este movimiento es una transición
    float frameInterval;                  // Tiempo entre frames en segundos de este movimiento
    int target_frame;                     // Frame donde debe empezar el siguiente movimiento
    
    // *** CONSTRUCTOR ***
    Movement(
             string name = "",            // Nombre del movimiento
             string spriteSheetName = "", // Nombre del archivo de la imagen SpriteSheet
             int row = 0,                 // Fila en el sprite sheet
             int numRegions = 1,          // Número de fotogramas
             bool isTransition = false,   // Si es transición
             float frameInterval = -0.1f,
             int target_frame = 0
             );
    
    // *** DESTRUCTOR ***
    // En estos structs solo ensuciarían el código sin aportar beneficio real, ya que no usas new.
};
