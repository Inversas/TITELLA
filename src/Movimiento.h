#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>


using namespace std;

// +++++++++++++ ENUM +++++++++++++
// Categoría semántica de una transición: describe de dónde "viene" el movimiento.
// Los movimientos base (IDLE, WALK, RUN) usan NONE.
enum class TransitionOrigin {
    NONE,          // Movimientos base: no son transición
    TO_IDLE,       // Frenadas: WALK_TO_IDLE_x, RUN_TO_IDLE_x
    TO_WALK,       // Entradas a WALK: IDLE_TO_WALK, RUN_TO_WALK_x
    TO_RUN,        // Entradas a RUN: IDLE_TO_RUN, WALK_TO_RUN_x
    WALK_TURN,     // Giros desde WALK: WALK_TURN_1, WALK_TURN_2
    RUN_TURN,      // Giros desde RUN: RUN_TURN_1, RUN_TURN_2
    IDLE_TURN_TO_IDLE,  // Giro simple desde IDLE
    IDLE_TURN_TO_RUN    // Giro desde IDLE con intención de correr
};
 

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
    map<int, string> jump_transitions;    // Mapa Puntos de Salida con el nombre de la transicion Objetivo
    bool isTransition;                    // Si este movimiento es una transición
    float frameInterval;                  // Tiempo entre frames en segundos de este movimiento
    int target_frame;                     // Frame donde debe empezar el siguiente movimiento
    TransitionOrigin originType;          // Categoría semántica: de dónde viene esta transición

    
    // *** CONSTRUCTOR ***
    Movement(
             string name = "",            // Nombre del movimiento
             string spriteSheetName = "", // Nombre del archivo de la imagen SpriteSheet
             int row = 0,                 // Fila en el sprite sheet
             int numRegions = 1,          // Número de fotogramas
             bool isTransition = false,   // Si es transición
             float frameInterval = -0.1f,
             int target_frame = 0,
             TransitionOrigin originType = TransitionOrigin::NONE
             );
    
    // *** DESTRUCTOR ***
    // En estos structs solo ensuciarían el código sin aportar beneficio real, ya que no usas new.
};
