#include "GUIManager.h"

void GUIManager::setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager) {
    
    this->movementManager = &movementManager;        // Guardamos la referencia a MovementManager
    this->spriteSheetManager = &spriteSheetManager;  // Guardamos la referencia a SpriteSheetManager
    
    // Inicializar GUI
    gui.setup("Settings");
    gui.setPosition(ofGetWidth() - 220, 10);
    
    // Añadir los sliders a la GUI
    gui.add(scaleFactorGui.setup("Scale Factor", 1.0f, 0.5, 3.0));
    gui.add(currentRowGui.setup("Current Row", "0"));
    gui.add(currentRegionGui.setup("Current Region", "0"));
    gui.add(NextRegionToGoGui.setup("Next Region To Go", ""));
    gui.add(currentMovementFrameIntervalGui.setup("F.I. Movement", ""));
    gui.add(frameIntervalGui.setup("F.I. Global", 0.1f, 0.01, 0.5));
    
    //Sliders de Tiempo de Movimientos
    gui.add(idleFrameIntervalGui.setup("F.I. IDLE", 0.1f, 0.01, 0.5));
    gui.add(walkFrameIntervalGui.setup("F.I. WALK", 0.1f, 0.01, 0.5));
    gui.add(walkToIdle1FrameIntervalGui.setup("F.I. WALK_TO_IDLE_1", 0.1f, 0.01, 0.5));
    gui.add(walkToIdle2FrameIntervalGui.setup("F.I. WALK_TO_IDLE_2", 0.1f, 0.01, 0.5));
    gui.add(runFrameIntervalGui.setup("F.I. RUN", 0.1f, 0.01, 0.5));
    gui.add(runToIdle1FrameIntervalGui.setup("F.I. RUN_TO_IDLE_1", 0.1f, 0.01, 0.5));
    gui.add(runToIdle2FrameIntervalGui.setup("F.I. RUN_TO_IDLE_2", 0.1f, 0.01, 0.5));
    
    // Inicializar los sliders con los valores actuales
    frameIntervalGui = movementManager.getFrameInterval();
    idleFrameIntervalGui = movementManager.getMovementFrameInterval("IDLE");
    walkFrameIntervalGui = movementManager.getMovementFrameInterval("WALK");
    walkToIdle1FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_IDLE_1");
    walkToIdle2FrameIntervalGui = movementManager.getMovementFrameInterval("WALK_TO_IDLE_2");
    runFrameIntervalGui = movementManager.getMovementFrameInterval("RUN");
    runToIdle1FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TO_IDLE_1");
    runToIdle2FrameIntervalGui = movementManager.getMovementFrameInterval("RUN_TO_IDLE_2");

    
    // Asociar listeners a los sliders
    scaleFactorGui.addListener(this, &GUIManager::onScaleFactorChanged);
    
    frameIntervalGui.addListener(this, &GUIManager::onFrameIntervalChanged);
    idleFrameIntervalGui.addListener(this, &GUIManager::onIdleFrameIntervalChanged);
    walkFrameIntervalGui.addListener(this, &GUIManager::onWalkFrameIntervalChanged);
    walkToIdle1FrameIntervalGui.addListener(this, &GUIManager::onWalkToIdle1FrameIntervalChanged);
    walkToIdle2FrameIntervalGui.addListener(this, &GUIManager::onWalkToIdle2FrameIntervalChanged);
    runFrameIntervalGui.addListener(this, &GUIManager::onRunFrameIntervalChanged);
    runToIdle1FrameIntervalGui.addListener(this, &GUIManager::onRunToIdle1FrameIntervalChanged);
    runToIdle2FrameIntervalGui.addListener(this, &GUIManager::onRunToIdle2FrameIntervalChanged);
}

void GUIManager::update() {
    // Actualiza en el FrameInterval del gestor de movimientos los valores de la GUI
    movementManager->updateFrameIntervalFromGUI();
    
    // Actualiza Fila Actual en la GUI como String
    currentRowGui = std::to_string(movementManager->getCurrentRow());
    
    // Actualiza Región Actual en la GUI como String
    currentRegionGui = std::to_string(movementManager->getCurrentRegion());
    
    // Obtiene la siguiente región a llegar antes de transición en String; si es -1, asigna "N/A"
    NextRegionToGoGui = movementManager->getNextRegionToGo() == -1 ? "N/A" : std::to_string(movementManager->getNextRegionToGo());
    
    // Actualiza Frame Interval Actual en la GUI como String
    //TO DO se debería ver el FramInterval Global, no el que se esta usando en el moviento.
    currentMovementFrameIntervalGui = std::to_string(movementManager->getCurrentMovementFrameInterval());
}


void GUIManager::draw() {
    gui.draw();
}


#pragma region Callbacks para los sliders
void GUIManager::onScaleFactorChanged(float& value) {
    spriteSheetManager->setScaleFactor(value);
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

void GUIManager::onRunFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN", value);  
}

void GUIManager::onRunToIdle1FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_IDLE_1", value);
}

void GUIManager::onRunToIdle2FrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("RUN_TO_IDLE_2", value);
}

#pragma endregion
