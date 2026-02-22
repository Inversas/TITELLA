#include "SpriteSheetManager.h"

// Carga la hoja de sprites desde un archivo
bool SpriteSheetManager::loadSpriteSheet(const std::string& filename) {
    // Carga la imagen de la hoja de sprites desde el archivo especificado
    return spriteSheet.load(filename);
}

// Dibuja una región específica de la hoja de sprites
void SpriteSheetManager::draw(float x, float y, int row, int region, bool isFacingRight) {
    
    
    // ·······························································
    //             PUNTO ROJO --> CENTRO ORIGINAL DE LA PANNTALLA
    // ·······························································
    ofSetColor(255,0,0);
    drawCircle(0,0,5);
    
    
    // ·······························································
    //             PUNTO AMARILLO --> CENTRO DE LA VENTANA
    // ·······························································
    ofSetColor(255,255,0);
    drawCircle(ofGetWidth()/2,ofGetHeight()/2,5);
    
    
    
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    // Guardamos la matriz actual
    ofPushMatrix();

    
    
    // El origen (0,0) ahora coincide con la posición del títere (x, y)
    // Trasladamos el sistema al punto (x, y) que viene del PhysicsManager.
    // IMPORTANTE: A partir de aquí, las colisiones deben entender que este es el centro del objeto.
    ofTranslate(x, y);
    
    // ·······························································
    //             PUNTO VERDE --> ORIGEN REAL del personaje, Sin ajuste de REGION. Marca el centro de la region sin Giro ni escala.
    // ·······························································
    ofSetColor(0,255,0);
    drawCircle(0,0,5);
    

    // DIBUJO FACING RIGHT
    if (isFacingRight) {
        ofScale(scaleFactor, scaleFactor);
        
        // Posicion a dibujar, centrando region
        float drawX = -REGION_WIDTH / 2;
        float drawY = -REGION_HEIGHT / 2;
        
        // Draw Guides
        drawGuides(drawX, drawY);
        
        // ·······························································
        //             PUNTO BLANCO --> Es el (0,0) relativo al sprite (esquina superior izq de la imagen)
        // ·······························································
        ofSetColor(255);
        drawCircle(drawX, drawY, 5);
        

        // Personaje
        spriteSheet.drawSubsection(
            drawX, drawY,
            REGION_WIDTH, REGION_HEIGHT,
            region * REGION_WIDTH,
            row * REGION_HEIGHT
        );
    }
    // DIBUJO FACING LEFT (ESPEJO)
    else {
        ofScale(-scaleFactor, scaleFactor);
        
        // Posicion a dibujar (ajustada para el giro horizontal)
        // El 100 es temporal hasta redibujar bien centrado el sprite
        float drawX = -REGION_WIDTH / 2 + 100;
        float drawY = -REGION_HEIGHT / 2;
        
        // Draw Guides
        drawGuides(drawX, drawY);
        
        // ·······························································
        //             PUNTO BLANCO --> ANCLAJE PERSONAJE (POSICION)
        // ·······························································
        drawCircle(drawX, drawY, 5);
        
        // Personaje
        spriteSheet.drawSubsection(
            drawX, drawY,
            REGION_WIDTH, REGION_HEIGHT,
            region * REGION_WIDTH,
            row * REGION_HEIGHT
        );
    }
    
    // Restauramos la matriz
    ofPopMatrix();
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

}



// *** DRAWS ***
void SpriteSheetManager::drawRegion(float x, float y){
    ofNoFill();            // Solo el contorno
    ofSetColor(255,255,255);   // Color Blanco
    ofDrawRectangle(x, y, REGION_WIDTH, REGION_HEIGHT);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
}

void SpriteSheetManager::drawCircle(float x, float y, float radius){
    //ofNoFill();            // Solo el contorno
    //ofSetColor(255);       // Color blanco
    ofDrawCircle(x, y, radius);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
}


void SpriteSheetManager::drawGuides(float x, float y) {
    // Region Base
    drawRegion(x, y);
    // Hitbox GUIA
    drawHitBox(x, y);
    // HitWall
    drawHitWall(x, y);
    // HitFloor
    drawHitFloor(x, y);
}

void SpriteSheetManager::drawHitBox(float x, float y){
    ofNoFill();            // Solo el contorno
    ofSetColor(255);       // Color Blanco
    ofDrawRectangle(x, y+HITBOX_OFFSET_Y, HITBOX_W, HITBOX_H);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
}

void SpriteSheetManager::drawHitWall(float x, float y) {
    ofSetColor(255, 0, 0);
    ofDrawLine(x + HITBOX_W, y + HITBOX_OFFSET_Y,
               x + HITBOX_W, y + HITBOX_OFFSET_Y + HITBOX_H);
}

void SpriteSheetManager::drawHitFloor(float x, float y) {
    
    float floorY = y + HITBOX_OFFSET_Y + HITBOX_H;
    
    ofSetColor(0, 0, 255);
    ofDrawLine(x, floorY,
               x + HITBOX_W, y + HITBOX_OFFSET_Y + HITBOX_H);
    
    //HITFLOOR RAY
    ofSetColor(64, 224, 208);
    
    // Su posición X es la base (x) + el desplazamiento del slider (hitFloorRayX)
    float rayX = x + HITRAY_FLOOR_X;
    ofDrawLine(rayX, floorY, rayX, floorY - 15);
    ofDrawCircle(rayX, floorY, 3); // El punto de contacto exacto
}








// ------------------- GETTERS - SETTERS -------------------

// Establece el factor de escala para la hoja de sprites
void SpriteSheetManager::setScaleFactor(float scaleFactor) {
    // Asigna el nuevo factor de escala
    this->scaleFactor = scaleFactor;
}


int SpriteSheetManager::getRegionWidth() const {
    return REGION_WIDTH;
}

int SpriteSheetManager::getRegionHeight() const {
    return REGION_HEIGHT;
}

float SpriteSheetManager::getHitboxW() const {
    return HITBOX_W;
}

float SpriteSheetManager::getHitboxH() const {
    return HITBOX_H;
}

float SpriteSheetManager::getHitRayXFloor() const {
    return HITRAY_FLOOR_X;
}

void SpriteSheetManager::setHitboxW(float hitboxW) {
    HITBOX_W = hitboxW;
}

void SpriteSheetManager::setHitRayXFloor(float hitRayFloor) {
    HITRAY_FLOOR_X = hitRayFloor;
}


