#pragma once

#include "ofxGui.h"
#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"

class GUIManager {
public:
    void setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& phisicsManager);
    void update();
    void draw();

private:
    //*** GUI ***
    ofxPanel gui;
    
    //*** SLIDERS ***
    ofxFloatSlider scaleFactorGui;
    
    //*** TIME SLIDERS ***
    ofxFloatSlider frameIntervalGui; // Slider para Tiempo Global
    ofxLabel currentMovementFrameIntervalGui;
    
    //*** TIME SLIDERS SPECIFIC MOVEMENT ***
    ofxFloatSlider idleFrameIntervalGui; // Slider para IDLE
    ofxFloatSlider walkFrameIntervalGui; // Slider para WALK
    ofxFloatSlider walkToIdle1FrameIntervalGui; // Slider para WalkToIdle1
    ofxFloatSlider walkToIdle2FrameIntervalGui; // Slider para WalkToIdle2
    ofxFloatSlider walkToRun1FrameIntervalGui; // Slider para WalkToRun1
    ofxFloatSlider walkToRun2FrameIntervalGui; // Slider para WalkToRun2
    ofxFloatSlider runFrameIntervalGui;  // Slider para RUN
    ofxFloatSlider runToIdle1FrameIntervalGui; // Slider para RunToIdle1
    ofxFloatSlider runToIdle2FrameIntervalGui; // Slider para RunToIdle2
    ofxFloatSlider runToWalk1FrameIntervalGui; // Slider para RunToWalk1
    ofxFloatSlider runToWalk2FrameIntervalGui; // Slider para RunToWalk2
    ofxFloatSlider turnFrameIntervalGui; // Slider para TURN
    ofxFloatSlider turnToRunFrameIntervalGui; // Slider para TURN_TO_RUN
    ofxFloatSlider walkTurn1FrameIntervalGui; // Slider para WalkTurn1
    ofxFloatSlider walkTurn2FrameIntervalGui; // Slider para WalkTurn2
    ofxFloatSlider runTurn1FrameIntervalGui; // Slider para RunTurn1
    ofxFloatSlider runTurn2FrameIntervalGui; // Slider para RunTurn2
    
    
    //*** LABELS ***
    ofxLabel currentRowGui;
    ofxLabel currentRegionGui;
    ofxLabel currentVelocityXGui;       // Label para mostrar la velocidad actual del personaje
    ofxLabel currentMovementNameGui;   // Label para mostrar el nombre del movimiento actual
    ofxLabel NextOutRegionGui;         // Label para mostrar la siguiente región de salida
    ofxLabel currentStateGui;          // Para el estado actual
    ofxLabel targetStateGui;           // Para el estado objetivo

    ofxLabel controlKeysGui; // Label para mostrar las teclas de control
    ofxLabel currentIntentionGui; // Label para mostrar la Intención Actual
    
    
    //*** PHYSICS SLIDERS ***
    // NUEVOS: Sliders de velocidad (píxeles/frame)
    ofxFloatSlider maxSpeedWalkGui;
    ofxFloatSlider maxSpeedRunGui;
    
    // *** COLLISION SLIDERS (FROM SPRITE SHEET MANAGER) ***
    ofxFloatSlider hitBoxWGui;
    ofxFloatSlider  hitRayFloorXGui;
    
    

    MovementManager* movementManager;       // Añadimos una referencia a MovementManager
    SpriteSheetManager* spriteSheetManager; // Añadimos una referencia a SpriteSheetManager
    InputManager* inputManager;             // Añadimos una referencia a InputManager
    PhysicsManager* physicsManager;         // Añadimos una referencia a PhysicsManager
    
    
    
    
   

    
    // Callback for the scale factor slider
    void onScaleFactorChanged(float& value);
    
    // Callbacks for GUI sliders
    void onFrameIntervalChanged(float& value); // Callback para Global
    void onIdleFrameIntervalChanged(float& value); // Callback para IDLE
    void onWalkFrameIntervalChanged(float& value); // Callback para WALK
    void onWalkToIdle1FrameIntervalChanged(float& value); // Callback para WalkToIdle1
    void onWalkToIdle2FrameIntervalChanged(float& value); // Callback para Wal
    void onWalkToRun1FrameIntervalChanged(float& value); // Callback para WalkToRun1
    void onWalkToRun2FrameIntervalChanged(float& value); // Callback para WalkToRun2
    void onRunFrameIntervalChanged(float& value);  // Callback para RUN
    void onRunToIdle1FrameIntervalChanged(float& value); // Callback para RunToIdle1
    void onRunToIdle2FrameIntervalChanged(float& value); // Callback para RunToIdle2
    void onRunToWalk1FrameIntervalChanged(float& value); // Callback para RunToWalk1
    void onRunToWalk2FrameIntervalChanged(float& value); // Callback para RunToWalk2
    void onTurnFrameIntervalChanged(float& value); // Callback para TURN
    void onTurnToRunFrameIntervalChanged(float& value); // Callback para TurnToRun
    void onWalkTurn1FrameIntervalChanged(float& value); // Callback para WalkTurn1
    void onWalkTurn2FrameIntervalChanged(float& value); // Callback para WalkTurn2
    void onRunTurn1FrameIntervalChanged(float& value); // Callback para RunTurn1
    void onRunTurn2FrameIntervalChanged(float& value); // Callback para RunTurn2
    
    void onMaxSpeedWalkChanged(float& value); 
    void onMaxSpeedRunChanged(float& value);

    void onHitBoxWChanged(float& value);
    void onHitRayFloorXChanged(float& value);
    
};
