#pragma once

#include "ofMain.h"
#include "SpriteSheetManager.h"
#include "MovementManager.h"
#include "GUIManager.h"
#include "InputManager.h"
#include <memory>

// Clase principal que gestiona la aplicación
class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
private:
    
    // Gestor de hojas de sprites
    SpriteSheetManager spriteSheetManager;
    
    // Gestor de movimientos
    MovementManager movementManager;
    
    // Gestor de GUI
    GUIManager guiManager;
    
    // Gestor de entradas (teclado, ratón, etc.)
    InputManager inputManager;
};
