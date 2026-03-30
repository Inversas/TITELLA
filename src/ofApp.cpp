#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(60);
    
    
    
    
    
    // IMATGES SPRITES SHEETS
    spriteSheetManager.loadSpriteSheet("ProvaTest_7.png");
    
    
    
    
    
    // Configura el gestor de entradas con el gestor de movimientos
    inputManager.setup();
    
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Configura el gestor de físicas con la posición inicial del personaje
    physicsManager.setup(ofGetWidth()/2,ofGetHeight()/2);
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    // Configura el gestor de colisiones, con el tamaño de region de SpriteSheetManager
    collisionManager.setup(
        spriteSheetManager.getRegionWidth(),
        spriteSheetManager.getRegionHeight()
    );

    // SPRITE SHEET
    spriteSheetManager.setCollisionManager(&collisionManager);
    
    // GESTOR DE MOVIMIENTOS
    movementManager.setup("movements.json", spriteSheetManager, inputManager, physicsManager, collisionManager);
    
    // GUI
    guiManager.setup(movementManager, spriteSheetManager, inputManager, physicsManager, collisionManager);
}


void ofApp::update() {
    
    // Obtiene el tiempo transcurrido en segundos desde que se ha ejectuado la aplicación
    float currentTime = ofGetElapsedTimef();
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    physicsManager.update();

    // GESTOR DE MOVIMIENTOS
    movementManager.update(currentTime);
    
    // GUI
    guiManager.update();
}


void ofApp::draw() {
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    collisionManager.draw();
    
    // SPRITE SHEET
    spriteSheetManager.draw(
        physicsManager.getPosition().x,
        physicsManager.getPosition().y,
        movementManager.getCurrentRow(),
        movementManager.getCurrentRegion(),
        movementManager.getIsFacingRight()
    );
    
    // GUI
    guiManager.draw();
}


void ofApp::keyPressed(int key) {
    
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
