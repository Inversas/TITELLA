#pragma once

#include "ofMain.h"
//
// Clase que gestiona una hoja de sprites
class SpriteSheetManager {
    
public:
    // Carga la hoja de sprites desde un archivo
    bool loadSpriteSheet(const std::string& filename);
    
    // Dibuja una región específica de la hoja de sprites
    void draw(float x, float y, int row, int region, bool isFacingRight);
    
    // Establece el factor de escala para la hoja de sprites
    void setScaleFactor(float scaleFactor);
    
    //PROVISIONAL
    void drawRegion(float x, float y);
    
    //HITBOX
    void drawHitBox(float x, float y);
    void drawHitWall(float x, float y);
    void drawHitFloor(float x, float y);
    void drawGuides(float x, float y);

    //PROVISIONAL
    void drawCircle(float x, float y, float radius);
    
    
    

    // ------------------- GETTERS - SETTERS -------------------
    
    //HITBOX_W
    float getHitboxW() const { return HITBOX_W; }
    float getHitboxH() const { return HITBOX_H; }
    void setHitboxW(float hitboxW) { HITBOX_W = hitboxW; }
    
    
    //HITRAY_FLOOR
    float getHitRayXFloor() const { return HITRAY_FLOOR_X; }
    void setHitRayXFloor(float hitRayFloor) { HITRAY_FLOOR_X = hitRayFloor; }
    
    //REGION_WIDTH y REGION_HEIGHT
    int getRegionWidth() const { return REGION_WIDTH; }
    int getRegionHeight() const { return REGION_HEIGHT; }
    
    
private:
    ofImage spriteSheet; // Objeto que contiene la imagen de la hoja de sprites
    static const int REGION_WIDTH = 300; // Ancho de cada región en la hoja de sprites
    static const int REGION_HEIGHT = 300; // Alto de cada región en la hoja de sprites
    static const int SPRITE_OFFSET_X = 50; // Desplazamiento del sprite
    float scaleFactor = 1.0f; // Factor de escala para dibujar la hoja de sprites
    
    
    // *** HITBOX ***
    float HITBOX_W = REGION_WIDTH - 100;   // 200px
    float HITBOX_H = REGION_HEIGHT - 78;  // 222px
    float HITBOX_OFFSET_Y = 10;           // El ajuste vertical
    
    
    float HITRAY_FLOOR_X = 97.0f; // Posición horizontal del rayo de detección del suelo (ajustable por GUI)
    
};
