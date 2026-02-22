#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(60);
    spriteSheetManager.loadSpriteSheet("ProvaTest_5.png");
    // Configura el gestor de entradas con el gestor de movimientos
    inputManager.setup();
    
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Configura el gestor de físicas con la posición inicial del personaje
    physicsManager.setup(ofGetWidth()/2,ofGetHeight()/2);
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    // Configura el gestor de colisiones
    collisionManager.setup();
    
    
    // Configura el gestor de movimientos con "movements.json"
    movementManager.setup("movements.json", spriteSheetManager, inputManager, physicsManager, collisionManager);
    
    // Configura el gestor de la GUI con los gestores de movimientos y de sprites
    guiManager.setup(movementManager, spriteSheetManager, inputManager, physicsManager);
}


void ofApp::update() {
    
    // Obtiene el tiempo transcurrido en segundos desde que se ha ejectuado la aplicación
    float currentTime = ofGetElapsedTimef();
    
    physicsManager.update();

    // Actualiza el gestor de movimientos con el tiempo actual
    movementManager.update(currentTime);
    
    // Actualiza el gestor de físicas 
    //physicsManager.applyVelocity();
    
    // Actualiza el gestor de la GUI
    guiManager.update();
}


void ofApp::draw() {
    // Dibuja interactors
    collisionManager.draw();
    
    // Dibuja el sprite actual basado en el gestor de movimientos
    spriteSheetManager.draw(
        physicsManager.getPosition().x,
        physicsManager.getPosition().y,
        movementManager.getCurrentRow(),
        movementManager.getCurrentRegion(),
        movementManager.getIsFacingRight()
    );
    
    //Dibuja la GUI
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
