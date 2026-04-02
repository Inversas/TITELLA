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

// +++++++++++++ STRUCT +++++++++++++
// Estructura para definir un interactor (objeto interactivo)
struct Interactor {
    ofVec2f p1;            // Punto inicial
    ofVec2f p2;            // Punto final
    InteractorType type;   // Tipo: SURFACE, WALL o BUTTON
    string name;           // Nombre identificador
    float influenceRadius; // Radio de influencia propio, se usa en WALLS, i se pone como el regionW
    bool hit;              // Estado de contacto
    bool gui;              // Si la GUI pide destacarlo

    // Constructor para inicializar un interactor (Estilo Movement)
    Interactor(
        ofVec2f p1 = ofVec2f(0,0),
        ofVec2f p2 = ofVec2f(0,0),
        InteractorType type = InteractorType::SURFACE,
        string name = "",
       float influenceRadius = 300.0f, // Valor por defecto,
       bool hit = false,
       bool gui = false
    );
};
