#include "Movimiento.h"
/*
// Constructor para inicializar un movimiento
Movement::Movement(
             string name,             // Nombre del movimiento
             string spriteSheetName,  // Nombre del Archivo (Imagen.png) del movimiento
             int row,                 // Fila en el sprite sheet
             int numRegions,          // Número de fotogramas
             bool isTransition,       // Si es transición
             float frameInterval,     // Velocidad predeterminada
             int target_frame         // Frame donde debe empezar el siguiente movimiento, por defecto 0
             ) : name(name),spriteSheetName(spriteSheetName), row(row), numRegions(numRegions),
    isTransition(isTransition), frameInterval(frameInterval), target_frame(target_frame) {}
*/



 // Constructor para inicializar un movimiento
 Movement::Movement(
              string name,             // Nombre del movimiento
              string spriteSheetName,  // Nombre del Archivo (Imagen.png) del movimiento
              int row,                 // Fila en el sprite sheet
              int numRegions,          // Número de fotogramas
              bool isTransition,       // Si es transición
              float frameInterval,     // Velocidad predeterminada
              int target_frame,         // Frame donde debe empezar el siguiente movimiento, por defecto 0
              TransitionOrigin originType // Categoría semántica: de dónde viene esta transición
              ) : name(name),spriteSheetName(spriteSheetName), row(row), numRegions(numRegions),
     isTransition(isTransition), frameInterval(frameInterval), target_frame(target_frame), originType(originType) {}
 
