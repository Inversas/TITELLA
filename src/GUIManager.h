#pragma once

#include "ofxGui.h"

// Necesitamos acceder a Interactor
#include "CollisionManager.h"

// Forward Delcarations
class MovementManager;
class SpriteSheetManager;
class InputManager;
class PhysicsManager;

// ****************************************** CLASE GUI MANAGER ******************************************
class GUIManager {
public:
    // DESTRUCTOR (lleva la virgulilla ~)
    ~GUIManager();
    void setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& phisicsManager, CollisionManager& collisionManager);
    void update();
    void draw();
    

private:
    // *** GUI ***
    ofxPanel gui;
    
    // *** GRUPOS DE LA INTERFAZ ***
    ofxGuiGroup settersGroup;        // Grupo [SETTERS]
    ofxGuiGroup frameIntervalGroup;  // Grupo [F.I.]
    ofxGuiGroup interactorsGroup;    // Grupo [INTERACTORS]
    ofxGuiGroup liveGroup;           // Grupo [LIVE] (Parámetros en tiempo real)
    
    
    // ==========================================
    // 1. SETTERS
    // ==========================================
    ofxFloatSlider frameIntervalGui;    // Slider para Tiempo Global
    ofxFloatSlider scaleFactorGui;      // Factor de escala global
    ofxFloatSlider maxSpeedWalkGui;     // Velocidad máxima caminar      $$$$$$$$$$$$$
    ofxFloatSlider maxSpeedRunGui;      // Velocidad máxima correr       $$$$$$$$$$$$$
    ofxFloatSlider hitBoxWGui;          // Ancho de la Hitbox            >>>>>>>>>>>>>
    ofxFloatSlider hitRayFloorXGui;     // Posición X del rayo de suelo  >>>>>>>>>>>>>
    
    
    // ==========================================
    // 2. F.I. (Frame Intervals)
    // ==========================================
    ofxLabel currentMovementFrameIntervalGui;
    ofxFloatSlider idleFrameIntervalGui;          // Slider para IDLE
    ofxFloatSlider walkFrameIntervalGui;          // Slider para WALK
    ofxFloatSlider walkToIdle1FrameIntervalGui;   // Slider para WalkToIdle1
    ofxFloatSlider walkToIdle2FrameIntervalGui;   // Slider para WalkToIdle2
    ofxFloatSlider walkToRun1FrameIntervalGui;    // Slider para WalkToRun1
    ofxFloatSlider walkToRun2FrameIntervalGui;    // Slider para WalkToRun2
    ofxFloatSlider runFrameIntervalGui;           // Slider para RUN
    ofxFloatSlider runToIdle1FrameIntervalGui;    // Slider para RunToIdle1
    ofxFloatSlider runToIdle2FrameIntervalGui;    // Slider para RunToIdle2
    ofxFloatSlider runToWalk1FrameIntervalGui;    // Slider para RunToWalk1
    ofxFloatSlider runToWalk2FrameIntervalGui;    // Slider para RunToWalk2
    ofxFloatSlider turnFrameIntervalGui;          // Slider para TURN
    ofxFloatSlider turnToRunFrameIntervalGui;     // Slider para TURN_TO_RUN
    ofxFloatSlider walkTurn1FrameIntervalGui;     // Slider para WalkTurn1
    ofxFloatSlider walkTurn2FrameIntervalGui;     // Slider para WalkTurn2
    ofxFloatSlider runTurn1FrameIntervalGui;      // Slider para RunTurn1
    ofxFloatSlider runTurn2FrameIntervalGui;      // Slider para RunTurn2
    ofxFloatSlider idleToWalkFrameIntervalGui;    // Slider para Idle To Walk
    ofxFloatSlider idleToRunFrameIntervalGui;;    // Slider para Idle To Run
    
    
    
    // ==========================================
    // 3.INTERACTORS
    // ==========================================
    // Usamos punteros normales para evitar conflictos con addListener
    vector<ofxToggle*> dynamicButtons;
    // *** ACTUALIZAR BOTONES DE INTERACTORS ***
    void updateInteractorsGroup(ofxGuiGroup &group);
    // *** AÑADIR BOTON A INTEREACTORS ***
    void addDynamicButton(string name);
    // *** ELIMINAR BOTONES DE INTEREACTORS ***
    void clearDynamicButtons();
    // *** PONER TODOS LOS INTERACTORS A FALSE ***
    void resetInteractors();
    
    // ==========================================
    // 4. LIVE (Parámetros de estado actuales)
    // ==========================================
    ofVec2f livePanelPosition;
    void updateLivePanel();
    void drawLivePanel(ofVec2f position);
    
    
    string currentRowGui;
    string currentRegionGui;
    string currentVelocityXGui;      // Label para mostrar la velocidad actual del personaje
    string currentMovementNameGui;   // Label para mostrar el nombre del movimiento actual
    string nextOutRegionGui;         // Label para mostrar la siguiente región de salida
    string currentStateGui;          // Para el estado actual
    string targetStateGui;           // Para el estado objetivo
    string controlKeysGui;           // Label para mostrar las teclas de control
    string currentIntentionGui;      // Label para mostrar la Intención Actual
    
    
    // *** REFERENCIAS A GESTORES *** //
    MovementManager* movementManager;       // Añadimos una referencia a MovementManager
    SpriteSheetManager* spriteSheetManager; // Añadimos una referencia a SpriteSheetManager
    InputManager* inputManager;             // Añadimos una referencia a InputManager
    PhysicsManager* physicsManager;         // Añadimos una referencia a P
    CollisionManager* collisionManager;     // Añadimos una referencia a CollisionManagercsManager
    

    // *** CALLBACKS *** //

    // Callbacks for INTERACTOR buttons
    void onInteractorButtonPressed(bool& value);
    
    // Callbacks for SETTERS sliders
    void onScaleFactorChanged(float& value);
    void onMaxSpeedWalkChanged(float& value);
    void onMaxSpeedRunChanged(float& value);
    void onHitBoxWChanged(float& value);
    void onHitRayFloorXChanged(float& value);
    
    
    // Callbacks for FRAME INTERVAL sliders
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
    void onIdleToWalkFrameIntervalChanged(float& value); // Callback para Idle To Walk
    void onIdleToRunFrameIntervalChanged(float& value); // Callback para Idle To Run
};
