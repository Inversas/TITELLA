#include "CollisionManager.h"

void CollisionManager::setup() {
    
    // 1. Definimos el suelo (desde la esquina inferior izquierda a la derecha)
    interactors.push_back(Interactor(
        ofVec2f(0, 700),            // Punto inicial (0, alto)
        ofVec2f(ofGetWidth(), 700), // Punto final (ancho, alto)
        InteractorType::SURFACE,              // Es un SUELO
        "actGround"
    ));

    // 2. Definimos el límite izquierdo (pared vertical en x = 0)
    interactors.push_back(Interactor(
        ofVec2f(0, 0),             // Punto inicial (arriba)
        ofVec2f(0, ofGetHeight()), // Punto final (abajo)
        InteractorType::WALL,      // Es una PARED
        "actLimitL"
    ));

    // 3. Definimos el límite derecho (pared vertical en x = ancho)
    interactors.push_back(Interactor(
        ofVec2f(ofGetWidth(), 0),             // Punto inicial (arriba)
        ofVec2f(ofGetWidth(), ofGetHeight()), // Punto final (abajo)
        InteractorType::WALL,                 // Es una PARED
        "actLimitR"
    ));
}

void CollisionManager::update() {
    // Aquí irá la lógica de comprobación más adelante
}

void CollisionManager::draw() {
    // Recorremos todos los interactores que creamos en el setup
    for (auto &inter : interactors) {
        
        if (inter.type == InteractorType::SURFACE) {
            ofSetColor(180, 120, 255); // Lila
        }
        else if (inter.type == InteractorType::WALL) {
            ofSetColor(255, 0, 0);     // Rojo
        }
        else if (inter.type == InteractorType::BUTTON) {
            ofSetColor(100, 200, 255); // Celeste
        }

        // Dibujamos la línea que une los dos puntos
        ofDrawLine(inter.p1.x, inter.p1.y, inter.p2.x, inter.p2.y);
    }

    // 2. Dibujamos los nombres moviendo el texto según la pared
    ofSetColor(255); // Texto en blanco
    for (auto &inter : interactors) {
        
        float textX = inter.p1.x;
        float textY = inter.p1.y + 20; // Bajamos un poco el texto para que no pise el borde superior

        if (inter.name == "actLimitR") {
            // Si es la pared derecha, movemos el texto a la izquierda de la línea
            // Restamos unos 80 píxeles (aprox. lo que ocupa la palabra)
            textX = inter.p1.x - 85;
        }
        else {
            // Para el suelo y la pared izquierda, lo movemos un poco a la derecha
            textX = inter.p1.x + 10;
        }

        ofDrawBitmapString(inter.name, textX, textY);
    }
}
