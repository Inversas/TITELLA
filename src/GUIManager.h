#pragma once

#include "ofxGui.h"
#include "MovementManager.h"
#include "SpriteSheetManager.h"


class GUIManager {
public:
    void setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager);
    void update();
    void draw();

private:
    ofxPanel gui;
    ofxFloatSlider scaleFactorGui;
    ofxFloatSlider frameIntervalGui; // Slider para Tiempo Global
    ofxLabel currentMovementFrameIntervalGui;
    ofxLabel currentRowGui;
    ofxLabel currentRegionGui;
    ofxLabel NextRegionToGoGui;
    ofxFloatSlider idleFrameIntervalGui; // Slider para IDLE
    ofxFloatSlider walkFrameIntervalGui; // Slider para WALK
    ofxFloatSlider walkToIdle1FrameIntervalGui; // Slider para WalkToIdle1
    ofxFloatSlider walkToIdle2FrameIntervalGui; // Sliedr para WalkToIdle2
    ofxFloatSlider runFrameIntervalGui;  // Slider para RUN
    ofxFloatSlider runToIdle1FrameIntervalGui; // Slider para RunToIdle1
    ofxFloatSlider runToIdle2FrameIntervalGui; // Sliedr para RunToIdle2
    MovementManager* movementManager;        // Añadimos una referencia a MovementManager
    SpriteSheetManager* spriteSheetManager;  // Añadimos una referencia a SpriteSheetManager
    

    
    // Callback for the scale factor slider
    void onScaleFactorChanged(float& value);
    
    // Callbacks for GUI sliders
    void onFrameIntervalChanged(float& value); // Callback para Global
    void onIdleFrameIntervalChanged(float& value); // Callback para IDLE
    void onWalkFrameIntervalChanged(float& value); // Callback para WALK
    void onWalkToIdle1FrameIntervalChanged(float& value); // Callback para WalkToIdle1
    void onWalkToIdle2FrameIntervalChanged(float& value); // Callback para WalkToIdle2
    void onRunFrameIntervalChanged(float& value);  // Callback para RUN
    void onRunToIdle1FrameIntervalChanged(float& value); // Callback para RunToIdle1
    void onRunToIdle2FrameIntervalChanged(float& value); // Callback para RunToIdle1
    
};
