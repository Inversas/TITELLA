#pragma once
#include <vector>

#include "ofMain.h"

#include "Interactor.h"

using namespace std;

// Lo comento porque podríamos eliminarlo
// class SpriteSheetManager;

// +++++++++++++ STRUCT +++++++++++++
// HITBOX DATA
struct HitboxData {
    float width;
    float height;
    float floorRayX;
    float regionW;
    float regionH;
};

// +++++++++++++ STRUCT +++++++++++++
// SENSORS DATA, para no pasar tantos parametros
struct SensorState {
    float wallCurrent, wallFuture;
    float floorRayX;
    float floorCurrentY, floorFutureY;
    float headY;
    float feetY;   // Será el floorCurrentY
};

// +++++++++++++ STRUCT +++++++++++++
// PAQUETE DE COLISIONES, para evitar exclusiones (evita que si detecto suelo, ya no detecte pared)
struct CollisionResult {
    Interactor* wall = nullptr;
    Interactor* floor = nullptr;
    
    bool isWalledLeft = false;
    bool isWalledRight = false;
    bool isGrounded = false;
    
    // Hay alguna colison
    bool hasAny() const { return wall != nullptr || floor != nullptr; }
    
    // Hay colision con pared
    bool hasAnyWall() const { return wall != nullptr; }
};

// ****************************************** CLASE COLLISION MANAGER ****************************************** //
// Clase que gestiona las colisiones
class CollisionManager {

public:
    
    // *** SETUP ***
    // Inicializa los límites, el suelo y el HitboxData
    void setup(float regionW, float regionH);
    
    void update();

    // *** DRAW ***
    void draw();
    
    // **************** DEBUG (en realidad tiene su propio booleano) ******************
    // Dibujará las líneas
    void drawInteractors();
    
    
    // *** DETECCIÓN DE COLISIONES A FUTURO ***
    CollisionResult checkCollisions(ofVec2f currentPos, ofVec2f velocity, float gravity, bool isFacingRight);
    
    // ------------------- GETTERS - SETTERS -------------------
    const vector<Interactor>& getInteractors() const;
    CollisionResult getLastResult() const;
    
    HitboxData getHitbox() const;
    HitboxData getBaseHitbox() const;
    void setHitboxWidth(float w);
    void setHitRayXFloor(float x);
    
    // Actualiza el parametro "gui" de Interactor, segun su nombre.
    void setInteractorGUI(string name, bool state);
    
    // Modifica el valor de escala
    void setCurrentScale(float scale);
    
private:
    
    // Lista donde guardamos todos los suelos, paredes y botones
    vector<Interactor> interactors;
    
    // *** VALORES BASE ***
    HitboxData baseHitbox;
    // *** VALORES DE TRABAJO ***
    HitboxData currentHitbox;

    // *** GUARDA EL ULTIMO RESULTADO DE COLISIONES***
    CollisionResult lastResult;
    
    // Escala actual del personaje (1.0 = 100%)
    float currentScale = 1.0f;
    
    // *** ACTUALIZA VALORES DE TRABAJO SEGUN ESCALA ***
    void updateScaledHitbox();
    
    // *** CACULA SENSORES FUTUROS ***
    SensorState calculateSensors(ofVec2f pos, ofVec2f vel, float grav, bool isFacingRight);
    
    // *** CHECK SURFACE COLLISION ***
    bool checkFloorCollision(Interactor& inter, const SensorState& s);
    // *** CHECK WALL COLLISION ***
    bool checkWallRightCollision(Interactor& inter, const SensorState& s);
    bool checkWallLeftCollision(Interactor& inter, const SensorState& s);

    // *** DRAWS DEBUG ***
    // Recibe la referencia al interactor para saber su posición y radio
    void drawWallHighlight(const Interactor& inter);
    // Recibe la referencia al interactor para saber su ancho y posición
    void drawFloorHighlight(const Interactor& inter);
};
