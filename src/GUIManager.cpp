#include "GUIManager.h"

#include "MovementManager.h"
#include "SpriteSheetManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "EditorManager.h"

void GUIManager::setup(MovementManager& movementManager, SpriteSheetManager& spriteSheetManager, InputManager& inputManager, PhysicsManager& physicsManager, CollisionManager& collisionManager, EditorManager& editorManager) {
    
    // *** REFERENCIAS A GESTORES *** //
    this->movementManager = &movementManager;        // Guardamos la referencia a MovementManager
    this->spriteSheetManager = &spriteSheetManager;  // Guardamos la referencia a SpriteSheetManager
    this->inputManager = &inputManager;              // Guardamos la referencia a InputManager
    this->physicsManager = &physicsManager;          // Guardamos la referencia a PhysicsManager
    this->collisionManager = &collisionManager;      // Guardamos la referencia a CollisionManager
    this->editorManager = &editorManager;            // Guardamos la referencia a EditorManager
    
    // Inicializar GUI
    gui.setup("Settings");
    gui.setPosition(ofGetWidth() - 220, 30);
    
    
    // ==============================================================================================================================
    // CONFIGURACIÓN GRUPO: [SETTERS]
    // ==============================================================================================================================
    settersGroup.setup("SETTERS");
    
    // GLOBAL SLIDERS
    settersGroup.add(frameIntervalGui.setup("F.I. Global", movementManager.getFrameInterval(), 0.01, 0.5));
    settersGroup.add(scaleFactorGui.setup("Scale Factor", 1.0f, 0.5, 3.0));
    //scaleFactorGui = movementManager.getScaleFactor(); ????

    
    // FISICAS SLIDERS $$$$$$$$$$$$$
    settersGroup.add(maxSpeedWalkGui.setup("Max Speed Walk", physicsManager.getMaxSpeedWalk(), 0.0, 15.0));
    settersGroup.add(maxSpeedRunGui.setup("Max Speed Run", physicsManager.getMaxSpeedRun(), 0.0, 25.0));

    // COLLISION SLIDERS >>>>>>>>>>>>
    settersGroup.add(hitBoxWGui.setup("HitBox W", collisionManager.getHitbox().width, 100, 400));
    settersGroup.add(hitRayFloorXGui.setup("HitRayFloor X", collisionManager.getHitbox().floorRayX, 0, 200));
    
    // AÑADIR A GUI
    // Sin & estaríamos pasando una copia
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
    
    // ==========================================
    // INICIALIZAR TODOS LOS SLIDERS F.I.
    // ==========================================
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
    updateInteractorsGroup();
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
    

    
    // ==============================================================================================================================
    // LISTENNERS TODOS SLIDERS
    // ==============================================================================================================================
    // [SETTERS]
    frameIntervalGui.addListener(this, &GUIManager::onFrameIntervalChanged);
    scaleFactorGui.addListener(this, &GUIManager::onScaleFactorChanged);
    
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    maxSpeedWalkGui.addListener(this, &GUIManager::onMaxSpeedWalkChanged);
    maxSpeedRunGui.addListener(this, &GUIManager::onMaxSpeedRunChanged);
    
    // >>>>>>>>>>>> COLISIONES >>>>>>>>>>>>
    hitBoxWGui.addListener(this, &GUIManager::onHitBoxWChanged);
    hitRayFloorXGui.addListener(this, &GUIManager::onHitRayFloorXChanged);
    
    // [F.I.]
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
    // TO DO se debería ver el FramInterval Global, no el que se esta usando en el moviento. ???
    currentMovementFrameIntervalGui = std::to_string(movementManager->getCurrentMovementFrameInterval());
    
    // ==============================================================================================================================
    // UPDATE GRUPO: [INTERACTORS]
    // ==============================================================================================================================
    if(interactorsGroup.isMinimized()){
        resetInteractorsButtons();
    }
    
    
    // ==============================================================================================================================
    // UPDATE LIVE PANEL
    // ==============================================================================================================================
    updateLivePanel();
    
}


void GUIManager::draw() {
    // Dibuja la GUI, metodo del objeto "gui"
    gui.draw();
    // Dibuja el Live Panel
    drawLivePanel(livePanelPosition);
}

void GUIManager::drawEdit() {
    // Dibuja solo el grupo Interactors
    interactorsGroup.draw();
}


// ==============================================================================================================================
// FUNCIONES GRUPO: [INTERACTORS]
// ==============================================================================================================================
// *** ACTUALIZAR BOTONES DE INTERACTORS ***
void GUIManager::updateInteractorsGroup() {
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // INFORMACION IMPORTANTE PARA REVISAR la gestión de memoria
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // Es un vector de punteros (vector<ofxToggle*>).
    // En C++, cuando creas algo con la palabra new (como hacemos al crear los botones),
    // ese objeto vive en una zona de la memoria llamada Heap.
    // Los objetos en el Heap no se borran solos. Si simplemente vaciáramos el vector,
    // los botones seguirían existiendo en la RAM, pero perderíamos su dirección (serían "objetos fantasma").
    // delete destruye el objeto físicamente y libera esos bytes.
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬

    // *** LIMPIEZA ***
    // Borramos los botones antiguos de la memoria y del grupo
    clearDynamicButtons();

    // *** ITERAR Y CREAR ***
    // Añadimos un rastreador de índice
    int indiceBoton = -1;
    // Recorremos los interactores del CollisionManager
    for (const auto & inter : collisionManager->getInteractors()) {
        indiceBoton++;
        addDynamicButton(inter.name, indiceBoton);
    }
    
    // Forzamos a la GUI a recalcular posiciones
    //group.sizeChanged();
    
    // Si se minimiza la seccion ITERACTORS
    if(interactorsGroup.isMinimized()){
        // Poner Botones a False
        resetInteractorsButtons();
    }
}

void  GUIManager::setNameInteractorsGroup(const string& groupName){
    // Si el nombre ya es el mismo, no hace falta llamar a setName (ahorro extra)
    if(interactorsGroup.getName() != groupName) {
        interactorsGroup.setName(groupName);
    }
}


// *** ABRIR / CERRAR ITERACTORS **
void GUIManager::setMaximizeInteractors(){
    interactorsGroup.maximize();
}
void GUIManager::setMinimizeInteractors(){
    interactorsGroup.minimize();
}

// *** CARGA SETTINGS ESPECIFICO ***
void GUIManager::loadSettingsFromFile(const std::string& path) {
    // Carga el settings.xml de la ruta
    gui.loadFromFile(path);
    ofLogNotice("GUIManager") << "Configuración cargada desde: " << path;
}
// *** GUARDA SETTINGS ESPECIFICO ***
void GUIManager::saveSettingsToFile(const string& path) {
    // Guarda en el settings.xml de la ruta
    gui.saveToFile(path);
    ofLogNotice("GUIManager") << "Settings exportados a: " << path;
}



// *** AÑADIR BOTON A INTEREACTORS ***
void GUIManager::addDynamicButton(string name, int indiceBoton) {
    
    // Creamos el botón en el "heap" (memoria dinámica)
    ofxToggle* btn = new ofxToggle();
    
    // Setup del boton
    btn->setup(name, false);
    
    // REGISTRAMOS EL EVENTO
    btn->addListener(this, &GUIManager::onInteractorButtonPressed);
    
    // Color Rojo para los que se pueden eliminar
    if(editorManager && editorManager->getEnabled() && indiceBoton >= 3){
        btn->setBackgroundColor(ofColor(200, 0, 0, 180));
    }
    
    // Añadimos el boton al grupo
    interactorsGroup.add(btn);
    
    // Lo guardamos en nuestro vector para poder borrarlo luego
    dynamicButtons.push_back(btn);
}

// *** ELIMINAR BOTONES DE INTEREACTORS Y RE-CONFIGURAR ***
void GUIManager::clearDynamicButtons() {
    
    // Recorremos los botones creados
    for (auto btn : dynamicButtons) {
        if (btn != nullptr) {
            // Liberamos la memoria de cada botón antiguo
            delete btn;
        }
    }
    // dynamicButtons todavía guarda las "direcciones postales" (punteros) de donde estaban esos botones, lo limpiamos
    dynamicButtons.clear();
    
    // Limpiamos visualmente el grupo
    interactorsGroup.clear();
    
    //El grupo necesita un setup para resetear su cabecera
    interactorsGroup.setup("INTERACTORS DETECT");
}

// *** PONER TODOS LOS INTERACTORS A FALSE ***
void GUIManager::resetInteractorsButtons() {
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
    
    // --- CONFIGURACIÓN DE DIMENSIONES ---
    // Rectángulo negro de fondo
    float panelW = 300;
    float panelH = 250;
    // Margen interno izquierdo para el texto
    float offsetRectangle = 15;
    // Margen personaje - panel (Es el aire que hay entre la cabeza del personaje y el borde inferior del panel)
    float headMargin = 15;
    // Distancia entre secciones
    float lineSection = 40;
    // Distancia entre líneas de texto
    float lineHeight = 20;
    

    // --- CÁLCULO DE POSICIÓN (Flotando sobre el personaje) ---
    // Copiamos la posición recibida (la del personaje)
    ofVec2f draw_position = position;
    // Centramos el panel de 300px restando la mitad de su ancho
    draw_position.x = position.x - (panelW / 2);
    // Encima de la cabeza: altura region / 2 - (alto panel + el aire entre la capeza del personaje y el borde inferior del panel)
    draw_position.y = position.y - collisionManager->getBaseHitbox().regionH / 2 - (panelH + headMargin);

    // ##############################################################################################################################
    //                                                  START OF MATRIX
    // ##############################################################################################################################
    // Guardamos el estilo actual de dibujo, para que no afecte fuera lo que hagamos en esta función
    ofPushStyle();
    ofPushMatrix();
    // Movemos el origen de coordenadas
    ofTranslate(draw_position.x, draw_position.y);
    
    
    // ==========================================
    // --- FONDO PRINCIPAL DEL PANEL ---
    // ==========================================
    // Color NEGRO con transparencia
    ofSetColor(0, 0, 0, 160);
    // Dibujamos el fondo (subido -20px para el banner superior)
    ofDrawRectangle(0, 0, panelW, panelH);
    
    
    // ==========================================
    // --- NIVEL 1: TECLAS (BANNER DESTACADO) ---
    // ==========================================
    // Obtenemos el string de teclas del InputManager
    string keys = controlKeysGui;
    // Si está vacío, mostramos "NONE" (esto en principio ya viene bien desde InputManager
    if(keys == "") keys = "NONE";

    // Color MOSTAZA para resaltar la cabecera
    ofSetColor(209, 169, 6);
    // Si hay teclas activas, rellenamos el rectángulo
    if(keys != "No Pressed"){
        ofFill();
    }
    // Dibujamos el banner superior de 30px de alto
    ofDrawRectangle(0, 0, panelW, 30);
    // RESET relleno para el resto de elementos
    ofNoFill();
    
    // Color negro para el texto sobre el fondo MOSTAZA
    ofSetColor(0);
    // Dibujamos el texto centrado en el banner
    ofDrawBitmapString("KEYS: [ " + keys + " ]", offsetRectangle, lineHeight);
    
    // ==========================================
    // --- NIVEL 2: ESTADOS ---
    // ==========================================
    // Color BLANCO para el texto general
    ofSetColor(255);
    // Espacio vertical inicial debajo del banner
    int yOffset = lineSection + lineHeight;
    // Dibujamos el estado de la máquina de estados, a la altura del NIVEL 2
    ofDrawBitmapString("STATE:  " + currentStateGui, offsetRectangle, yOffset);
    
    // Si hay una intención activa, usamos color LILA
    if(currentIntentionGui != "NO WANTS"){
        ofSetColor(179, 47, 245);
    }
    // Dibujamos la intención de movimiento del usuario
    ofDrawBitmapString("INTENT: " + currentIntentionGui, offsetRectangle, yOffset + lineHeight);
    // Volvemos a BLANCO para el texto general
    ofSetColor(255);
    // Dibujamos el estado al que queremos transicionar
    ofDrawBitmapString("TARGET: " + targetStateGui, offsetRectangle, yOffset + lineHeight*2);

    // ==========================================
    // --- NIVEL 3: MOV / ROW / REGION / NEXT REGION ---
    // ==========================================
    // Espacio vertical inicial debajo de la seccion anterior
    yOffset += lineSection*2 + 5;
    
    // Nombre del movimiento actual
    string movName = currentMovementNameGui;
    // Comprobamos si el movimiento es una transición
    bool isTransition = movementManager->getCurrentMovement()->isTransition;
    
    // VERDE si es movimiento base, ROJO si es una transición
    ofSetColor(isTransition ? ofColor::red : ofColor::green);
    // Dibujamos el nombre del movimiento
    ofDrawBitmapString("MOV: " + movName, offsetRectangle, yOffset);
    
    // Color GRIS para datos secundarios
    ofSetColor(200);
    // Mostramos la fila y la región actual del SpriteSheet
    string rowRegion = "ROW: " + (string)currentRowGui + " | REG: " + (string)currentRegionGui;
    // Dibujamos el numero de Region
    ofDrawBitmapString(rowRegion, offsetRectangle, yOffset + lineHeight);
    
    // Almacenamos el punto de salida (PS) programado
    string nextReg = nextOutRegionGui;
    // Creamos una variable para guardar el color
    ofColor regColor;
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Si en el futuro cada movimiento tiene diferentes PS
    // Habrá que crear una función que segun el movimiento y su PS devuelva un color u otro
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // Si es el frame 1
    if (nextReg == "1") {
        // Color AZUL
        regColor = ofColor::dodgerBlue;
    // Si es el frame 5
    } else if (nextReg == "5") {
        // Color NARANJA
        regColor = ofColor::orange;
    // Todo lo demás
    } else {
        // Color BLANCO
        regColor = ofColor::white;
    }
    
    // Aplicamos COLOR seleccionado
    ofSetColor(regColor);
    // Dibujamos el numero de Next Out Region
    ofDrawBitmapString("NEXT OUT: " + nextReg, offsetRectangle,  yOffset + lineHeight*2);

    // ==========================================
    // --- NIVEL 4: VELOCITY ---
    // ==========================================
    // Espacio vertical inicial debajo del banner
    yOffset += lineSection*2;
    // Color BLANCO semitransparente para la velocidad
    ofSetColor(255, 255, 255, 150);
    // Texto con la velocidad actual en el eje X
    string velText = "VEL X: " + (string)currentVelocityXGui;
    // Dibujamos la velocidad en X
    ofDrawBitmapString(velText, offsetRectangle, yOffset);
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // AADIR GROUNDED / WALLED
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    ofPopMatrix();
    // Restauramos el estilo
    ofPopStyle();
    // ##############################################################################################################################
    //                                                  END OF MATRIX
    // ##############################################################################################################################
}


#pragma region Callbacks para los sliders
// ==============================================================================================================================
// LISTENNERS GRUPO: [INTERACTORS]
// ==============================================================================================================================
void GUIManager::onInteractorButtonPressed(bool& value) {
    
    // --- IDENTIFICACIÓN DEL BOTÓN ---
    // Creamos un puntero vacío para guardar el botón que ha hecho clic
    ofxToggle* botonAccionado = nullptr;
    
    // Añadimos un rastreador de índice
    int indiceBoton = -1;
    
    for (auto btn : dynamicButtons) {
        indiceBoton++;
        // TRUCO TÉCNICO: Comparamos la dirección de memoria de la variable 'value'
        // con la del parámetro del botón. Si coinciden, hemos encontrado al "culpable".
        if (&btn->getParameter().cast<bool>().get() == &value) {
            botonAccionado = btn;
            // Ya lo tenemos, salimos del bucle
            break;
        }
    }
    // Si no encontramos el botón por lo que sea, salimos
    if (!botonAccionado) return;
    
    
    // [ EDIT MODE ]
    if (editorManager && editorManager->getEnabled()) {
        // Solo actuar si el toggle se ENCIENDE (clic del usuario)
        if (!value) return;
        
        // SEGURIDAD: Evitamos que el usuario borre los 3 Limites Base
        if (indiceBoton < 3) {
            ofLogWarning("GUIManager") << "Intento de borrar interactor protegido: " << botonAccionado->getName();
            // Opcional: forzamos que el toggle se apague si se quedó encendido por error
            return;
        }
        
        // --- MODO EDICIÓN: EL BOTÓN BORRA ---
        ofLogNotice("GUIManager") << "Modo Edicion: Borrando " << botonAccionado->getName();
        editorManager->deleteInteractor(botonAccionado->getName());
        
        // NOTA: No hace falta refrescar la GUI aquí manualmente,
        // porque EditorManager::deleteInteractor pone bNeedsGuiUpdate a true
        // y tu ofApp ya se encarga de regenerar los botones.
        
    }
    
    // [ DEBUG MODE ]
    else {
        // Si el botón se ha ENCENDIDO (value == true)
        if (value) {
            ofLogNotice("GUIManager") << ">>> SELECCIONADO: " << botonAccionado->getName();
            // Destacamos el interactor
            collisionManager->setInteractorGUI(botonAccionado->getName(), true);
            
            // --- LÓGICA DE 'RADIO BUTTON' (APAGAMOS los demás botones) ---
            for (auto btn : dynamicButtons) {
                if (btn != botonAccionado) {
                    // Importante: .set(false) enviará otro evento de callback,
                    // pero como value será false, entrará en el bloque de abajo y limpiará los GUI
                    btn->getParameter().cast<bool>().set(false);
                }
            }
        }
        
        // --- APAGADO BOTON (value == false) ---
        else {
            // ofLogNotice("GUIManager") << ">>> DESELECCIONADO: " << botonAccionado->getName();

            // Desactivamos visualizacion de interactor
            collisionManager->setInteractorGUI(botonAccionado->getName(), false);
        }
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
