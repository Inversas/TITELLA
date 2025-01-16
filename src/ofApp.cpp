#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(60);
    spriteSheetManager.loadSpriteSheet("ProvaTest_2.png");
    
    // Configura el gestor de movimientos con "movements.json"
    movementManager.setup("movements.json", spriteSheetManager);
    
    // Configura el gestor de la GUI con los gestores de movimientos y de sprites
    guiManager.setup(movementManager, spriteSheetManager);
    
    // Configura el gestor de entradas con el gestor de movimientos
    inputManager.setup(movementManager);
}


void ofApp::update() {
    
    // Obtiene el tiempo transcurrido en segundos desde que se ha ejectuado la aplicación
    float currentTime = ofGetElapsedTimef();

    // Actualiza el gestor de movimientos con el tiempo actual
    movementManager.update(currentTime);
    
    // Actualiza el gestor de la GUI
    guiManager.update();
}


void ofApp::draw() {
    
    // Dibuja el sprite actual basado en el gestor de movimientos
    spriteSheetManager.draw(movementManager.getCurrentRow(), movementManager.getCurrentRegion());
    
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
