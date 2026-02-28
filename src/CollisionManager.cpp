#include "CollisionManager.h"

// ??????
// Aquí sí incluimos el manager del sprite para poder usar sus funciones
#include "SpriteSheetManager.h"

void CollisionManager::setup(float regionW, float regionH) {
    
    // *** HITBOX ***
    currentHitbox.width = regionW - 100;     // 200px
    currentHitbox.height = regionH - 78;     // 222px
    currentHitbox.offsetY = 10.0f;           // El ajuste vertical
    currentHitbox.floorRayX = 97.0f;         // Posición horizontal del rayo de detección del suelo (ajustable por GUI)
    currentHitbox.regionW = regionW;         // Guardamos el ancho de la región para usarlo en los cálculos de colisión
    currentHitbox.regionH = regionH;         // Guardamos la altura de la región para usarlo en los cálculos de colisión
    
    
    
    // *** INTERACTORS ***
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
    drawInteractors();
}

void CollisionManager::drawInteractors() {
    
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

    // Dibujamos los nombres moviendo el texto según la pared
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

// DETECCIÓN DE COLISIONES A FUTURO
CollisionResult CollisionManager::checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, bool isFacingRight) {
    
    // ==========================================
    // FASE 1: CÁLCULO DE SENSORES, posiciones futuras
    // ==========================================
    
    //Resultados Frame Actual
    CollisionResult result;
    // Sensores actuales y proyectados al futuro
    SensorState sensors = calculateSensors(currentPos, velocity, gravity, isFacingRight);
    
    
    // ==========================================
    // FASE 2: BUCLES DE DETECCIÓN DE INTERACTORES
    // ==========================================
    
    // Recorremos todos los Interactors
    for (auto &inter : interactors) {
        
        // WALL
        if (inter.type == InteractorType::WALL) {
            
            // Si hay colisión
            if (checkWallCollision(inter, sensors)) {
                result.wall = &inter;
            }
        }

        // SURFACE
        if (inter.type == InteractorType::SURFACE) {
            // Si hay colisión
            if (checkFloorCollision(inter, sensors)) {
                result.floor = &inter;
            }
        }
    }

    return result;
    
}
    

// CALCULA SENSORES FUTUROS
//Traduce las coordenadas locales del personaje al mundo real.
SensorState CollisionManager::calculateSensors(ofVec2f pos, ofVec2f vel, float grav, bool isFacingRight) {
    
    SensorState s;
    HitboxData h = currentHitbox;
    
    // Colocar calculos en el centro de la región
    // Representa la esquina izquierda de nuestra caja de 300x300
    float drawX = -h.regionW / 2.0f;

    // Paredes
    if (isFacingRight) {
        // El sensor está en: Mi Centro + Inicio del dibujo + Ancho de mi Hitbox
        s.wallCurrent = pos.x + drawX + h.width;
        // Lo mismo pero con la velocidad aplicada
        s.wallFuture  = (pos.x + vel.x) + drawX + h.width;
    } else {
        // De momento compesnamos el desplazamiento visual del sprite al girar
        float ajuste = -100.0f;
        s.wallCurrent = pos.x - (drawX + h.width) + ajuste;
        s.wallFuture  = (pos.x + vel.x) - (drawX + h.width) + ajuste;
    }

    // Suelo
    // El rayo X se calcula a partir del centro del personaje más un desplazamiento específico (floorRayX)
    float localRayX;

    if (isFacingRight) {
        // El rayo X está a una distancia 'floorRayX' desde el borde izquierdo (drawX)
        localRayX = drawX + h.floorRayX;
    }
    else {
        // Lógica de Espejo: Invertimos la posición respecto al centro y aplicamos el ajuste
        float ajuste = -100.0f;
        localRayX = -(drawX + h.floorRayX) + ajuste;
    }
    
    // Posición Y de los pies, actual y futura:
    // Bajamos desde el centro (drawX es -150) + margen de seguridad (10) + altura hitbox.
    s.floorRayX    = pos.x + localRayX;
    s.floorCurrentY = pos.y - h.regionW / 2.0f + 10 + h.height;
    s.floorFutureY  = (pos.y + grav) - h.regionW / 2.0f + 10 + h.height;

    return s;
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// [NOTA PARA EL FUTURO]: Para suelos de altura limitada o plataformas elevadas,
// ya filtramos por X (groundMinX/MaxX). Falta decidir si queremos "one-way platforms"
// (atravesables desde abajo).

//Suelos: Como ya tienes el minX y maxX, ya puedes crear un nivel con "flotantes" y el personaje se caerá por los huecos.

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// CHECK SURFACE COLLISION
// Verifica si el rayo vertical del personaje toca una superficie.
bool CollisionManager::checkFloorCollision(Interactor& inter, const SensorState& s) {
    
    // Determinamos los límites horizontales del suelo, líneas dibujadas de izq a der o viceversa
    float groundMinX = min(inter.p1.x, inter.p2.x);
    float groundMaxX = max(inter.p1.x, inter.p2.x);

    // 1. COMPROBACIÓN HORIZONTAL: ¿Está el rayo de mis pies sobre la plataforma?
    if (s.floorRayX >= groundMinX && s.floorRayX <= groundMaxX) {
        
        // 2. COMPROBACIÓN VERTICAL (Detección de cruce):
        // ¿Estaba por encima del suelo (o tocándolo) y en el siguiente frame estaría por debajo?
        if (s.floorCurrentY <= inter.p1.y && s.floorFutureY >= inter.p1.y) {
            return true;
        }
    }
    return false;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// [NOTA PARA EL FUTURO]: Actualmente la pared es un "láser infinito" en Y.
// Para muros finitos, habrá que comprobar si el rango vertical del personaje
// (usando su altura de Hitbox) se solapa con el segmento Y de la pared (p1.y a p2.y).

//Paredes: Tu idea de usar el "alto del HitWall" es la clave.
//Básicamente, será comparar si los pies del personaje están por debajo del "techo" del muro y su cabeza por encima de la "base" del muro.

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// CHECK WALL COLLISION
// Verifica si el sensor lateral ha invadido el espacio de una pared.
bool CollisionManager::checkWallCollision(Interactor& inter, const SensorState& s) {
    
    float wallX = inter.p1.x;
    if (inter.name == "actLimitR") {
        // ¿Mi posición futura está en la pared o más allá a la derecha?
        return (s.wallFuture >= wallX);
    }
    if (inter.name == "actLimitL") {
        // ¿Mi posición futura está en la pared o más allá a la izquierda?
        return (s.wallFuture <= wallX);
    }
    return false;
}


// ------------------- GETTERS - SETTERS -------------------
HitboxData CollisionManager::getHitbox() const {
    return currentHitbox;
}

void CollisionManager::setHitboxWidth(float w) {
    currentHitbox.width = w;
}

void CollisionManager::setHitRayXFloor(float x) {
    currentHitbox.floorRayX = x;
}
