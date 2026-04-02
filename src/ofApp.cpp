#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(60);
    
    // £££££££££££££ INPUT £££££££££££££
    // Configura el gestor de entradas con el gestor de movimientos
    inputManager.setup();
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // Configura el gestor de físicas con la posición inicial del personaje
    physicsManager.setup(ofGetWidth()/2,ofGetHeight()/2);
    
    // >>>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    // Configura el gestor de colisiones, con el tamaño de region de SpriteSheetManager
    collisionManager.setup(
        spriteSheetManager.getRegionWidth(),
        spriteSheetManager.getRegionHeight(),
        "interactors.json"
    );

    // &&&&&&&&&&&&& SPRITE SHEET &&&&&&&&&&&&&
    spriteSheetManager.setCollisionManager(&collisionManager);
    
    
    // GESTOR DE MOVIMIENTOS
    movementManager.setup("movements.json", spriteSheetManager, inputManager, physicsManager, collisionManager);
    
    // ^^^^^^^^^^^^^  GUI  ^^^^^^^^^^^^^
    guiManager.setup(movementManager, spriteSheetManager, inputManager, physicsManager, collisionManager, editorManager);
    
    // EDITOR (pasamos punteros a los managers que ya existen)
    editorManager.setup(&collisionManager, &guiManager);
    
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
    
    // EDITOR
    if (editMode) {
        editorManager.update(ofGetMouseX(), ofGetMouseY());
        
        // Si el editor creó o borró algo, refrescamos los botones de la GUI
        if (editorManager.hasPendingChanges()) {
            guiManager.updateInteractorsGroup();
            editorManager.setChangesProcessed();
        }
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
    
    // EDITOR
    if (editMode) {
        editorManager.draw();
        guiManager.drawEdit();
    }
}





void ofApp::keyPressed(int key) {
    bool modifier = ofGetKeyPressed(OF_KEY_COMMAND);
    
    // Si es una "D"
    if (key == 100) {
        setToggleDebug(!debug);
        setToggleEditMode(false);
        guiManager.setMinimizeInteractors();
    }
    // Si es una "I"
    if (key == 105) {
        setToggleShowInteractors(!showInteractors);
    }
    // Si es una "E"
    if (key == 101) {
        setToggleEditMode(!editMode);
        setToggleDebug(false);
        guiManager.setMaximizeInteractors();

    }
    
    if (modifier && (key == 'l' || key == 'L')) {
        if (debug){
            // CARGAMOS SETTINGS
            ofFileDialogResult loadFileResult = ofSystemLoadDialog("Seleccionar archivo de Settings (.xml)", false);
                        
            if (loadFileResult.bSuccess) {
                guiManager.loadSettingsFromFile(loadFileResult.getPath());
            }

        }
        else if (editMode){
            // CARGAMOS INTERACTORS
            ofFileDialogResult loadFileResult = ofSystemLoadDialog("Seleccionar archivo de Interactors (.json)", false);
            
            if (loadFileResult.bSuccess) {
                // Cargamos los datos en el manager de colisiones
                collisionManager.loadInteractorsJSON(loadFileResult.getPath());
                collisionManager.applyMandatorySettings(spriteSheetManager.getRegionWidth(), spriteSheetManager.getRegionHeight());
                
                // Refrescamos la lista de botones en la GUI
                guiManager.updateInteractorsGroup();
                
                ofLogNotice("App") << "Nivel de interactores cargado correctamente.";
            }
        }
    }
    
    // Detectamos si 's' o 'S' está presionada junto con la tecla de sistema (Command)
    if (modifier && (key == 's' || key == 'S')) {
        if (debug){
            // GUARDAMOS SETTINGS
            // Abrimos el diálogo para que el usuario elija nombre y carpeta
            ofFileDialogResult saveResult = ofSystemSaveDialog("settings.xml", "Exportar ajustes de la GUI (.xml)");
            
            if (saveResult.bSuccess) {
                guiManager.saveSettingsToFile(saveResult.getPath());
                ofLogNotice("App") << "Configuración guardada en: " << saveResult.getPath();
            }

        }
        else if (editMode){
            // GUARDAMOS INTERACTORS
            collisionManager.saveInteractorsJSON("interactors.json");
            ofLogNotice("App") << "Guardado manual solicitado por el usuario.";
        }
    }
    
    if (editMode) {
        // GESTIONAR BACKSPACE PARA CANCELAR CREACIÓN
        editorManager.handleKeyPressed(key);
    }
    
    // Maneja la pulsación de una tecla
    inputManager.keyPressed(key);
}

void ofApp::keyReleased(int key) {
    // Maneja la liberación de una tecla
    inputManager.keyReleased(key);
}

void ofApp::mousePressed(int x, int y, int button){
    
    if (editMode) {
        // En modo edición, el click es para crear/borrar puntos
        editorManager.handleMousePressed(x, y, button);
    }
    else {
        // Fuera de edición, Lógica de teletransporte del personaje
        physicsManager.setPositionX((float)x);
        physicsManager.setPositionY((float)y);
    }
}


void ofApp::setToggleDebug(bool value) {
    debug = value;
}

void ofApp::setToggleShowInteractors(bool value) {
    showInteractors = value;
}

void ofApp::setToggleEditMode(bool value) {
    editMode = value;
    editorManager.setEnabled(value);
    
    guiManager.updateInteractorsGroup();
}

