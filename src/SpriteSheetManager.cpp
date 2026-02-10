#include "SpriteSheetManager.h"

// Carga la hoja de sprites desde un archivo
bool SpriteSheetManager::loadSpriteSheet(const std::string& filename) {
    // Carga la imagen de la hoja de sprites desde el archivo especificado
    return spriteSheet.load(filename);
}

// Dibuja una región específica de la hoja de sprites
void SpriteSheetManager::draw(float x, float y, int row, int region, bool isFacingRight) {
    
    // 1. Guardamos la matriz actual
    ofPushMatrix();
    
    // 2. Traslación base al centro de la ventana (ajustada por tu offset de 150)
    ofTranslate(ofGetWidth()/2 - 150, ofGetHeight()/2 - 150);
    
    // 3. Nos movemos a la posición (x, y) que dicta la física
    // Esto hace que el dibujo siga al "títere"
    ofTranslate(x, y);

    if (isFacingRight) {
        // --- DIBUJO DERECHA ---
        ofScale(scaleFactor, scaleFactor);
        
        // El offset se aplica normalmente
        float drawX = SPRITE_OFFSET_X;

        drawRectangle(drawX, 0);
        drawCircle(drawX, 0, 5);
        
        spriteSheet.drawSubsection(
            drawX, 0,
            REGION_WIDTH, REGION_HEIGHT,
            region * REGION_WIDTH,
            row * REGION_HEIGHT
        );
    }
    else {
        // --- DIBUJO IZQUIERDA (ESPEJO) ---
        ofScale(-scaleFactor, scaleFactor);
        
        // IMPORTANTE: Al estar en escala negativa, para que el cuerpo
        // quede en el mismo sitio, debemos desplazarlo hacia el "nuevo negativo"
        // que visualmente es la derecha.
        float drawX = -REGION_WIDTH + SPRITE_OFFSET_X;

        drawRectangle(drawX, 0);
        drawCircle(drawX, 0, 5);
        
        spriteSheet.drawSubsection(
            drawX, 0,
            REGION_WIDTH, REGION_HEIGHT,
            region * REGION_WIDTH,
            row * REGION_HEIGHT
        );
    }
    
    // 4. Restauramos la matriz
    ofPopMatrix();
}

// Establece el factor de escala para la hoja de sprites
void SpriteSheetManager::setScaleFactor(float scaleFactor) {
    // Asigna el nuevo factor de escala
    this->scaleFactor = scaleFactor;
}

void SpriteSheetManager::drawRectangle(float x, float y){
    // --- NUEVO: Dibujo del cuadrado guía ---
    ofNoFill();            // Solo el contorno
    ofSetColor(255);       // Color blanco
    ofDrawRectangle(x, y, REGION_WIDTH, REGION_HEIGHT-65);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
    // ---------------------------------------
}

void SpriteSheetManager::drawCircle(float x, float y, float radius){
    // --- NUEVO: Dibujo del punto guía ---
    ofNoFill();            // Solo el contorno
    ofSetColor(255);       // Color blanco
    ofDrawCircle(x, y, radius);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
    // ---------------------------------------
}

