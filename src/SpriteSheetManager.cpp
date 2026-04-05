#include "SpriteSheetManager.h"
#include "CollisionManager.h"

// *** CONSTRUCTOR ***
SpriteSheetManager::SpriteSheetManager()
    : collisionManager(nullptr),
      currentScale(1.0f)
{
    // Limpiamos el mapa de texturas por seguridad al arrancar
    spriteSheets.clear();
    
    ofLogNotice("SpriteSheetManager") << "Constructor: Gestor de sprites listo.";
}

// *** DESTRUCTOR ***
SpriteSheetManager::~SpriteSheetManager() {
    // 1. Limpiamos el mapa de imágenes.
    // Al llamar a clear(), el mapa llama al destructor de cada 'ofImage',
    // lo que libera la memoria de la textura en la tarjeta gráfica (GPU).
    spriteSheets.clear();
    
    // 2. Limpiamos el puntero al CollisionManager.
    // Como es una referencia "prestada" (no la creamos con new),
    // NO hacemos delete, solo lo ponemos a nullptr.
    collisionManager = nullptr;

    ofLogNotice("SpriteSheetManager") << "Destructor: Memoria de texturas (GPU) liberada.";
}


// *** SERÍA COMO EL SETUP ***
void SpriteSheetManager::setCollisionManager(CollisionManager* collision) {
    this->collisionManager = collision;
}

// *** CARGA DE IMAGENES DE SPRITE ***
// Carga la hoja de sprites dado su nombre, usado al cargar el JSON en MovementManager
bool SpriteSheetManager::loadSpriteSheet(const std::string& filename) {
    // GENERAR NOMBRE STIRNG VERSION DEBUG
    string filename_debug = "";
    if (filename.length() > 4) {
        // Tomamos todo menos los últimos 4 caracteres (".png")
        filename_debug = filename.substr(0, filename.length() - 4);
        // Añadimos debug y la extension
        filename_debug = filename_debug + "_debug.png";
    }
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    // Añadir un control para poder cargar solo la version debug?
    // Para implementarlo ver que pasa si eliminamos la version normal
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    
    // Comprobamos si ya están cargadas ( DEBUG Y NORMAL )
    if (spriteSheets.find(filename) != spriteSheets.end() && spriteSheets.find(filename_debug) != spriteSheets.end()) {
        return true; // Ya existen, no hace falta cargarlas de nuevo
    }
    
    // Objeto imagen donde cargar la hoja y para meterlo en el mapa de spriteSheets
    ofImage newSheet;
    // Intentamos cargar la imagen
    if (newSheet.load(filename)) {
        // La guardamos en el mapa se spriteSheets, usando el nombre como llave
        spriteSheets[filename] = newSheet;
        ofLogNotice("SpriteSheetManager") << "Cargada nueva hoja: " << filename;
        // Intentamos cargar la versión DEBUG
        if (newSheet.load(filename_debug)) {
            // La guardamos en el mapa usando el nombre como llave
            spriteSheets[filename_debug] = newSheet;
            ofLogNotice("SpriteSheetManager") << "Cargada nueva hoja: " << filename_debug;
        }
        else {
            ofLogError("SpriteSheetManager") << "Error al cargar el archivo: " << filename_debug ;
            return false;
        }
        return true;
    }
    ofLogError("SpriteSheetManager") << "Error al cargar el archivo: " << filename ;
    return false;
}

// *** DRAWS ***
// Dibuja una región específica de la hoja de sprites
void SpriteSheetManager::draw(float x, float y, int row, int region, bool isFacingRight, const string& sheetName, bool debug) {
    
    // ************************ DEBUG ************************
    // Cambiamos el sheetName segun su versión debug
    string spriteSheetName = sheetName;
    // Si - [ DEBUG MODE ]
    if(debug){
        // Si el nombre tiene almenos 4 caracteres (".png")
        if (sheetName.length() > 4) {
            // Tomamos todo menos los últimos 4 caracteres (".png")
            string base = sheetName.substr(0, sheetName.length() - 4);
            // Retornamos la base + el sufijo + la extensión
            spriteSheetName = base + "_debug.png";
        }
    }
    
    // ARCHIVO IMAGEN A USAR
    ofImage& spriteSheet = spriteSheets[spriteSheetName];

    // ************************ DEBUG ************************
    // Si - [ DEBUG MODE ]
    if (debug) {
        // ·······························································
        //             PUNTO ROJO --> CENTRO ORIGINAL DE LA PANNTALLA
        // ·······························································
        ofSetColor(255,0,0);
        ofNoFill();
        drawCircle(0,0,5);
        
        // ·······························································
        //             PUNTO AMARILLO --> CENTRO DE LA VENTANA
        // ·······························································
        ofSetColor(255,255,0);
        ofNoFill();
        drawCircle(ofGetWidth()/2,ofGetHeight()/2,5);
    }
    
    
// ##############################################################################################################################
//                                                  START OF MATRIX
// ##############################################################################################################################
    // Guardamos la matriz actual
    ofPushMatrix();
    
    // El origen (0,0) ahora coincide con la posición del títere (x, y)
    // Trasladamos el sistema al punto (x, y) que viene del PhysicsManager.
    // IMPORTANTE: A partir de aquí, las colisiones deben entender que este es el centro del objeto.
    ofTranslate(x, y);
    
    // ************************ DEBUG ************************
    // Si - [ DEBUG MODE ]
    if(debug){
        // ·······························································
        //             PUNTO VERDE --> ORIGEN REAL del personaje. Marca el centro de la region sin escala.
        // ·······························································
        // Por el translate, sería como hacer: drawCircle(x, y, 5);
        ofSetColor(0,255,0);
        drawCircle(0,0,5);
    }

    // DIBUJO FACING RIGHT
    if (isFacingRight) {
        // Aplicamos la Escala antes de dibujar
        ofScale(currentScale, currentScale);
        
        // Posicion a dibujar, centrando region
        float drawX = -REGION_WIDTH / 2;
        float drawY = -REGION_HEIGHT / 2;
        
        // ************************ DEBUG ************************
        // Si - [ DEBUG MODE ]
        if (debug){
            // Draw Guides
            drawGuides(drawX, drawY);
            
            // ·······························································
            //             PUNTO BLANCO --> Es el (0,0) relativo al sprite (esquina superior izq de la imagen)
            // ·······························································
            ofSetColor(255);
            drawCircle(drawX, drawY, 5);
        }

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
        
        // Aplicamos la Escala antes de dibujar
        ofScale(-currentScale, currentScale);
        
        // Posicion a dibujar (ajustada para el giro horizontal)
        float drawX = -REGION_WIDTH / 2;
        float drawY = -REGION_HEIGHT / 2;
        
        // ************************ DEBUG ************************
        // Si - [ DEBUG MODE ]
        if (debug){
            // Draw Guides
            drawGuides(drawX, drawY);
            
            // ·······························································
            //             PUNTO BLANCO --> Es el (0,0) relativo al sprite (esquina superior derecha (al estar volteado) de la imagen)
            // ·······························································
            ofSetColor(255);
            drawCircle(drawX, drawY, 5);
        }
        
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
    
// ##############################################################################################################################
//                                                  END OF MATRIX
// ##############################################################################################################################
}

void SpriteSheetManager::drawCircle(float x, float y, float radius){
    //ofNoFill();            // Solo el contorno
    //ofSetColor(255);       // Color blanco
    ofDrawCircle(x, y, radius);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
}

// *** DRAWS DEBUG ***
void SpriteSheetManager::drawRegion(float x, float y){
    ofNoFill();            // Solo el contorno
    ofSetColor(255,255,255);   // Color Blanco
    ofDrawRectangle(x, y, REGION_WIDTH, REGION_HEIGHT);
    ofSetColor(255);       // Resetear color para el sprite (por si acaso)
}

void SpriteSheetManager::drawGuides(float x, float y) {
    // Recuperamos los datos del hitbox para dibujar
    // Usamos la Base porque al dibujar ya aplicamos la escala con ofScale

    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // Se puede poner auto: "Oye, tú ya sabes qué tipo de dato devuelve la función getHitbox(), así que asígnale ese tipo automáticamente a la variable data"
    // auto data = collisionManager->getBaseHitbox();
    // Pero pondremos Hitbox para más claridad
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    HitboxData hitbox = collisionManager->getBaseHitbox();
    
    // Recuperamos el ÚLTIMO resultado de colisión que se calculó en el update
    bool isHitWall = collisionManager->getLastResult().hasAnyWall();
    
    // Region Base
    drawRegion(x, y);
    // Hitbox GUIA
    drawHitBox(x, y, hitbox, isHitWall);
    // HitWall
    drawHitWall(x, y, hitbox);
    // HitFloor
    drawHitFloor(x, y, hitbox);
}

void SpriteSheetManager::drawHitBox(float x, float y, const HitboxData& hitbox, bool isHitWall) {
    // DECISIONES DE COLOR
    if (isHitWall) {
        // DIBUJAMOS EL RELLENO (Rojo muy transparente)
        ofFill();
        ofSetColor(255, 0, 0, 40); // 40 es un rojo muy suave, cámbialo a tu gusto
        ofDrawRectangle(x, y, hitbox.width, hitbox.height);
        ofNoFill();
        //ofSetColor(255, 0, 0, 255);
    } else {
        // ESTADO NORMAL: Solo contorno blanco
        ofNoFill();
        ofSetColor(255);
    }
    // DIBUJAR HITBOX
    ofDrawRectangle(x, y, hitbox.width, hitbox.height);
    // Resetear color para el sprite (por si acaso)
    ofSetColor(255);
}

void SpriteSheetManager::drawHitWall(float x, float y, const HitboxData& hitbox) {
    ofSetColor(255, 0, 0);
    ofDrawLine(x + hitbox.width, y,
               x + hitbox.width, y + hitbox.height);
}

void SpriteSheetManager::drawHitFloor(float x, float y, const HitboxData& hitbox) {
    
    float floorY = y + hitbox.height;
    
    ofSetColor(0, 0, 255);
    ofDrawLine(x, floorY,
               x + hitbox.width, y + hitbox.height);
    
    //HITFLOOR RAY
    ofSetColor(64, 224, 208);
    
    // Su posición X es la base (x) + el desplazamiento del slider (hitFloorRayX)
    float rayX = x + hitbox.floorRayX;
    ofDrawLine(rayX, floorY, rayX, floorY - 15);
    ofDrawCircle(rayX, floorY, 3); // El punto de contacto exacto
}




// ------------------- GETTERS - SETTERS -------------------

// Recupera el factor de escala actual para la hoja de sprites
float SpriteSheetManager::getCurrentScale() const {
    return currentScale;
}

// Establece el factor de escala para la hoja de sprites
void SpriteSheetManager::setCurrentScale(float scaleFactor) {
    // Asigna el nuevo factor de escala
    this->currentScale = scaleFactor;
}

// *** REGION DIMENSIONS ***
int SpriteSheetManager::getRegionWidth() const {
    return REGION_WIDTH;
}
int SpriteSheetManager::getRegionHeight() const {
    return REGION_HEIGHT;
}




