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
        ofVec2f(1200, 0),             // Punto inicial (arriba)
        ofVec2f(1200, ofGetHeight()), // Punto final (abajo)
        InteractorType::WALL,                 // Es una PARED
        "actLimitR"
    ));
}

void CollisionManager::update() {
    // Aquí irá la lógica de comprobación más adelante
}

void CollisionManager::draw() {
    
    // Recorremos todos los interactores
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

Interactor* CollisionManager::checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, SpriteSheetManager& sprite, bool isFacingRight) {
    
    // --- HIT WALL---
    float offsetX = -sprite.getRegionWidth()/ 2.0f;
    
    // Posición futura para anticipar el choque
    float futuroCentroX = currentPos.x + velocity.x;
    
    float currentSensorX = currentPos.x + offsetX + sprite.getHitboxW();
    float futureSensorX = (currentPos.x + velocity.x) + offsetX + sprite.getHitboxW();

    
    // --- LÓGICA DE DIRECCIÓN ---
        if (isFacingRight) {
            // Sensor en el borde DERECHO (Línea roja estándar)
            currentSensorX = currentPos.x + offsetX + sprite.getHitboxW();
            futureSensorX = (currentPos.x + velocity.x) + offsetX + sprite.getHitboxW();
        }
        else {
            // Sensor en el borde IZQUIERDO (Espejo)
            // Si el dibujo se voltea, el sensor ahora mide hacia la izquierda desde el centro
            // Nota: El +100 que pusimos en el draw también debe reflejarse aquí si quieres precisión total
            float ajusteEspejo = -100.0f;
            currentSensorX = currentPos.x - (offsetX + sprite.getHitboxW()) + ajusteEspejo;
            futureSensorX = (currentPos.x + velocity.x) - (offsetX + sprite.getHitboxW()) + ajusteEspejo;
        }
    // 3. Revisamos los interactores
    for (auto &inter : interactors) {
        
        // Solo nos interesan las paredes (WALL)
        if (inter.type == InteractorType::WALL) {
            float wallX = inter.p1.x; // La coordenada X de la pared (ej: 1200 o 0)

            // --- DETECCIÓN PARA LA PARED DERECHA ---
            if (inter.name == "actLimitR") {
                // Si antes estábamos a la izquierda y ahora estaríamos a la derecha (o justo encima)
                if (currentSensorX <= wallX && futureSensorX >= wallX) {
                    return &inter; // ¡Impacto!
                }
            }

            // --- DETECCIÓN PARA LA PARED IZQUIERDA ---
            // (Nota: Para la izquierda el sensor suele ser el otro lado del sprite,
            // pero por ahora usemos la misma lógica si solo quieres probar la derecha)
            if (inter.name == "actLimitL") {
                if (currentSensorX >= wallX && futureSensorX <= wallX) {
                    return &inter;
                }
            }
        }
        
        
        // --- 2. DETECCIÓN DE SUELO (SURFACE) ---
        if (inter.type == InteractorType::SURFACE) {
            float groundY = inter.p1.y;
            float groundMinX = min(inter.p1.x, inter.p2.x);
            float groundMaxX = max(inter.p1.x, inter.p2.x);

            // 1. Calculamos la X exacta del rayo (Copiando tu lógica de giro)
            float localRayX;
            if (isFacingRight) {
                localRayX = -150.0f + sprite.getHitRayXFloor();
            } else {
                float ajusteEspejo = -100.0f;
                localRayX = -(-150.0f + sprite.getHitRayXFloor()) + ajusteEspejo;
            }
            
            float worldRayX = currentPos.x + localRayX;

            // 2. Calculamos la Y de los pies (Actual y Futura)
            float feetY = currentPos.y - 150.0f + 10 + sprite.getHitboxH();
            float futureFeetY = (currentPos.y + gravity) - 150.0f + 10 + sprite.getHitboxH();

            // Imprimimos la situación de la Y antes de los IFs
                cout << "--- EVALUANDO Y (Pies) ---" << endl;
                cout << "  [1] Y Actual:  " << feetY << endl;
                cout << "  [2] Y Futura:  " << futureFeetY << endl;
                cout << "  [3] Y Suelo:   " << groundY << endl;

                // Condición 1: ¿Estoy por encima o justo en la línea ahora?
                bool actualmenteArriba = (feetY <= groundY);
                // Condición 2: ¿En el próximo frame estaré por debajo o justo en la línea?
                bool proximoAbajo = (futureFeetY >= groundY);

                cout << "  ¿Actual Arriba? " << (actualmenteArriba ? "SI" : "NO") << endl;
                cout << "  ¿Proximo Abajo? " << (proximoAbajo ? "SI" : "NO") << endl;

            
            // 3. COMPROBACIÓN LÓGICA
            if (feetY <= groundY) {
                // ...pero en el siguiente paso estaría abajo (o justo encima)
                if (futureFeetY >= groundY) {
                    
                    // ¡DETECCIÓN PREVENTIVA!
                    // No esperamos a que atraviese. Avisamos de que el futuro es peligroso.
                    cout << "[DEBUG] Colisión inminente detectada. Frenando antes de cruzar." << endl;
                    return &inter;
                }
            }
            
        }
    }

    return nullptr;
}
