#include "SpriteSheetManager.h"

// Carga la hoja de sprites desde un archivo
bool SpriteSheetManager::loadSpriteSheet(const std::string& filename) {
    // Carga la imagen de la hoja de sprites desde el archivo especificado
    return spriteSheet.load(filename);
}

// Dibuja una región específica de la hoja de sprites
void SpriteSheetManager::draw(int row, int region, bool isFacingRight) {
    
    // Guarda la matriz de transformación actual
    ofPushMatrix();
    
    
    // Traslada la posición al centro de la ventana
    ofTranslate(ofGetWidth()/2-150, ofGetHeight()/2-150);
    
    //Si la dirección del persona es izquierda
    if (isFacingRight) {
        // Escala la imagen según el factor de escala
        ofScale(scaleFactor, scaleFactor);
        
        spriteSheet.drawSubsection(
                                0, 0, // Dibuja en el origen (0, 0)
                                REGION_WIDTH, REGION_HEIGHT, // Dimensiones de la región a dibujar
                                region * REGION_WIDTH, // Coordenada x de la región en la hoja de sprites
                                row * REGION_HEIGHT // Coordenada y de la región en la hoja de sprites
                                );
    }
    else {
        ofScale(-scaleFactor, scaleFactor); // Escala invertida horizontalmente
        
        // Ajusta la posición de dibujo para la escala invertida
        spriteSheet.drawSubsection(
                                -REGION_WIDTH + SPRITE_OFFSET_X, 0, // Ajusta la posición x para la escala invertida con el offset
                                REGION_WIDTH, REGION_HEIGHT, // Dimensiones de la región a dibujar
                                region * REGION_WIDTH, // Coordenada x de la región en la hoja de sprites
                                row * REGION_HEIGHT // Coordenada y de la región en la hoja de sprites
                                );
    }
    
    
    // Restaura la matriz de transformación guardada
    ofPopMatrix();
}

// Establece el factor de escala para la hoja de sprites
void SpriteSheetManager::setScaleFactor(float scaleFactor) {
    // Asigna el nuevo factor de escala
    this->scaleFactor = scaleFactor;
}
