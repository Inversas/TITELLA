#pragma once

#include "ofMain.h"
//
// Clase que gestiona una hoja de sprites
class SpriteSheetManager {
    
public:
    // Carga la hoja de sprites desde un archivo
    bool loadSpriteSheet(const std::string& filename);
    
    // Dibuja una región específica de la hoja de sprites
    void draw(int row, int region, bool isFacingRight);
    
    // Establece el factor de escala para la hoja de sprites
    void setScaleFactor(float scaleFactor);
    
    
private:
    ofImage spriteSheet; // Objeto que contiene la imagen de la hoja de sprites
    static const int REGION_WIDTH = 300; // Ancho de cada región en la hoja de sprites
    static const int REGION_HEIGHT = 300; // Alto de cada región en la hoja de sprites
    static const int SPRITE_OFFSET_X = 100; // Desplazamiento del sprite
    float scaleFactor = 1.0f; // Factor de escala para dibujar la hoja de sprites
    
};
