#include "GUIManager.h"

#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"

void GUIManager::setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& physicsManager, CollisionManager& collisionManager) {
    
    this->movementManager = &movementManager;        // Guardamos la referencia a MovementManager
    this->spriteSheetManager = &spriteSheetManager;  // Guardamos la referencia a SpriteSheetManager
    this->inputManager = &inputManager;              // Guardamos la referencia a InputManager
    this->physicsManager = &physicsManager;          // Guardamos la referencia a PhysicsManager
    this->collisionManager = &collisionManager;      // Guardamos la referencia a CollisionManager
    
    // Inicializar GUI
    gui.setup("Settings");
    gui.setPosition(ofGetWidth() - 220, 30);
    
    
    // ==============================================================================================================================
    // CONFIGURACIÓN GRUPO: [SETTERS]
    // ==============================================================================================================================
    settersGroup.setup("SETTERS");
    
    // GLOBAL SLIDERS
    settersGroup.add(frameIntervalGui.setup("F.I. Global", 0.1f, 0.01, 0.5));
    settersGroup.add(scaleFactorGui.setup("Scale Factor", 1.0f, 0.5, 3.0));
    
    // FISICAS SLIDERS $$$$$$$$$$$$$
    settersGroup.add(maxSpeedWalkGui.setup("Max Speed Walk", 5.0f, 0.0, 15.0));
    settersGroup.add(maxSpeedRunGui.setup("Max Speed Run", 10.0f, 0.0, 25.0));
    
    // COLLISION SLIDERS >>>>>>>>>>>>
    settersGroup.add(hitBoxWGui.setup("HitBox W", collisionManager.getHitbox().width, 100, 400));
    settersGroup.add(hitRayFloorXGui.setup("HitRayFloor X", collisionManager.getHitbox().floorRayX, 0, 200));
    
    // AÑADIR A GUI
    gui.add(&settersGroup);
    
    // ************************ DEBUG ************************
    // ABIERTO POR DEFECTO
    settersGroup.maximize();
    
    
    // ==============================================================================================================================
    // CONFIGURACIÓN GRUPO: [F.I.]
    // ==============================================================================================================================
    frameIntervalGroup.setup("F.I. (Frame Intervals)");
    
    // F.I SLIDERS
    frameIntervalGroup.add(currentMovementFrameIntervalGui.setup("F.I. Movement", ""));
    frameIntervalGroup.add(idleFrameIntervalGui.setup("F.I. IDLE", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkFrameIntervalGui.setup("F.I. WALK", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkToIdle1FrameIntervalGui.setup("F.I. WALK_TO_IDLE_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkToIdle2FrameIntervalGui.setup("F.I. WALK_TO_IDLE_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkToRun1FrameIntervalGui.setup("F.I. WALK_TO_RUN_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkToRun2FrameIntervalGui.setup("F.I. WALK_TO_RUN_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runFrameIntervalGui.setup("F.I. RUN", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runToIdle1FrameIntervalGui.setup("F.I. RUN_TO_IDLE_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runToIdle2FrameIntervalGui.setup("F.I. RUN_TO_IDLE_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runToWalk1FrameIntervalGui.setup("F.I. RUN_TO_WALK_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runToWalk2FrameIntervalGui.setup("F.I. RUN_TO_WALK_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(turnFrameIntervalGui.setup("F.I. TURN", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(turnToRunFrameIntervalGui.setup("F.I. TURN_TO_RUN", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkTurn1FrameIntervalGui.setup("F.I. WALK_TURN_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(walkTurn2FrameIntervalGui.setup("F.I. WALK_TURN_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runTurn1FrameIntervalGui.setup("F.I. RUN_TURN_1", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(runTurn2FrameIntervalGui.setup("F.I. RUN_TURN_2", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(idleToWalkFrameIntervalGui.setup("F.I. IDLE_TO_WALK", 0.1f, 0.01, 0.5));
    frameIntervalGroup.add(idleToRunFrameIntervalGui.setup("F.I. IDLE_TO_RUN", 0.1f, 0.01, 0.5));
    
    // AÑADIR A GUI
    gui.add(&frameIntervalGroup);
    
    // ************************ DEBUG ************************
    // CERRADO POR DEFECTO
    frameIntervalGroup.minimize();
   
    
    // ==============================================================================================================================
    // CONFIGURACIÓN GRUPO: [INTERACTORS]
    // ==============================================================================================================================
    interactorsGroup.setup("INTERACTORS DETECT");
    // Creamos tantos botones como Interactors existen
    updateInteractorsGroup(interactorsGroup);
    // Añadimos el grupo a la GUI
    gui.add(&interactorsGroup);
    
    // ************************ DEBUG ************************
    // CERRADO POR DEFECTO
    interactorsGroup.minimize();

    
    // ==============================================================================================================================
    // LIVE PANEL
    // ==============================================================================================================================
    currentRowGui = "0";
    currentRegionGui = "0";
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    currentVelocityXGui = "0.0";
    // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    currentMovementNameGui = "IDLE";
    nextOutRegionGui = "";
    currentStateGui = "IDLE";
    targetStateGui = "IDLE";
    controlKeysGui = "NONE";
    currentIntentionGui = "NO WANTS";
    livePanelPosition = physicsManager.getPosition();
    
    

    // ==========================================
    // INICIALIZAR SLIDERS
    // ==========================================
    // scaleFactorGui = movementManager.getScaleFactor(); ????
    currentVelocityXGui = std::to_string(physicsManager.getVelocity().x);
    maxSpeedWalkGui = physicsManager.getMaxSpeedWalk();
    maxSpeedRunGui = physicsManager.getMaxSpeedRun();
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
    idleToWalkFrameIntervalGui = movementManager.getMovementFrameInterval("IDLE_TO_WALK");
    idleToRunFrameIntervalGui = movementManager.getMovementFrameInterval("IDLE_TO_RUN");
    
    

    
    // ==========================================
    // LISTENNERS SLIDERS
    // ==========================================
    frameIntervalGui.addListener(this, &GUIManager::onFrameIntervalChanged);
    scaleFactorGui.addListener(this, &GUIManager::onScaleFactorChanged);
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    maxSpeedWalkGui.addListener(this, &GUIManager::onMaxSpeedWalkChanged);
    maxSpeedRunGui.addListener(this, &GUIManager::onMaxSpeedRunChanged);
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    hitBoxWGui.addListener(this, &GUIManager::onHitBoxWChanged);
    hitRayFloorXGui.addListener(this, &GUIManager::onHitRayFloorXChanged);
    
    // FRAME INTERVALS
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
    idleToWalkFrameIntervalGui.addListener(this, &GUIManager::onIdleToWalkFrameIntervalChanged);
    idleToRunFrameIntervalGui.addListener(this, &GUIManager::onIdleToRunFrameIntervalChanged);

}

void GUIManager::update() {
    
    // ==============================================================================================================================
    // UPDATE GRUPO: [SETTERS]
    // ==============================================================================================================================
    // Actualiza en el FrameInterval del gestor de movimientos los valores de la GUI
    movementManager->setFrameIntervalFromGUI();
    
    
    // ==============================================================================================================================
    // UPDATE GRUPO: [F.I.]
    // ==============================================================================================================================
    // Actualiza Frame Interval Actual en la GUI como String
    // TO DO se debería ver el FramInterval Global, no el que se esta usando en el moviento.
    currentMovementFrameIntervalGui = std::to_string(movementManager->getCurrentMovementFrameInterval());
    
    // ==============================================================================================================================
    // UPDATE GRUPO: [INTERACTORS]
    // ==============================================================================================================================
    if(interactorsGroup.isMinimized()){
        resetInteractors();
    }
    
    
    // ==============================================================================================================================
    // UPDATE LIVE PANEL
    // ==============================================================================================================================
    updateLivePanel();
    
}


void GUIManager::draw() {
    gui.draw();
    drawLivePanel(livePanelPosition);
}


// ==============================================================================================================================
// FUNCIONES GRUPO: [INTERACTORS]
// ==============================================================================================================================
// *** ACTUALIZAR BOTONES DE INTERACTORS ***
void GUIManager::updateInteractorsGroup(ofxGuiGroup &group) {
    // 1. LIMPIEZA: Borramos los botones antiguos de la memoria y del grupo
    for (auto btn : dynamicButtons) {
        delete btn; // Liberamos la RAM
    }
    dynamicButtons.clear();
    group.clear(); // Limpiamos visualmente el grupo
    
    // 2. RE-CONFIGURAR: El grupo necesita un setup para resetear su cabecera
    group.setup("INTERACTORS DETECT");

    // 3. ITERAR Y CREAR: Recorremos los interactores del CollisionManager
    for (const auto & inter : collisionManager->getInteractors()) {
        addDynamicButton(inter.name);
    }
    
    // 4. ACTUALIZAR: Forzamos a la GUI a recalcular posiciones
    //group.sizeChanged();
}

// *** AÑADIR BOTON A INTEREACTORS ***
void GUIManager::addDynamicButton(string name) {
    // Creamos el botón en el "heap" (memoria dinámica)
    ofxToggle* btn = new ofxToggle();
    
    // Setup del boton
    btn->setup(name, false);
    
    // REGISTRAMOS EL EVENTO
    btn->addListener(this, &GUIManager::onInteractorButtonPressed);
    
    // Añadimos el boton al grupo
    interactorsGroup.add(btn);
    
    // Lo guardamos en nuestro vector para poder borrarlo luego
    dynamicButtons.push_back(btn);
}

// *** ELIMINAR BOTONES DE INTEREACTORS ***
void GUIManager::clearDynamicButtons() {
    // Quita los botones del panel
    interactorsGroup.clear();
    // Recorremos los botones creados
    for (auto btn : dynamicButtons) {
        // Borra el objeto de la memoria
        delete btn;
    }
    dynamicButtons.clear();
    interactorsGroup.setup("INTERACTORS DETECT");
}

// *** PONER TODOS LOS INTERACTORS A FALSE ***
void GUIManager::resetInteractors() {
    // Recorremos los botones creados
    for (auto btn : dynamicButtons) {
        // Lo ponemos a False
        btn->getParameter().cast<bool>().set(false);
        // Desactivamos la visualización con el parámetro de interactor "gui"
        collisionManager->setInteractorGUI(btn->getName(), false);
    }
}



// ==============================================================================================================================
// FUNCIONES LIVE PANEL
// ==============================================================================================================================
// *** UPDATE LIVE PANEL ***
void GUIManager::updateLivePanel() {
    // Actualiza Fila Actual en la GUI como String
    currentRowGui = std::to_string(movementManager->getCurrentRow());
    
    // Actualiza Región Actual en la GUI como String
    currentRegionGui = std::to_string(movementManager->getCurrentRegion());
    
    // Obtiene la siguiente región a llegar antes de transición en String; si es -1, asigna "N/A"
    if (! movementManager->isWaitingForTransition()) {
        nextOutRegionGui = "-";
    } else {
        nextOutRegionGui = movementManager->getNextOutRegion() == -1 ? "N/A" : std::to_string(movementManager->getNextOutRegion());
    }
    
    // Actualiza Estado Actual en la GUI como String
    currentStateGui = movementManager->getCurrentState();
    
    // Actualiza Estado Objetivo en la GUI como String
    targetStateGui = movementManager->getTargetState();
    
 
    // Actualizar las teclas presionadas
    controlKeysGui = inputManager->getPressedKeysAsString();

    // Actualizar las intenciones
    currentIntentionGui = inputManager->getIntentsAsString();
    
    // Actualiza el nombre del movimiento actual en la GUI
    currentMovementNameGui = movementManager->getCurrentMovementName();
    
    // Actualizar la velocidad actual del personaje en la GUI, es la base sin escalado
    currentVelocityXGui = std::to_string(physicsManager->getVelocity().x);
    
    // Posicion
    livePanelPosition = physicsManager->getPosition();
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Añadir un label con la velocidad escalada?
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
}

// *** DRAW LIVE PANEL ***
void GUIManager::drawLivePanel(ofVec2f position) {
    // Definimos dimensiones fijas para poder usarlas en los cálculos
    float panelW = 300;
    float panelH = 300;
    float offsetRectangle = 15; // Un poco más de margen para el panel grande

    // Calculamos la posición centrada sobre el personaje
    ofVec2f draw_position = position;
    // Centramos el panel de 300px restando la mitad de su ancho
    draw_position.x = position.x - (panelW / 2);
    draw_position.y = position.y - collisionManager->getBaseHitbox().regionH / 2 - (panelH + 20);

    ofPushStyle();
    ofPushMatrix();
    ofTranslate(draw_position.x, draw_position.y);
    
    // --- FONDO PRINCIPAL DEL PANEL ---
    ofSetColor(0, 0, 0, 160); // Un pelín más opaco para que se lea mejor
    ofDrawRectangle(0, -20, panelW, panelH);
    
    // --- NIVEL 1: TECLAS (BANNER TOTAL) ---
    string keys = controlKeysGui;
    if(keys == "") keys = "NONE";

    ofSetColor(209, 169, 6);
    // Dibujamos el rectángulo amarillo que ocupa TODO el ancho (panelW)
    if(keys != "No Pressed"){
        ofFill();
    }
    ofDrawRectangle(0, -20, panelW, 30); // 30px de alto para que respire
    ofNoFill();
    
    // Texto sobre el banner amarillo
    ofSetColor(0);
    // Ajustamos la Y para que quede centrado en el banner de 30px
    ofDrawBitmapString("KEYS: [ " + keys + " ]", offsetRectangle, 0);
        
    // --- NIVEL 2: ESTADOS ---
    ofSetColor(255);
    int yOffset = 45; // Empezamos más abajo porque el banner es más alto
    // Aumentamos el interlineado a 20 para aprovechar los 300px de alto
    ofDrawBitmapString("STATE:  " + currentStateGui, offsetRectangle, yOffset);
    if(currentIntentionGui != "NO WANTS"){
        ofSetColor(179, 47, 245);
    }
    ofDrawBitmapString("INTENT: " + currentIntentionGui, offsetRectangle, yOffset + 20);
    ofSetColor(255);
    ofDrawBitmapString("TARGET: " + targetStateGui, offsetRectangle, yOffset + 40);

    // --- NIVEL 3: MOV / ROW / REGION ---
    yOffset += 85;
    
    string movName = currentMovementNameGui;
    bool isTransition = movementManager->getCurrentMovement()->isTransition;
    
    ofSetColor(isTransition ? ofColor::red : ofColor::green);
    ofDrawBitmapString("MOV: " + movName, offsetRectangle, yOffset);
    
    ofSetColor(200);
    string rowRegion = "ROW: " + (string)currentRowGui + " | REG: " + (string)currentRegionGui;
    ofDrawBitmapString(rowRegion, offsetRectangle, yOffset + 20);

    // --- NIVEL 4: NEXT REGION & VELOCITY ---
    yOffset += 70;
    
    string nextReg = nextOutRegionGui;
    ofColor regColor;
    if (nextReg == "1") {
        regColor = ofColor::dodgerBlue;
    } else if (nextReg == "5") {
        regColor = ofColor::orange;
    } else {
        regColor = ofColor::white;
    }
    
    ofSetColor(regColor);
    ofDrawBitmapString("NEXT OUT: " + nextReg, offsetRectangle, yOffset);

    // VELOCIDAD X (Texto simple al 60%)
    ofSetColor(255, 255, 255, 150);
    string velText = "VEL X: " + (string)currentVelocityXGui;
    ofDrawBitmapString(velText, offsetRectangle, yOffset + 20);

    // OPCIONAL: Una línea decorativa al final para cerrar el panel
    ofSetColor(255, 255, 255, 30);
    ofDrawLine(0, panelH - 25, panelW, panelH - 25);

    ofPopMatrix();
    ofPopStyle();
}


#pragma region Callbacks para los sliders
// ==============================================================================================================================
// LISTENNERS GRUPO: [INTERACTORS]
// ==============================================================================================================================
void GUIManager::onInteractorButtonPressed(bool& value) {
    
    // Buscamos qué botón ha disparado el evento (comparando por dirección de memoria)
    ofxToggle* botonAccionado = nullptr;
    
    for (auto btn : dynamicButtons) {
        if (&btn->getParameter().cast<bool>().get() == &value) {
            botonAccionado = btn;
            break;
        }
    }

    // Si no encontramos el botón por lo que sea, salimos
    if (!botonAccionado) return;

    // Si el botón se ha ENCENDIDO (value == true)
    if (value) {
        cout << "\n>>> SELECCIONADO: " << botonAccionado->getName() << endl;
        
        // Activamos visualizacion de interactor
        collisionManager->setInteractorGUI(botonAccionado->getName(), true);

        // APAGAMOS los demás (Lógica Radio Button)
        for (auto btn : dynamicButtons) {
            if (btn != botonAccionado) {
                // Importante: .set(false) enviará otro evento de callback,
                // pero como value será false, entrará en el bloque de abajo y limpiará los GUI
                btn->getParameter().cast<bool>().set(false);
            }
        }
    }
    // 3. Si el botón se ha APAGADO (value == false)
    else {
        // cout << ">>> DESELECCIONADO: " << botonAccionado->getName() << endl;
        // Desactivamos visualizacion de interactor
        collisionManager->setInteractorGUI(botonAccionado->getName(), false);
    }
}

// ==============================================================================================================================
// LISTENNERS GRUPO: [SETTERS]
// ==============================================================================================================================
void GUIManager::onScaleFactorChanged(float& value) {
    // Al escalar, Reposicionar el personaje, media región arriba de la posición actual
    physicsManager->setPositionY(physicsManager->getPosition().y - (spriteSheetManager->getRegionHeight()/2) * value);
    
    // Actualitzar todos los factores de escala
    movementManager->setScaleFactor(value);
}

void GUIManager::onFrameIntervalChanged(float& value) {
    movementManager->setFrameInterval(value);
}

// $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
void GUIManager::onMaxSpeedWalkChanged(float& value) {
    physicsManager->setMaxSpeedWalk(value);
}
void GUIManager::onMaxSpeedRunChanged(float& value) {
    physicsManager->setMaxSpeedRun(value);
}

// >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
void GUIManager::onHitBoxWChanged(float& value) {
    collisionManager->setHitboxWidth(value);
}
void GUIManager::onHitRayFloorXChanged(float& value) {
    collisionManager->setHitRayXFloor(value);
}


// ==============================================================================================================================
// LISTENNERS GRUPO: [F.I.]
// ==============================================================================================================================
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

void GUIManager::onIdleToWalkFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("IDLE_TO_WALK", value);
}

void GUIManager::onIdleToRunFrameIntervalChanged(float& value) {
    movementManager->setMovementFrameInterval("IDLE_TO_RUN", value);
}

#pragma endregion


// DESTRUCTOR
GUIManager::~GUIManager() {
    // 1. Limpiamos la parte visual de la GUI
    interactorsGroup.clear();
    
    // 2. Liberamos la memoria de cada botón que creamos con 'new'
    for (auto btn : dynamicButtons) {
        if (btn != nullptr) {
            delete btn;
        }
    }
    
    // 3. Vaciamos el vector por seguridad
    dynamicButtons.clear();
    
    ofLogNotice("GUIManager") << "Memoria de botones dinámicos liberada correctamente.";
}
