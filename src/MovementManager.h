#pragma once
#include <memory>
#include <map>
#include <string>
#include <unordered_map>

#include "Movimiento.h"
#include "ofxJSON.h"

// !!! MOTIVO NO forward declarations !!!
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// Para pasar Tipos InputState
// "Copia y pega aquí todo el contenido de InputManager.h. Necesito saber exactamente cuánto mide, qué métodos tiene y qué variables guarda".
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
#include "InputManager.h"
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// Son como hijos de MovementManager, solo viven aquí, por eso debe conocer todo de ellos
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
#include "MovementUtilities.h"
#include "MovementCrono.h"


// !!! MOTIVO FORWARD DECLARATIONS !!!
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// FORWARD DECLARATION, MEJOR SI SOLO USAS UN PUNTERO:
// "Va a existir una clase llamada así. No te preocupes por los detalles ahora, solo resérvame un hueco para un puntero o una referencia".
// Si cambias algo en la clase el compilador no tiene que volver a compilarla porque realmente no sabe qué hay dentro, solo que es una "clase".
// Si la Clase A necesita a la B, y la B necesita a la A, y ambas tienen #include, el compilador entra en un bucle infinito.  Usando class rompemos ese bucle.
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
class SpriteSheetManager;
class PhysicsManager;
class CollisionManager;


// *** ESTADOS LÓGICOS DEL PERSONAJE ***
// Estos estados representan lo que el personaje está haciendo lógicamente, independientemente de la animación exacta.
// La diferencia entre Estado y Movimiento es como la diferencia entre "Querer ir a comprar" y "Caminar hacia la tienda".
enum class MovementState {
    IDLE,
    WALKING,
    RUNNING,
    TURNING,    // Cuando está ejecutando una animación de giro
    STOPPING,   // Cuando está en una transición hacia IDLE
    // JUMPING,
    // FALLING
};

// *** INTENCIONES LIMPIAS ***
enum class MovementCommand {
    
        // PARADA: Sin input O choque frontal contra pared
        GO_STOP,       // !hasAnyDirection || (wantsRight && wallR) || (wantsLeft && wallL)
        
        // GIROS: El input es opuesto a la mirada actual
        GO_TURN, // wantsRight && !isFacingRight ||  wantsLeft && isFacingRight
        // GO_TURN_RIGHT // wantsRight && !isFacingRight
        // GO_TURN_LEFT // wantsLeft && isFacingRight

        // MOVIMIENTO: Mirada e Intención alineadas, sin paredes bloqueando
        GO_FORWARD,    // wantsRight && !wantsRun || wantsLeft && !wantsRun
        GO_FORWARD_FAST      // wantsRight && wantsRun || wantsLeft && wantsRun
        // GO_RIGHT,      // wantsRight && !wantsRun
        // GO_LEFT,       // wantsLeft && !wantsRun
        // GO_RUN_RIGHT,  // wantsRight && wantsRun
        // GO_RUN_LEFT    // wantsLeft && wantsRun
    
    
        // GO_JUMP
};

// *** MOMENTOS DE ANIMACIÓN ***
enum class MovementMoment {
    CHANGE,
    TRIGGER,
    // FINISHED
};


// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// La única diferencia real entre un STRUCT y una CLASS es que en el struct todo es public por defecto.
// Por lo demás, son hermanos gemelos: ambos pueden tener métodos, constructores y destructores.
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬

// ****************************************** CLASE MOVMENT MANAGER ****************************************** //
// Clase que gestiona los movimientos del personaje
class MovementManager {

// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// "Public" es lo que la clase hace por los demás.
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
public:
    
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // INFO CONSTRUCTORES / DESTRUCTORES
    // Si hay new: "Crítico para liberar memoria dinámica manual y evitar leaks."
    // Si hay punteros externos (*): "Asegura la desconexión de dependencias externas y evita punteros colgantes."
    // Si solo hay variables simples: "Garantiza un estado inicial 'limpio' y evita que el programa lea basura de la RAM."
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    
    // !!! MOTIVO CONSTR / DESTR !!!
    // Usamos 4 Punteros Inteligentes (std::unique_ptr), El unique_ptr es como un empleado que se despide y limpia su mesa solo.
    // Usamos Punteros Simples (referencias) que debemos limpiar (poner a nullptr) para evitar errores.
    // Los punteros normales (*) son como post-its con direcciones: si no los quitas (nullptr),
    // alguien podría intentar ir a esa dirección cuando la casa ya ha sido demolida.
    // Por eso usamos el Constructor para dar seguridad y el Destructor para limpiar.
    // Memoria dinámica es cuando usamos "new" o "delete".
    // *** CONSTRUCTOR ***
    MovementManager();
    // *** DESTRUCTOR *** (lleva la virgulilla ~)
    ~MovementManager();
    
    
    // *** CONFIGURACIÓN Y CONEXIÓN ***
    // Inicializa el gestor de movimientos desde un archivo JSON, asociando un gestor de hojas de sprites y un gestor de entradas
    // Más abajo en private definimos el InputManager como puntero, porque necesitamos consultarlo constantemente.
    // Pero no definimos el SpriteSheetManager nos viene de una referencia en ofApp, no necesita guardar su contacto.
    void setup(const std::string& filename, SpriteSheetManager& spriteSheetManager, InputManager& input, PhysicsManager& physics, CollisionManager& collision);
    
    // *** CICLO DE VIDA ***
    // Actualiza ESTADOS segun INTENCIONES y gestiona si debe hacer cambio de Region
    void update();
    
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // Los Getters y Setters se pueden definir en el .h como "inline"
    // (dentro de la llave { }) no necesitan escribirse en el .cpp
    // EJEMPLO:
    // int getCurrentRow() const { return currentRow; }
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // Cuando encontramos:
    // CONST tipo funcion() CONST
    // El primer const significa: "El puntero que devuelvo es constante (no puedes modificar lo que apunta)."
    // El último const significa: "Esta función no modifica el estado interno del MovementManager."
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    

    // ------------------- GETTERS - SETTERS -------------------
    
    // ==========================================
    //      GETS CURRENT
    // ==========================================
    // Obtiene la fila actual del sprite sheet correspondiente al movimiento actual.
    int getCurrentRow() const;
    // Obtiene la siguiente región a la que debe llegar.
    int getNextOutRegion() const;
    // Obtiene el archvio del movimiento actual
    string getCurrentFile() const;
    // Obtiene el intervalo de frame del movimiento actual, si está definido, de lo contrario utiliza el intervalo global. (en public porque la GUI la usa)
    float getCurrentMovementFrameInterval() const;
    // Obtiene el movimiento actual
    const Movement* getCurrentMovement() const;
    // Obtiene el movimiento actual
    std::string getCurrentMovementName() const;
    
    
    // ==========================================
    //      VARIABLES DE PERSONAJE
    // ==========================================
    // *** DIRECTION ***
    // Obtiene la dirección del personaje
    bool getIsFacingRight();
    // Invierte la dirección del personaje
    void toggleIsFacingRight();
    // *** SCALE ***
    // Obtiene la escala del personaje
    const float getScaleFactor() const;
    // Modifica la escala del personaje
    void setScaleFactor(float scaleFactor);
    
    
    // ==========================================
    //      GETS - SETS: CRONO
    // ==========================================
    // Obtiene la región actual (frame) del sprite sheet correspondiente al movimiento actual.
    int getCurrentRegion() const;
    // Obtiene el intervalo de fotogramas global
    float getFrameInterval() const;
    // Establece el intervalo de fotogramas global
    void setFrameInterval(float interval);
 
    // ==========================================
    //      GETS - SETS: MOVEMENTS UTILITIES
    // ==========================================
    // Obtiene el mapa de todos los movimientos disponibles
    const std::map<std::string, Movement>& getMovements() const;
    // Obtiene el intervalo de fotogramas para un movimiento específico
    float getMovementFrameInterval(const std::string& movementName) const;
    // Establece el intervalo de fotogramas para un movimiento específico
    void setMovementFrameInterval(const std::string& movementName, float interval);

    
    // ==========================================
    //    GETS STATES ( EXCLUSIVOS PARA GUI )
    // ==========================================
    // Retorna un string con el estado actual del personaje
    std::string getCurrentState() const;
    // Retorna un string con el estado objetivo del personaje
    std::string getTargetState() const;
    // Retorna true si está esperando una transición, false en caso contrario
    bool isWaitingForTransition() const;
    

    
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
// "Private" es cómo la clase se organiza a sí misma para hacer su trabajo.
// ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬

private:
    // *** VARIABLES DE PERSONAJE ***
    bool isFacingRight = true;         // Dirección del personaje
    float scaleFactor = 1.0f;          // Factor de escala, todos los otros managers copian este

    // *** VARIABLES DE MOVIMIENTOS ***
    bool waitingForTransition = false; // Indica si se está esperando una transición
    int targetRegion = -1;             // Región objetivo para la transición
    
    // *** VARIABLES DE ESTADO DE PHYSICS ***
    bool isGrounded = false;
    bool isWalledLeft = false;
    bool isWalledRight = false;
    
    
    // *** CURRENT ***
    // Fila actual en la hoja de sprites
    int currentRow = 0;
    // Puntero único que almacena el movimiento actual (null por defecto).
    std::unique_ptr<Movement> currentMovement = nullptr;
    // Variable para almacenar el nombre del movimiento actual.
    std::string currentMovementName;
    
    // *** ENUMS ***
    // Variable para guardar el estado actual (inicializado en IDLE).
    MovementState currentState = MovementState::IDLE;
    // Variable para guardar el estado objetivo (inicializado en IDLE)
    MovementState targetState = MovementState::IDLE;
    // Variable para guardar el commando actual (inicializado en GO_STOP).
    MovementCommand currentCommand = MovementCommand::GO_STOP;

    

    // *** REFERENCIAS POR VALOR DE MOVEMENT MANAGEMENT ***
    // Es quien tiene la lista de movements
    MovementUtilities movementUtilities;
    // Es quien gestiona el paso de frames
    MovementCrono movementCrono;

    
    // *** PUNTEROS A OTROS MANAGERS ***
    // Puntero al gestor de entradas
    InputManager* inputManager = nullptr;
    // Puntero al gestor de físicas
    PhysicsManager* physicsManager = nullptr;
    // Puntero al gestor de Sprites
    SpriteSheetManager* spriteSheetManager = nullptr;
    // Puntero al gestor de Colisiones
    CollisionManager* collisionManager = nullptr;
    
    
    
    
    
    // ##############################################################################################################################
    //                                                         FUNCIONES
    // ##############################################################################################################################
    
    // ==========================================
    //     TRADUCTORES (Intención --> Estado)
    // ==========================================
    // Comando --> Estado Objetivo
    void updateIntent();
    // Intención --> Comando
    void translateIntent();

    
    // ==========================================
    //      UPDATE STATE ( JUEZ )
    // ==========================================
    // Switch de estados y llama a playMovement o handleTransition según convenga.
    void updateState(MovementState targetState);
    // !!!!!!! JUEZ DE SUELO !!!!!!! //
    void updateGroundedState(MovementState targetState);
    // !!!!!!! JUEZ DE AIRE !!!!!!! //
    void updateAirState(MovementState targetState);
    
    // ==========================================
    //      HANDELERS DE ESTADOS
    // ==========================================
    // Gestionan los cambios de estado segun el CURRENT TARGET (nombre de la función)
    void handleIdleState(MovementState target, MovementMoment moment);
    void handleWalkingState(MovementState target, MovementMoment moment);
    void handleRunningState(MovementState target, MovementMoment moment);
    void handleTurningState(MovementState target, MovementMoment moment);
    void handleStoppingState(MovementState target, MovementMoment moment);
    
    // ==========================================
    //      GESTIÓN DE TRANSICIONES
    // ==========================================
    void triggerTransition();
    void triggerGroundedTransition();
    void triggerAirTransition();
    void finishedTransition();
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    // & (Referencia): Evita que el ordenador cree una "copia" de toda la estructura InputState en la memoria cada vez que llamas a la función.
    // Las referencias (&) siempre apuntan a algo que existe, así que el código queda más limpio y seguro.
    
    // * (Puntero): Los punteros pueden ser nullptr (nulos), lo que te obligaría a poner un if (intent != nullptr) por seguridad.
    
    // El const: El const garantiza que no vas a modificar accidentalmente los valores de intent dentro de la función.
    
    // También si el objeto tiene más de uno o dos bytes, y no necesitas modificarlo, pásalo por const & → por referencia
    // En cambio tipos primitivos y enums → por valor
    // ¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
    void finishedGroundedTransition();
    void finishedAirTransition();



    // ==========================================
    //      MOTOR
    // ==========================================
    // Es private porque No quieres que desde ofApp.cpp alguien llame por error a movementManager.updateFrame().
    // Si lo hicieran, los frames podrían avanzar el doble de rápido o saltar cuando no deben, rompiendo la animación.
    // Como es privado, sabes que nadie fuera de esa clase depende de esa función.
    void updateFrame();
    
    // ==========================================
    // $$$$$$$$$$$$$ FISICAS $$$$$$$$$$$$$
    // ==========================================
    void handleMovementPhysics(const std::string& name);
    
    // ==========================================
    //      EJECUTOR
    // ==========================================
    // Inicia el movimiento especificado por su nombre, con parámetro opcional para la región inicial. No decide estados
    void playMovement(const std::string& movementName, int region = 0);
    
    // ==========================================
    //      OTEADOR (mira al futuro)
    // ==========================================
    // Mira el JSON del movimiento actual.
    // Busca el transitionRegion (punto de salida) más cercano.
    // Activa la bandera waitingForTransition = true.
    // No hace nada más. Se queda esperando a que el "reloj" la avise.
    void handleTransition();



    // TURN_TO_RUN solo se puede dar desde IDLE, pero el estado es TURNING
    // Para no añadir un estado nuevo, que además entraría en conflicto con RUN_TO_TURN
    // Creamos este flag que solo se evaluará en currentState IDLE con targetState RUN.
    bool flag_turn_to_run = false;
    
    // |||||||||||||||||||||||||||| [NOTA PARA EL FUTURO] |||||||||||||||||||||||||||||||||
    /*
    // Si esto empieza a darse tendremos que crear el Objeto Petición i modificar el TRADUCTOR y el JUEZ en consecuencia.
         struct MovementRequest {
             MovementState state;
             std::string variant = ""; // Aquí el traductor puede decir "TO_RUN"
         };
    */
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
};
