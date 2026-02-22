#pragma once
#include <memory>

#include "ofMain.h"

//NECESARIOS AQUI PARA LAS VARIABLES POR VALOR
#include "SpriteSheetManager.h"
#include "MovementManager.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "GUIManager.h"
#include "InputManager.h"


// Clase principal que gestiona la aplicación
class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
private:
    
    
    // *** VARIABLES POR VALOR (El "OBJETO REAL") ***
    
    // Gestor de hojas de sprites
    SpriteSheetManager spriteSheetManager;
    
    // Gestor de movimientos
    MovementManager movementManager;
    
    // Gestor de físicas
    PhysicsManager physicsManager;
    
    // Gestor de colisiones
    CollisionManager collisionManager;
    
    // Gestor de GUI
    GUIManager guiManager;
    
    // Gestor de entradas (teclado, ratón, etc.)
    InputManager inputManager;
};
