#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(60);
    
    /// £££££££££££££ INPUT £££££££££££££
    // Configura el gestor de entradas con el gestor de movimientos
    inputManager.setup();
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Configura el gestor de físicas con la posición inicial del personaje
    physicsManager.setup(ofGetWidth()/2,ofGetHeight()/2);
    
    // >>>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    // Configura el gestor de colisiones, con el tamaño de region de SpriteSheetManager
    collisionManager.setup(
        spriteSheetManager.getRegionWidth(),
        spriteSheetManager.getRegionHeight()
    );

    // &&&&&&&&&&&&& SPRITE SHEET &&&&&&&&&&&&&
    spriteSheetManager.setCollisionManager(&collisionManager);
    
    
    // GESTOR DE MOVIMIENTOS
    movementManager.setup("movements.json", spriteSheetManager, inputManager, physicsManager, collisionManager);
    
    // ^^^^^^^^^^^^^  GUI  ^^^^^^^^^^^^^
    guiManager.setup(movementManager, spriteSheetManager, inputManager, physicsManager, collisionManager);
    
    // MENU
    debug = true;
    showInteractors = true;
    editMode = false;
}


// *** UPDATE ***
void ofApp::update() {
    
    // Obtiene el tiempo transcurrido en segundos desde que se ha ejectuado la aplicación
    float currentTime = ofGetElapsedTimef();
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    physicsManager.update();

    // GESTOR DE MOVIMIENTOS (revisa también las colisiones)
    movementManager.update(currentTime);
    
    
    // ************************ DEBUG ************************
    // ^^^^^^^^^^^^^  GUI  ^^^^^^^^^^^^^
    if(debug){
        guiManager.update();
    }
    
}


// *** DRAW ***
void ofApp::draw() {
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    if(showInteractors){
        collisionManager.draw();
    }
    
    // &&&&&&&&&&&&& SPRITE SHEET &&&&&&&&&&&&&
    spriteSheetManager.draw(
        physicsManager.getPosition().x,
        physicsManager.getPosition().y,
        movementManager.getCurrentRow(),
        movementManager.getCurrentRegion(),
        movementManager.getIsFacingRight(),
        movementManager.getCurrentFile(),
        debug
    );
    
    
    // ************************ DEBUG ************************
    // ^^^^^^^^^^^^^  GUI  ^^^^^^^^^^^^^
    if(debug){
        guiManager.draw();
    }
    
}





void ofApp::keyPressed(int key) {
    // Si es una "D"
    if (key == 100) {
        setToggleDebug(!debug);
    }
    // Si es una "I"
    if (key == 105) {
        setToggleShowInteractors(!showInteractors);
    }
    // Si es una "E"
    if (key == 101) {
        setToggleEditMode(!editMode);
    }
    
    // Maneja la pulsación de una tecla
    inputManager.keyPressed(key);
}

void ofApp::keyReleased(int key) {
    
    // Maneja la liberación de una tecla
    inputManager.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int button){
        physicsManager.setPositionX((float)x);
        physicsManager.setPositionY((float)y);    
}


void ofApp::setToggleDebug(bool value) {
    debug = value;
}

void ofApp::setToggleShowInteractors(bool value) {
    showInteractors = value;
}

void ofApp::setToggleEditMode(bool value) {
    editMode = value;
}

