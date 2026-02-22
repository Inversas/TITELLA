#pragma once
#include <vector>

#include "ofMain.h"

#include "SpriteSheetManager.h"
#include "Interactor.h"


using namespace std;

// Clase que gestiona las colisiones en el juego
class CollisionManager {

public:
    // Inicializa los límites y el suelo
    void setup();
    
    void update();

    // Dibujará las líneas de colisión para debug
    void draw();
    
    // Retorna un puntero al Interactor chocado, o nullptr si no hay nada
    Interactor* checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, SpriteSheetManager& sprite, bool isFacingRight);
    
    
    
private:
    // Lista donde guardamos todos los suelos, paredes y botones
    vector<Interactor> interactors;
    
    
    
        
};
