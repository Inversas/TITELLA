#pragma once

#include "ofMain.h"

// Clase que gestiona una hoja de sprites
class SpriteSheetManager {
    
public:
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
    void drawHitBox(float x, float y);
    void drawHitWall(float x, float y);
    void drawHitFloor(float x, float y);




    // ------------------- GETTERS - SETTERS -------------------
    
    // Establece el factor de escala para la hoja de sprites
    void setScaleFactor(float scaleFactor);
    
    // *** HITBOX ***
    float getHitboxW() const;
    float getHitboxH() const;
    void setHitboxW(float hitboxW);

    // *** HITRAY_FLOOR ***
    float getHitRayXFloor() const;
    void setHitRayXFloor(float hitRayFloor);

    // *** REGION DIMENSIONS ***
    int getRegionWidth() const;
    int getRegionHeight() const;
    
    
private:
    ofImage spriteSheet; // Objeto que contiene la imagen de la hoja de sprites
    static const int REGION_WIDTH = 300; // Ancho de cada región en la hoja de sprites
    static const int REGION_HEIGHT = 300; // Alto de cada región en la hoja de sprites
    static const int SPRITE_OFFSET_X = 50; // Desplazamiento del sprite
    float scaleFactor = 1.0f; // Factor de escala para dibujar la hoja de sprites
    
    
    
    
    
    // !!! DEBERÍA IR A COLLISION MANAGER !!!
    // *** HITBOX ***
    float HITBOX_W = REGION_WIDTH - 100;   // 200px
    float HITBOX_H = REGION_HEIGHT - 78;  // 222px
    float HITBOX_OFFSET_Y = 10;           // El ajuste vertical
    
    
    float HITRAY_FLOOR_X = 97.0f; // Posición horizontal del rayo de detección del suelo (ajustable por GUI)
    
};
