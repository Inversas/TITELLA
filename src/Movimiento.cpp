#include "Movimiento.h"

// Constructor para inicializar un movimiento
Movement::Movement(
             string name,             // Nombre del movimiento
             int row,                 // Fila en el sprite sheet
             int numRegions,          // Número de fotogramas
             bool isTransition,       // Si es transición
             float frameInterval,     // Velocidad predeterminada
             set<string> keys         // Teclas asociadas
             ) : name(name), row(row), numRegions(numRegions),
    isTransition(isTransition), frameInterval(frameInterval), keys(keys) {}
