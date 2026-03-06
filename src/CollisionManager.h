#pragma once
#include <vector>

#include "ofMain.h"

#include "Interactor.h"

using namespace std;

class SpriteSheetManager;


// HITBOX DATA
struct HitboxData {
    float width;
    float height;
    float offsetY;
    float floorRayX;
    float feetOffset; // El cálculo de los +82px
    float regionW;
    float regionH;
};

// SENSORS DATA, para no pasar tantos parametros
struct SensorState {
    float wallCurrent, wallFuture;
    float floorRayX;
    float floorCurrentY, floorFutureY;
    float headY;
    float feetY;   // Será el floorCurrentY
};

// PAQUETE DE COLISIONES, para evitar exclusiones (si detectaba suelo, ya no detectaba pared)
struct CollisionResult {
    Interactor* wall = nullptr;
    Interactor* floor = nullptr;
    
    bool isWalledLeft = false;
    bool isWalledRight = false;
    bool isGrounded = false;
    
    bool hasAny() const { return wall != nullptr || floor != nullptr; }
    
    bool hasAnyWall() const { return wall != nullptr; }
};

// Clase que gestiona las colisiones en el juego
class CollisionManager {

public:
    // Inicializa los límites, el suelo y el HitboxData
    void setup(float regionW, float regionH);
    
    void update();

    
    void draw();
    
    // Dibujará las líneas de colisión para debug
    void drawInteractors();
    
    // Retorna el paquete de colisiones detectadas
    CollisionResult checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, bool isFacingRight);
    
    // ------------------- GETTERS - SETTERS -------------------
    HitboxData getHitbox() const;
    void setHitboxWidth(float w);
    void setHitRayXFloor(float x);
    
    const vector<Interactor>& getInteractors() const;
    CollisionResult getLastResult() const;
    
    
private:
    // Lista donde guardamos todos los suelos, paredes y botones
    vector<Interactor> interactors;
    
    // La instancia real de los datos
    HitboxData currentHitbox;
    
    // Guardamos el último resultado
    CollisionResult lastResult;
    
    // CACULA SENSORES FUTUROS
    SensorState calculateSensors(ofVec2f pos, ofVec2f vel, float grav, bool isFacingRight);
    
    // CHECK SURFACE COLLISION
    bool checkFloorCollision(Interactor& inter, const SensorState& s);
    // CHECK WALL COLLISION
    bool checkWallRightCollision(Interactor& inter, const SensorState& s);
    bool checkWallLeftCollision(Interactor& inter, const SensorState& s);
    
    // Recibe la referencia al interactor para saber su posición y radio
    void drawWallHighlight(const Interactor& inter);
    // Recibe la referencia al interactor para saber su ancho y posición
    void drawFloorHighlight(const Interactor& inter);
};
