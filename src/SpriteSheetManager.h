#pragma once

#include "ofMain.h"

// !!! MOTIVO FORWARD DECLARATIONS !!!
// Un puntero siempre mide lo mismo (4 u 8 bytes),
// sin importar lo que haya dentro de la clase.
// Por tanto, al compilador le basta con saber que esa clase
// Además Evitamos el Bucle:
// Si CollisionManager incluye a SpriteSheetManager y viceversa, el programa no compilaría.
//La Forward Declaration rompe ese círculo.
class CollisionManager;
struct HitboxData;

// ****************************************** CLASE SPRITE SHEET MANAGER ****************************************** //
// Clase que gestiona una hoja de sprites
class SpriteSheetManager {
    
public:
    // !!! MOTIVO CONSTR / DESTR !!!
    // Sobretodo por el uso de Punteros Simples (referencias).
    // También por std::map libera la memoria de la RAM automáticamente al cerrarse el programa,
    // pero las imágenes en juegos a veces dejan "residuos" en la VRAM (memoria de video).
    // Al forzar el clear() en el destructor, te aseguras.
    // *** CONSTRUCTOR ***
    SpriteSheetManager();
    // *** DESTRUCTOR ***
    ~SpriteSheetManager();
    
    
    // Usamos * (Puntero) "puede o no" tener un Manager de colisiones, queremos poder asignarlo después del setup. (Tipo puntero)
    // Usarías & (Referencia) si fuera absolutamente imposible que el Sprite existiera sin el CollisionManager. (Dirección de)
    void setCollisionManager(CollisionManager* collision);
    
    // Carga la hoja de sprites desde un archivo, esta función se llama en MovementManager al cargar el JSON de movimientos
    bool loadSpriteSheet(const std::string& filename);
    
    
    // *** DRAWS ***
    // Dibuja una región específica de la hoja de sprites, debe saber que hoja de sprites
    void draw(float x, float y, int row, int region, bool isFacingRight, const string& sheetName, bool debug);
    // Dibuja un circulo
    void drawCircle(float x, float y, float radius);
    
    
    // *** DRAWS DEBUG ***
    // Dibuja los límites de la región
    void drawRegion(float x, float y);

    // Dibuja los limites de la HITBOX
    void drawGuides(float x, float y);
    void drawHitBox(float x, float y, const HitboxData& hitbox, bool isHitWall);
    void drawHitWall(float x, float y, const HitboxData& hitbox);
    void drawHitFloor(float x, float y, const HitboxData& hitbox);


    

    // ------------------- GETTERS - SETTERS -------------------
    // Recupera el factor de escala actual para la hoja de sprites
    float getCurrentScale() const;
    // Establece el factor de escala para la hoja de sprites
    void setCurrentScale(float scaleFactor);
    
    // *** REGION DIMENSIONS ***
    int getRegionWidth() const;
    int getRegionHeight() const;
    
    
private:
    // *** VARIABLES DE SPRITE ***
    std::map<std::string, ofImage> spriteSheets; // Mapa que contiene las imagenes de las hojas de sprites
    static const int REGION_WIDTH = 300;         // Ancho de cada región en la hoja de sprites
    static const int REGION_HEIGHT = 300;        // Alto de cada región en la hoja de sprites
    static const int SPRITE_OFFSET_X = 0;        // Desplazamiento del sprite
    float currentScale;                          // Factor de escala para dibujar la hoja de sprites
    
    // Conexión al CollisionManager
    CollisionManager* collisionManager = nullptr;    
};
