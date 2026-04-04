#pragma once
#include <memory>

#include "ofMain.h" // Necesario para ofVec2f y funciones matemáticas básicas

#include "CollisionManager.h"
#include "GUIManager.h"

// Definimos los estados posibles de la edición
enum EditorState {
    EDITOR_WAITING_P1,        // Esperando el primer clic (P1)
    EDITOR_WAITING_P2   // P1 fijado, moviendo el ratón para fijar P2
};

// ****************************************** CLASE EDITOR MANAGER ****************************************** //
// Clase que gestiona el modo edición para crear Interactors
class EditorManager {
    
public:
    // CONSTRUCTOR
    EditorManager();
    // DESTRUCTOR
    ~EditorManager();
    
    // *** SETUP ***
    // Inicializa los valores por defecto ()
    void setup(CollisionManager* _collisionManager, GUIManager* _guiManager);
    
    // *** UPDATE ***
    void update(int mouseX, int mouseY);

    // *** DRAW ***
    void draw();
    
    
    // *** INPUT HANDLERS ***
    void handleMousePressed(int x, int y, int button);
    void handleKeyPressed(int key);

    // *** LÓGICA DE EDICIÓN ***
    // Elimina un interactor y refresca la GUI
    void deleteInteractor(string name);
    
    // *** PUNTO DE IMAN ***
    ofVec2f getSnappedPoint(ofVec2f rawPos);
    
    // *** RESTRICCION HORIZOTNAL / VERTICAL ***
    ofVec2f applyAxialRestriction(ofVec2f start, ofVec2f end);
    

    
    // ------------------- GETTERS - SETTERS -------------------

    // *** GETTERS ***
    bool getEnabled() const;
    EditorState getState() const;
    bool hasPendingChanges() const;

    // *** SETTERS ***
    void setEnabled(bool value);
    void setChangesProcessed();

private:
    
    // *** VARIABLES  ***

    // Punteros a dependencias
    CollisionManager* collisionManager;
    GUIManager* guiManager;

    // Encendido / Apagado del editor
    bool editMode;
    // Waiting P1 o P2
    EditorState currentState;
    // Flag para avisar a ofApp que debe refrescar la GUI
    bool bNeedsGuiUpdate;

    // Puntos de geometría
    ofVec2f p1;              // Punto de origen fijado
    ofVec2f p2Preview;       // Punto final calculado (con snap y restricción)
    ofVec2f snappedMousePos; // Posición del ratón tras pasar por el filtro de "Snap"

    // Configuración visual
    float snapTolerance;     // Distancia en píxeles para que el imán actúe
    ofColor guideLineColor;  // Color de la línea elástica (Amarillo)
    
    
    // *** MÉTODOS INTERNOS ***
    // Helper para generar nombres únicos de interactores (ej: "Wall_1", "Wall_2")
    string generateUniqueName(InteractorType type);
};
