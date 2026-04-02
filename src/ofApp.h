#pragma once
#include <memory>

#include "ofMain.h"

// NECESARIOS ASÍ PARA LAS VARIABLES POR VALOR
#include "SpriteSheetManager.h"
#include "MovementManager.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "GUIManager.h"
#include "InputManager.h"
#include "EditorManager.h"


// Clase principal que gestiona la aplicación
class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    
    // *** SETS  MENU ***
    void setToggleDebug(bool value);
    void setToggleShowInteractors(bool value);
    void setToggleEditMode(bool value);
    
private:
    // *** VARIABLES POR VALOR (El "OBJETO REAL") ***

    // Gestor de entradas (teclado) £££££££££££££
    InputManager inputManager;
    
    // Gestor de hojas de sprites &&&&&&&&&&&&&
    SpriteSheetManager spriteSheetManager;
  
    // Gestor de físicas $$$$$$$$$$$$$
    PhysicsManager physicsManager;
    
    // Gestor de colisiones >>>>>>>>>>>>>
    CollisionManager collisionManager;
    
    // Gestor de movimientos
    MovementManager movementManager;
    
    // Gestor de GUI ^^^^^^^^^^^^^
    GUIManager guiManager;
    
    // Gestor de Modo Edición
    EditorManager editorManager;
    
    
    
    // *** TOGGLE MENU ***
    bool debug;
    bool showInteractors;
    bool editMode;
    
};
