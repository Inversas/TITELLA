#pragma once

#include "ofMain.h"
#include "Interactor.h"
#include <vector>

using namespace std;

// Clase que gestiona las colisiones en el juego
class CollisionManager {

public:
    // Inicializa los límites y el suelo
    void setup();
    
    void update();

    // Dibujará las líneas de colisión para debug
    void draw();
    
private:
    // Lista donde guardamos todos los suelos, paredes y botones
    vector<Interactor> interactors;
        
};
