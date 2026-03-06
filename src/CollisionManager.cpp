#include "CollisionManager.h"

// ??????
// Aquí sí incluimos el manager del sprite para poder usar sus funciones
#include "SpriteSheetManager.h"

void CollisionManager::setup(float regionW, float regionH) {
    
    // *** HITBOX ***
    currentHitbox.width = regionW;     
    currentHitbox.height = regionH - 78;     // 222px
    currentHitbox.offsetY = 10.0f;           // El ajuste vertical
    currentHitbox.floorRayX = 97.0f;         // Posición horizontal del rayo de detección del suelo (ajustable por GUI)
    currentHitbox.regionW = regionW;         // Guardamos el ancho de la región para usarlo en los cálculos de colisión
    currentHitbox.regionH = regionH;         // Guardamos la altura de la región para usarlo en los cálculos de colisión
    
    
    
    // *** INTERACTORS ***
    // 1. Definimos el suelo (desde la esquina inferior izquierda a la derecha)
    interactors.push_back(Interactor(
        ofVec2f(0, 800),            // Punto inicial (0, alto)
        ofVec2f(ofGetWidth(), 800), // Punto final (ancho, alto)
        InteractorType::SURFACE,              // Es un SUELO
        "actGround"
    ));

    // 2. Definimos el límite izquierdo (pared vertical en x = 0)
    interactors.push_back(Interactor(
        ofVec2f(0, 0),             // Punto inicial (arriba)
        ofVec2f(0, ofGetHeight()), // Punto final (abajo)
        InteractorType::WALL,      // Es una PARED
        "actLimitL",
        regionW

    ));

    // 3. Definimos el límite derecho (pared vertical en x = ancho)
    interactors.push_back(Interactor(
        ofVec2f(1200, 0),             // Punto inicial (arriba)
        ofVec2f(1200, ofGetHeight()), // Punto final (abajo)
        InteractorType::WALL,                 // Es una PARED
        "actLimitR",
        regionW
    ));
    
    // PRUEBA PARED INTERMEDIA
    interactors.push_back(Interactor(
    ofVec2f(700, 500),             // Punto inicial (arriba)
    ofVec2f(700, 800), // Punto final (abajo)
    InteractorType::WALL,                 // Es una PARED
    "prova_wall",
    regionW
     ));

    // PRUEBA SUELO INTERMEDIO
    interactors.push_back(Interactor(
        ofVec2f(300, 500),            // Punto inicial (0, alto)
        ofVec2f(700, 500), // Punto final (ancho, alto)
        InteractorType::SURFACE,              // Es un SUELO
        "prova_surf"
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
        
        if (inter.hit) {
            if (inter.type == InteractorType::WALL) {
                drawWallHighlight(inter);
            }
            else if (inter.type == InteractorType::SURFACE) {
                drawFloorHighlight(inter);
            }
        }
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
    // FASE 0: RESETEO DE TODOS LOS INTERACTORS
    // ==========================================
    for (auto &inter : interactors) {
        inter.hit = false;
    }
    
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
            
            // Si estoy mirando adelante y la pared está más que mi posicion central
            // Porque el sistema no puede dejar pasar mi posición si hay una pared, y si la paso por lo que sea, ya será detras de mi
            
            // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
            // Si un día queremos detectar paredes delante y detrás a la vez?
            // por la espalda, sería con un hitbox de espalda, no?
            // Se pueden detectar dos interactors de un mismo tipo a la vez?
            // Puede con un vector o array de interactos tipo wall?
            // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            // Calculamos la distancia absoluta desde el centro (floorRayX) hasta la pared
            float distAlMuro = abs(inter.p1.x - sensors.floorRayX);
            
            float radioInfluencia = inter.influenceRadius;
            
            // DETERMINAR LÍMITES VERTICALES DE LA PARED (p1 y p2 pueden estar invertidos)
            float wallTop = min(inter.p1.y, inter.p2.y);
            float wallBottom = max(inter.p1.y, inter.p2.y);
            
            // FILTRO VERTICAL: ¿Está el personaje en el rango de la pared?
            // Si la Cabeza está más arriba que el final de la pared
            // Y los Pies están más abajo que la base de la pared
            if (sensors.headY < wallBottom && sensors.feetY > wallTop) {
                
                // FILTRO DE INFUENCIA: ¿Está la pared dentro de mi radio de acción?
                if (distAlMuro <= radioInfluencia) {
                    
                    if(isFacingRight && inter.p1.x > sensors.floorRayX ){
                        // Comprobamos bloqueo derecho
                        if (checkWallRightCollision(inter, sensors)) {
                            inter.hit = true;     // Interactor Hitted
                            result.isWalledRight = true;
                            result.wall = &inter; // Guardamos la referencia por si el Juez la necesita
                        }
                    }
                    else if(!isFacingRight && inter.p1.x < sensors.floorRayX ){
                        // Comprobamos bloqueo izquierdo
                        if (checkWallLeftCollision(inter, sensors)) {
                            inter.hit = true; // Interactor Hitted
                            result.isWalledLeft = true;
                            result.wall = &inter;
                        }
                    }
                }
            }
        }

        // SURFACE
        if (inter.type == InteractorType::SURFACE) {
            // Si hay colisión
            if (checkFloorCollision(inter, sensors)) {
                inter.hit = true; // Interactor Hitted
                result.isGrounded = true;
                result.floor = &inter;
            }
        }
    }
    
    // Guardamos una copia
    lastResult = result;
    
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
    
    // Calculamos la parte superior de la hitbox (posicion base menos offset de region + margen)
    s.headY = pos.y - h.regionW / 2.0f + 10;

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
    s.floorFutureY  = (pos.y + vel.y + grav) - h.regionW / 2.0f + 10 + h.height;
    
    // Guardamos la posición de los pies para usarla en la lógica de colisión de paredes
    s.feetY = s.floorCurrentY;

    return s;
}


// |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
// Para suelos de altura limitada o plataformas elevadas,
// ya filtramos por X (groundMinX/MaxX). Falta decidir si queremos "one-way platforms"
// (atravesables desde abajo).

//Suelos: Como ya tienes el minX y maxX, ya puedes crear un nivel con "flotantes" y el personaje se caerá por los huecos.
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


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

// |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
// Actualmente la pared es un "láser infinito" en Y.
// Para muros finitos, habrá que comprobar si el rango vertical del personaje
// (usando su altura de Hitbox) se solapa con el segmento Y de la pared (p1.y a p2.y).

//Paredes: Tu idea de usar el "alto del HitWall" es la clave.
//Básicamente, será comparar si los pies del personaje están por debajo del "techo" del muro y su cabeza por encima de la "base" del muro.

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


// CHECK WALL COLLISION
// Verifica si el sensor lateral ha invadido el espacio de una pared.
// Detecta si una pared bloquea el avance hacia la DERECHA
bool CollisionManager::checkWallRightCollision(Interactor& inter, const SensorState& s) {
    
    float wallX = inter.p1.x;

    // SOLO una cosa: ¿He pasado la pared?
    // Si mi X actual YA está al otro lado (más allá) = colisión
    return s.wallCurrent >= wallX;
}

// Detecta si una pared bloquea el avance hacia la IZQUIERDA
bool CollisionManager::checkWallLeftCollision(Interactor& inter, const SensorState& s) {
    
    float wallX = inter.p1.x;
    
    // SOLO una cosa: ¿He pasado la pared?
    // Si mi X actual YA está al otro lado (antes) = colisión
    return s.wallCurrent <= wallX;
}


// CollisionManager.cpp

void CollisionManager::drawWallHighlight(const Interactor& inter) {
    // 1. Estilo: Azul transparente
    ofFill();
    ofSetColor(0, 150, 255, 50); // Azul con poca opacidad
    
    // 2. Calculamos el rectángulo del radio de influencia
    // La pared es vertical, así que usamos p1.x y la altura entre p1.y y p2.y
    float x = inter.p1.x - inter.influenceRadius;
    float y = inter.p1.y;
    float w = inter.influenceRadius * 2;
    float h = abs(inter.p2.y - inter.p1.y);
    
    ofDrawRectangle(x, y, w, h);
}

void CollisionManager::drawFloorHighlight(const Interactor& inter) {
    // 1. Estilo: Azul/Cian transparente
    ofFill();
    ofSetColor(0, 255, 255, 80);
    
    // 2. Dibujamos el rectángulo de 10px por debajo del suelo
    // El ancho es la diferencia entre p2 y p1
    float x = inter.p1.x;
    float y = inter.p1.y; // El suelo está en esta altura
    float w = abs(inter.p2.x - inter.p1.x);
    float h = 10.0f; // Los 10 píxeles de grosor que pediste
    
    ofDrawRectangle(x, y, w, h);
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

const vector<Interactor>& CollisionManager::getInteractors() const {
    return interactors;
}

CollisionResult CollisionManager::getLastResult() const {
    return lastResult;
}
