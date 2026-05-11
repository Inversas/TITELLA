#pragma once

#include "ofxGui.h"

// Necesitamos acceder a Interactor
#include "CollisionManager.h"


// !!! MOTIVO FORWARD DECLARATIONS !!!
// Solo usamos punteros a los Managers
class MovementManager;
class SpriteSheetManager;
class InputManager;
class PhysicsManager;
class EditorManager;


// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// El iconito de guardado de la GUI reescribe el archivo settings.xml de la carpeta data
// Ese archivo se carga por defecto al ejectuar el programa
// El iconito de cargar (flechas circulares) carga settings.xml
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬


// ****************************************** CLASE GUI MANAGER ******************************************
class GUIManager {
public:
    // !!! MOTIVO CONSTR / DESTR !!!
    // Usamos muchos Punteros y uso de memoria dinámica.
    // *** CONSTRUCTOR ***
    GUIManager();
    // *** DESTRUCTOR ***
    ~GUIManager();
    
    void setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& phisicsManager, CollisionManager& collisionManager, EditorManager& editorManager);
    void update();
    void draw();
    void drawEdit();

    // *** ACTUALIZAR BOTONES DE INTERACTORS ***
    void updateInteractorsGroup();
    void setNameInteractorsGroup(const string& groupName);
    void setMaximizeInteractors();
    void setMinimizeInteractors();
    
    // *** CARGA SETTINGS ESPECIFICO ***
    void loadSettingsFromFile(const std::string& path);
    // *** GUARDA SETTINGS ESPECIFICO ***
    void saveSettingsToFile(const string& path);
    

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
    ofxLabel currentVelocityYGui;       // Label para mostrar la velocidad actual del personaje en el eje Y
    ofxLabel gravityGui;                // Label para mostrar la gravedad
    
    
    // ==========================================
    // 2. F.I. (Frame Intervals)
    // ==========================================
    ofxLabel currentMovementFrameIntervalGui;
    ofxFloatSlider idleFrameIntervalGui;               // Slider para IDLE
    ofxFloatSlider walkFrameIntervalGui;               // Slider para WALK
    ofxFloatSlider walkToIdle1FrameIntervalGui;        // Slider para WalkToIdle1
    ofxFloatSlider walkToIdle2FrameIntervalGui;        // Slider para WalkToIdle2
    ofxFloatSlider walkToRun1FrameIntervalGui;         // Slider para WalkToRun1
    ofxFloatSlider walkToRun2FrameIntervalGui;         // Slider para WalkToRun2
    ofxFloatSlider runFrameIntervalGui;                // Slider para RUN
    ofxFloatSlider runToIdle1FrameIntervalGui;         // Slider para RunToIdle1
    ofxFloatSlider runToIdle2FrameIntervalGui;         // Slider para RunToIdle2
    ofxFloatSlider runToWalk1FrameIntervalGui;         // Slider para RunToWalk1
    ofxFloatSlider runToWalk2FrameIntervalGui;         // Slider para RunToWalk2
    ofxFloatSlider idleTurnToIdleFrameIntervalGui;     // Slider para IdleTurnToIdle
    ofxFloatSlider idleTurnToRunFrameIntervalGui;      // Slider para IdleTurnToRun
    ofxFloatSlider walkTurn1FrameIntervalGui;          // Slider para WalkTurn1
    ofxFloatSlider walkTurn2FrameIntervalGui;          // Slider para WalkTurn2
    ofxFloatSlider runTurn1FrameIntervalGui;           // Slider para RunTurn1
    ofxFloatSlider runTurn2FrameIntervalGui;           // Slider para RunTurn2
    ofxFloatSlider idleToWalkFrameIntervalGui;         // Slider para Idle To Walk
    ofxFloatSlider idleToRunFrameIntervalGui;;         // Slider para Idle To Run
    
    ofxFloatSlider idleToJumpFrameIntervalGui;         // Slider para IdleToJump
    ofxFloatSlider jumpFrameIntervalGui;               // Slider para JUMP
    ofxFloatSlider jumpToFallFrameIntervalGui;         // Slider para JumpToFall
    ofxFloatSlider fallFrameIntervalGui;               // Slider para FALL
    ofxFloatSlider landToIdleFrameIntervalGui;         // Slider para LandToIdle
    ofxFloatSlider walkToJump1FrameIntervalGui;        // Slider para WalkToJump1
    ofxFloatSlider walkToJump2FrameIntervalGui;        // Slider para WalkToJump2
    ofxFloatSlider landToWalkFrameIntervalGui;         // Slider para LandToWalk
    ofxFloatSlider runToJump1FrameIntervalGui;         // Slider para RunToJump1
    ofxFloatSlider runToJump2FrameIntervalGui;         // Slider para RunToJump2
    ofxFloatSlider landToRunFrameIntervalGui;          // Slider para LandToRun
    ofxFloatSlider jumpTurnFrameIntervalGui;           // Slider para JumpTurn
    ofxFloatSlider fallTurnFrameIntervalGui;           // Slider para FallTurn
    
    
    // ==========================================
    // 3.INTERACTORS
    // ==========================================
    // Usamos punteros normales para evitar conflictos con addListener
    vector<ofxToggle*> dynamicButtons;

    // *** AÑADIR BOTON A INTEREACTORS ***
    void addDynamicButton(string name, int indiceBoton);
    // *** ELIMINAR BOTONES DE INTEREACTORS ***
    void clearDynamicButtons();
    // *** PONER TODOS LOS INTERACTORS A FALSE ***
    void resetInteractorsButtons();
    
    // ==========================================
    // 4. LIVE (Parámetros de estado actuales)
    // ==========================================
    ofVec2f livePanelPosition;
    void updateLivePanel();
    void drawLivePanel(ofVec2f position);
    
    
    string currentRowGui;
    string currentRegionGui;
    
    string controlKeysGui;           // Label para mostrar las teclas de control
    string currentStateGui;          // Para el estado actual
    string currentIntentionGui;      // Label para mostrar la Intención Actual
    string targetStateGui;           // Para el estado objetivo
    
    string currentMovementNameGui;   // Label para mostrar el nombre del movimiento actual
    string nextOutRegionGui;         // Label para mostrar la siguiente región de salida

    string currentVelocityXGui;      // Label para mostrar la velocidad actual del personaje en el eje X

    bool isGroundedGui = false;     // Bool para mostrar si el personaje está en el suelo o no
    bool isWalledGui   = false;     // Bool  para mostrar si el personaje está tocando una pared o no

    
    
    // *** REFERENCIAS A GESTORES *** //
    MovementManager* movementManager;       // Añadimos una referencia a MovementManager
    SpriteSheetManager* spriteSheetManager; // Añadimos una referencia a SpriteSheetManager
    InputManager* inputManager;             // Añadimos una referencia a InputManager
    PhysicsManager* physicsManager;         // Añadimos una referencia a P
    CollisionManager* collisionManager;     // Añadimos una referencia a CollisionManagercsManager
    EditorManager* editorManager;           // Añadimos una referencia a EditorManager
    

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
    void onFrameIntervalChanged(float& value);                 // Callback para Global
    void onIdleFrameIntervalChanged(float& value);             // Callback para IDLE
    void onWalkFrameIntervalChanged(float& value);             // Callback para WALK
    void onWalkToIdle1FrameIntervalChanged(float& value);      // Callback para WalkToIdle1
    void onWalkToIdle2FrameIntervalChanged(float& value);      // Callback para WalkToIdle2
    void onWalkToRun1FrameIntervalChanged(float& value);       // Callback para WalkToRun1
    void onWalkToRun2FrameIntervalChanged(float& value);       // Callback para WalkToRun2
    void onRunFrameIntervalChanged(float& value);              // Callback para RUN
    void onRunToIdle1FrameIntervalChanged(float& value);       // Callback para RunToIdle1
    void onRunToIdle2FrameIntervalChanged(float& value);       // Callback para RunToIdle2
    void onRunToWalk1FrameIntervalChanged(float& value);       // Callback para RunToWalk1
    void onRunToWalk2FrameIntervalChanged(float& value);       // Callback para RunToWalk2
    void onIdleTurnToIdleFrameIntervalChanged(float& value);   // Callback para IdleTurnToIdle
    void onIdleTurnToRunFrameIntervalChanged(float& value);    // Callback para IdleTurnToRun
    void onWalkTurn1FrameIntervalChanged(float& value);        // Callback para WalkTurn1
    void onWalkTurn2FrameIntervalChanged(float& value);        // Callback para WalkTurn2
    void onRunTurn1FrameIntervalChanged(float& value);         // Callback para RunTurn1
    void onRunTurn2FrameIntervalChanged(float& value);         // Callback para RunTurn2
    void onIdleToWalkFrameIntervalChanged(float& value);       // Callback para Idle To Walk
    void onIdleToRunFrameIntervalChanged(float& value);        // Callback para Idle To Run

    void onIdleToJumpFrameIntervalChanged(float& value);       // Callback para IdleToJump
    void onJumpFrameIntervalChanged(float& value);             // Callback para JUMP
    void onJumpToFallFrameIntervalChanged(float& value);       // Callback para JumpToFall
    void onFallFrameIntervalChanged(float& value);             // Callback para FALL
    void onLandToIdleFrameIntervalChanged(float& value);       // Callback para LandToIdle
    void onWalkToJump1FrameIntervalChanged(float& value);      // Callback para WalkToJump1
    void onWalkToJump2FrameIntervalChanged(float& value);      // Callback para WalkToJump2
    void onLandToWalkFrameIntervalChanged(float& value);       // Callback para LandToWalk
    void onRunToJump1FrameIntervalChanged(float& value);       // Callback para RunToJump1
    void onRunToJump2FrameIntervalChanged(float& value);       // Callback para RunToJump2
    void onLandToRunFrameIntervalChanged(float& value);        // Callback para LandToRun
    void onJumpTurnFrameIntervalChanged(float& value);         // Callback para JumpTurn
    void onFallTurnFrameIntervalChanged(float& value);         // Callback para FallTurn
};
