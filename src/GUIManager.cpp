#include "GUIManager.h"

#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"

void GUIManager::setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& physicsManager) {
    
    this->movementManager = &movementManager;        // Guardamos la referencia a MovementManager
    this->spriteSheetManager = &spriteSheetManager;  // Guardamos la referencia a SpriteSheetManager
    this->inputManager = &inputManager;              // Guardamos la referencia a InputManager
    this->physicsManager = &physicsManager;          // Guardamos la referencia a PhysicsManager
    
    // Inicializar GUI
    gui.setup("Settings");
    gui.setPosition(ofGetWidth() - 220, 30);
    
    // Añadir los sliders a la GUI
    gui.add(scaleFactorGui.setup("Scale Factor", 1.0f, 0.5, 3.0));
    gui.add(currentMovementFrameIntervalGui.setup("F.I. Movement", ""));
    gui.add(frameIntervalGui.setup("F.I. Global", 0.1f, 0.01, 0.5));
    
    //Sliders de Tiempo de Movimientos
    gui.add(idleFrameIntervalGui.setup("F.I. IDLE", 0.1f, 0.01, 0.5));
    gui.add(walkFrameIntervalGui.setup("F.I. WALK", 0.1f, 0.01, 0.5));
    gui.add(walkToIdle1FrameIntervalGui.setup("F.I. WALK_TO_IDLE_1", 0.1f, 0.01, 0.5));
    gui.add(walkToIdle2FrameIntervalGui.setup("F.I. WALK_TO_IDLE_2", 0.1f, 0.01, 0.5));
    gui.add(walkToRun1FrameIntervalGui.setup("F.I. WALK_TO_RUN_1", 0.1f, 0.01, 0.5));
    gui.add(walkToRun2FrameIntervalGui.setup("F.I. WALK_TO_RUN_2", 0.1f, 0.01, 0.5));
    gui.add(runFrameIntervalGui.setup("F.I. RUN", 0.1f, 0.01, 0.5));
    gui.add(runToIdle1FrameIntervalGui.setup("F.I. RUN_TO_IDLE_1", 0.1f, 0.01, 0.5));
    gui.add(runToIdle2FrameIntervalGui.setup("F.I. RUN_TO_IDLE_2", 0.1f, 0.01, 0.5));
    gui.add(runToWalk1FrameIntervalGui.setup("F.I. RUN_TO_WALK_1", 0.1f, 0.01, 0.5));
    gui.add(runToWalk2FrameIntervalGui.setup("F.I. RUN_TO_WALK_2", 0.1f, 0.01, 0.5));
    gui.add(turnFrameIntervalGui.setup("F.I. TURN", 0.1f, 0.01, 0.5));
    gui.add(turnToRunFrameIntervalGui.setup("F.I. TURN_TO_RUN", 0.1f, 0.01, 0.5));
    gui.add(walkTurn1FrameIntervalGui.setup("F.I. WALK_TURN_1", 0.1f, 0.01, 0.5));
    gui.add(walkTurn2FrameIntervalGui.setup("F.I. WALK_TURN_2", 0.1f, 0.01, 0.5));
    gui.add(runTurn1FrameIntervalGui.setup("F.I. RUN_TURN_1", 0.1f, 0.01, 0.5));
    gui.add(runTurn2FrameIntervalGui.setup("F.I. RUN_TURN_2", 0.1f, 0.01, 0.5));
    
    //*** LABELS ***
    gui.add(currentRowGui.setup("Current Row", "0"));
    gui.add(currentRegionGui.setup("Current Region", "0"));
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    gui.add(currentVelocityXGui.setup("Vel X", "0.0"));
    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    gui.add(currentMovementNameGui.setup("MOV", "IDLE"));
    gui.add(NextOutRegionGui.setup("Next Out Region", ""));
    gui.add(currentStateGui.setup("STATE", "IDLE"));
    gui.add(targetStateGui.setup("TARGET", "IDLE"));
    gui.add(controlKeysGui.setup("", "NONE"));
    gui.add(currentIntentionGui.setup("INTENT", "NO WANTS"));
    
    
    // $$$$$$$$$$$$$ FISICAS SLIDERS $$$$$$$$$$$$$
    gui.add(maxSpeedWalkGui.setup("Max Speed Walk", 5.0f, 0.0, 15.0));
    gui.add(maxSpeedRunGui.setup("Max Speed Run", 10.0f, 0.0, 25.0));
    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
    
    
    
    
    // >>> COLLISION SLIDERS (FROM SPRITE SHEET MANAGER) >>>
    gui.add(hitBoxWGui.setup("HitBox W", spriteSheetManager.getHitboxW(), 100, 400));
    gui.add(hitRayFloorXGui.setup("HitRayFloor X", spriteSheetManager.getHitRayXFloor(), 0, 200));
    
    
    
    
    
    
    // Inicializar los sliders con los valores actuales
    frameIntervalGui = movementManager.getFrameInterval();
    idleFrameIntervalGui = movementManager.getMovementFrameInterval("IDLE");
    walkFrameIntervalGui = movementManager.getMovementFrameInterval("WALK");
    walkToIdle1FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_IDLE_1");
    walkToIdle2FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_IDLE_2");
    walkToRun1FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_RUN_1");
    walkToRun2FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_RUN_2");
    runFrameIntervalGui = movementManager.getMovementFrameInterval("RUN");
    runToIdle1FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TO_IDLE_1");
    runToIdle2FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TO_IDLE_2");
    walkToRun1FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_RUN_1");
    walkToRun2FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_RUN_2");
    turnFrameIntervalGui = movementManager.getMovementFrameInterval("TURN");
    turnToRunFrameIntervalGui = movementManager.getMovementFrameInterval("TURN_TO_RUN");
    walkTurn1FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TURN_1");
    walkTurn2FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TURN_2");
    runTurn1FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TURN_1");
    runTurn2FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TURN_2");
    
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    currentVelocityXGui = std::to_string(physicsManager.getVelocity().x);
    maxSpeedWalkGui = physicsManager.getMaxSpeedWalk();
    maxSpeedRunGui = physicsManager.getMaxSpeedRun();

    
    
    
    // Asociar listeners a los sliders
    scaleFactorGui.addListener(this, &GUIManager::onScaleFactorChanged);
    frameIntervalGui.addListener(this, &GUIManager::onFrameIntervalChanged);
    idleFrameIntervalGui.addListener(this, &GUIManager::onIdleFrameIntervalChanged);
    walkFrameIntervalGui.addListener(this, &GUIManager::onWalkFrameIntervalChanged);
    walkToIdle1FrameIntervalGui.addListener(this, &GUIManager::onWalkToIdle1FrameIntervalChanged);
    walkToIdle2FrameIntervalGui.addListener(this, &GUIManager::onWalkToIdle2FrameIntervalChanged);
    walkToRun1FrameIntervalGui.addListener(this, &GUIManager::onWalkToRun1FrameIntervalChanged);
    walkToRun2FrameIntervalGui.addListener(this, &GUIManager::onWalkToRun2FrameIntervalChanged);
    runFrameIntervalGui.addListener(this, &GUIManager::onRunFrameIntervalChanged);
    runToIdle1FrameIntervalGui.addListener(this, &GUIManager::onRunToIdle1FrameIntervalChanged);
    runToWalk1FrameIntervalGui.addListener(this, &GUIManager::onRunToWalk1FrameIntervalChanged);
    runToWalk2FrameIntervalGui.addListener(this, &GUIManager::onRunToWalk2FrameIntervalChanged);
    turnFrameIntervalGui.addListener(this, &GUIManager::onTurnFrameIntervalChanged);
    turnToRunFrameIntervalGui.addListener(this, &GUIManager::onTurnToRunFrameIntervalChanged);
    walkTurn1FrameIntervalGui.addListener(this, &GUIManager::onWalkTurn1FrameIntervalChanged);
    walkTurn2FrameIntervalGui.addListener(this, &GUIManager::onWalkTurn2FrameIntervalChanged);
    runTurn1FrameIntervalGui.addListener(this, &GUIManager::onRunTurn1FrameIntervalChanged);
    runTurn2FrameIntervalGui.addListener(this, &GUIManager::onRunTurn2FrameIntervalChanged);
    
    maxSpeedWalkGui.addListener(this, &GUIManager::onMaxSpeedWalkChanged);
    maxSpeedRunGui.addListener(this, &GUIManager::onMaxSpeedRunChanged);
    
    
    
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    hitBoxWGui.addListener(this, &GUIManager::onHitBoxWChanged);
    hitRayFloorXGui.addListener(this, &GUIManager::onHitRayFloorXChanged);
    
    
    
    
}

void GUIManager::update() {
    // Actualiza en el FrameInterval del gestor de movimientos los valores de la GUI
    movementManager->setFrameIntervalFromGUI();
    
    // Actualiza Fila Actual en la GUI como String
    currentRowGui = std::to_string(movementManager->getCurrentRow());
    
    // Actualiza Región Actual en la GUI como String
    currentRegionGui = std::to_string(movementManager->getCurrentRegion());
    
    // Obtiene la siguiente región a llegar antes de transición en String; si es -1, asigna "N/A"
    if (! movementManager->isWaitingForTransition()) {
        NextOutRegionGui = "-";
    } else {
        NextOutRegionGui = movementManager->getNextOutRegion() == -1 ? "N/A" : std::to_string(movementManager->getNextOutRegion());
    }
    
    // Actualiza Estado Actual en la GUI como String
    currentStateGui = movementManager->getCurrentState();
    
    // Actualiza Estado Objetivo en la GUI como String
    targetStateGui = movementManager->getTargetState();
    
    // Actualiza Frame Interval Actual en la GUI como String
    //TO DO se debería ver el FramInterval Global, no el que se esta usando en el moviento.
    currentMovementFrameIntervalGui = std::to_string(movementManager->getCurrentMovementFrameInterval());
    
    // Actualizar las teclas presionadas
    controlKeysGui = inputManager->getPressedKeysAsString();

    // Actualizar las intenciones
    currentIntentionGui = inputManager->getIntentsAsString();
    
    // Actualiza el nombre del movimiento actual en la GUI
    currentMovementNameGui = movementManager->getCurrentMovementName();
    
    
    // Actualizar la velocidad actual del personaje en la GUI
    currentVelocityXGui = std::to_string(physicsManager->getVelocity().x);
}


void GUIManager::draw() {
    gui.draw();
}


#pragma region Callbacks para los sliders
void GUIManager::onScaleFactorChanged(float& value) {
    spriteSheetManager->setScaleFactor(value);
    physicsManager->setCurrentScale(value);
    
    //Como el factor de escala solo afecta a las velocidades objetivo, debemos actualizarlas
    physicsManager->setMaxSpeedWalk(physicsManager->getMaxSpeedWalk());
    physicsManager->setMaxSpeedRun(physicsManager->getMaxSpeedRun());
}

void GUIManager::onFrameIntervalChanged(float& value) {
    movementManager->setFrameInterval(value);
}

void GUIManager::onIdleFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("IDLE", value);
}

void GUIManager::onWalkFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK", value);
}

void GUIManager::onWalkToIdle1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TO_IDLE_1", value);
}

void GUIManager::onWalkToIdle2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TO_IDLE_2", value);
}

void GUIManager::onWalkToRun1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TO_RUN_1", value);
}

void GUIManager::onWalkToRun2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TO_RUN_2", value);
}

void GUIManager::onRunFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN", value);  
}

void GUIManager::onRunToIdle1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_IDLE_1", value);
}

void GUIManager::onRunToIdle2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_IDLE_2", value);
}

void GUIManager::onRunToWalk1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_WALK_1", value);
}

void GUIManager::onRunToWalk2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_WALK_2", value);
}

void GUIManager::onTurnFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("TURN", value);
}

void GUIManager::onTurnToRunFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("TURN_TO_RUN", value);
}

void GUIManager::onWalkTurn1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TURN_1", value);
}

void GUIManager::onWalkTurn2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("WALK_TURN_2", value);
}

void GUIManager::onRunTurn1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TURN_1", value);
}

void GUIManager::onRunTurn2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TURN_2", value);
}


void GUIManager::onMaxSpeedWalkChanged(float& value) {
    physicsManager->setMaxSpeedWalk(value);
}

void GUIManager::onMaxSpeedRunChanged(float& value) {
    physicsManager->setMaxSpeedRun(value);
}





// >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
void GUIManager::onHitBoxWChanged(float& value) {
    spriteSheetManager->setHitboxW(value);
}

void GUIManager::onHitRayFloorXChanged(float& value) {
    spriteSheetManager->setHitRayXFloor(value);
}






#pragma endregion
