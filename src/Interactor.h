#pragma once

#include "ofMain.h"
#include <string>
#include <vector>

using namespace std;

// Tipos de Interactor
enum class InteractorType {
    SURFACE,
    WALL,
    BUTTON
};

// Estructura para definir un interactor (objeto interactivo)
struct Interactor {
    ofVec2f p1;         // Punto inicial
    ofVec2f p2;         // Punto final
    InteractorType type; // Tipo: SURFACE, WALL o BUTTON
    string name;        // Nombre identificador

    // Constructor para inicializar un interactor (Estilo Movement)
    Interactor(
        ofVec2f p1 = ofVec2f(0,0),
        ofVec2f p2 = ofVec2f(0,0),
        InteractorType type = InteractorType::SURFACE,
        string name = ""
    );
};
