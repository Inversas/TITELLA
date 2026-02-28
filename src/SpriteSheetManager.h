#pragma once

#include "ofMain.h"

class CollisionManager;
struct HitboxData;

// Clase que gestiona una hoja de sprites
class SpriteSheetManager {
    
public:
    //Usamos * (Puntero) "puede o no" tener un Manager de colisiones, queremos poder asignarlo después del setup. (Tipo puntero)
    //Usarías & (Referencia) si fuera absolutamente imposible que el Sprite existiera sin el CollisionManager. (Dirección de)
    void setCollisionManager(CollisionManager* collision);
    
    // Carga la hoja de sprites desde un archivo
    bool loadSpriteSheet(const std::string& filename);
    
    
    // *** DRAWS ***
    // Dibuja una región específica de la hoja de sprites
    void draw(float x, float y, int row, int region, bool isFacingRight);
    
    // Dibuja los límites de la región
    void drawRegion(float x, float y);
    
    //Dibuja un Criculo
    void drawCircle(float x, float y, float radius);
    
    //HITBOX
    void drawGuides(float x, float y);
    void drawHitBox(float x, float y, const HitboxData& hitbox);
    void drawHitWall(float x, float y, const HitboxData& hitbox);
    void drawHitFloor(float x, float y, const HitboxData& hitbox);




    // ------------------- GETTERS - SETTERS -------------------
    
    // Establece el factor de escala para la hoja de sprites
    void setScaleFactor(float scaleFactor);
    
    // *** REGION DIMENSIONS ***
    int getRegionWidth() const;
    int getRegionHeight() const;
    
    
private:
    ofImage spriteSheet; // Objeto que contiene la imagen de la hoja de sprites
    static const int REGION_WIDTH = 300; // Ancho de cada región en la hoja de sprites
    static const int REGION_HEIGHT = 300; // Alto de cada región en la hoja de sprites
    static const int SPRITE_OFFSET_X = 50; // Desplazamiento del sprite
    float scaleFactor = 1.0f; // Factor de escala para dibujar la hoja de sprites
    
    // Conexión al CollisionManager
    CollisionManager* collisionManager = nullptr;    
};
