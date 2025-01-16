#include "SpriteSheetManager.h"

// Carga la hoja de sprites desde un archivo
bool SpriteSheetManager::loadSpriteSheet(const std::string& filename) {
    // Carga la imagen de la hoja de sprites desde el archivo especificado
    return spriteSheet.load(filename);
}

// Dibuja una región específica de la hoja de sprites
void SpriteSheetManager::draw(int row, int region) {
    
    // Guarda la matriz de transformación actual
    ofPushMatrix();
    
    // Escala la imagen según el factor de escala
    ofScale(scaleFactor, scaleFactor);
    
    spriteSheet.drawSubsection(
                               0, 0, // Dibuja en el origen (0, 0)
                               REGION_WIDTH, REGION_HEIGHT, // Dimensiones de la región a dibujar
                               region * REGION_WIDTH, // Coordenada x de la región en la hoja de sprites
                               row * REGION_HEIGHT // Coordenada y de la región en la hoja de sprites
                               );
    
    // Supongamos que draw() dibuja el sprite en el origen (0, 0)
    // sprite.draw(-sprite.getWidth() / 2, -sprite.getHeight() / 2); // Comentado por ahora
    
    // Restaura la matriz de transformación guardada
    ofPopMatrix();
}

// Establece el factor de escala para la hoja de sprites
void SpriteSheetManager::setScaleFactor(float scaleFactor) {
    // Asigna el nuevo factor de escala
    this->scaleFactor = scaleFactor;
}