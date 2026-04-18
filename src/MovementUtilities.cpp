#include "MovementUtilities.h"

// *** CONSTRUCTOR ***
MovementUtilities::MovementUtilities() {
    // Nos aseguramos de que el mapa empiece totalmente vacío.
    movements.clear();
}

// *** DESTRUCTOR ***
MovementUtilities::~MovementUtilities() {
    // Liberamos los recursos del mapa. Al llamar a clear(), se invoca el
    // destructor de cada objeto 'Movement' almacenado. Esto es vital para
    // evitar fugas de memoria si en el futuro los movimientos contienen
    // punteros a otros objetos o recursos de vídeo.
    movements.clear();
    
    ofLogNotice("MovementUtilities") << "Destructor: Memoria del mapa de movimientos liberada.";
}

// *** CARGA ARCHIVO JSON ***
void MovementUtilities::loadMovements(const std::string& filename) {
    ofxJSONElement json;
    
    // Intenta abrir el archivo JSON
    if (!json.open(filename)) {
        ofLogError("MovementUtilities::loadMovements") << "No se pudo cargar el archivo JSON: " << filename;
        return;
    }
    
    // Limpiamos el mapa antes de cargar para evitar duplicados si se llama dos veces
    movements.clear();
    
    // Delegamos el procesamiento del contenido al método privado
    parseMovementsJson(json);
    
    ofLogNotice("MovementUtilities") << "Carga completada. Movimientos cargados: " << movements.size();
}



// Método interno para procesar el JSON (evita duplicar código si cargamos varios archivos)
void MovementUtilities::parseMovementsJson(ofxJSONElement& json) {
    // Iteramos sobre el array de "sprite_sheets"
    const auto& sheets = json["sprite_sheets"];
    for (int i = 0; i < sheets.size(); i++) {
        
        // Guardamos el nombre del archivo de imagen (ej: "Sprite_1.png")
        string currentSheetFile = sheets[i]["file"].asString();
        const auto& movementsInSheet = sheets[i]["movements"];
        
        // Itera sobre cada movimiento dentro de esta hoja de sprites
        for (const auto& movementKey : movementsInSheet.getMemberNames()) {
            const auto& jsonMovement = movementsInSheet[movementKey];
            
            Movement movement;
            movement.name = movementKey;
            movement.spriteSheetName = currentSheetFile; // Guardamos el nombre del archivo vinculado
            
            // Datos básicos
            movement.row = jsonMovement["row"].asInt();
            movement.numRegions = jsonMovement["numRegions"].asInt();
            movement.isTransition = jsonMovement["isTransition"].asBool();
            movement.frameInterval = jsonMovement["frameInterval"].asFloat();
            // Cargamos el target_frame (si no existe, pondrá 0 por defecto)
            movement.target_frame = jsonMovement.get("target_frame", 0).asInt();
            
            // --- ORIGIN TYPE ---
            // Usamos .get() con "NONE" como valor por defecto,
            // así los movimientos sin el campo (JSON antiguos) no rompen la carga.
            std::string originStr = jsonMovement.get("origin_type", "NONE").asString();

            if      (originStr == "TO_IDLE")    movement.originType = TransitionOrigin::TO_IDLE;
            else if (originStr == "TO_WALK")    movement.originType = TransitionOrigin::TO_WALK;
            else if (originStr == "TO_RUN")     movement.originType = TransitionOrigin::TO_RUN;
            else if (originStr == "WALK_TURN")  movement.originType = TransitionOrigin::WALK_TURN;
            else if (originStr == "RUN_TURN")   movement.originType = TransitionOrigin::RUN_TURN;
            else if (originStr == "TURN")       movement.originType = TransitionOrigin::IDLE_TO_TURN;
            else if (originStr == "TURN_TO_RUN") movement.originType = TransitionOrigin::TURN_TO_RUN;
            else                                movement.originType = TransitionOrigin::NONE;
            
            
            
            
            // --- CARGA DE TRANSICIONES (MAPAS) ---
            
            // Puntos de salida de STOP PS
            for (const auto& transition : jsonMovement["stop_transitions"].getMemberNames()) {
                movement.stop_transitions[std::stoi(transition)] = jsonMovement["stop_transitions"][transition].asString();
            }
            
            // Puntos de salida de TURN PS
            for (const auto& transition : jsonMovement["turn_transitions"].getMemberNames()) {
                movement.turn_transitions[std::stoi(transition)] = jsonMovement["turn_transitions"][transition].asString();
            }
            
            // Puntos de salida de CHANGE
            for (const auto& transition : jsonMovement["change_transitions"].getMemberNames()) {
                movement.change_transitions[std::stoi(transition)] = jsonMovement["change_transitions"][transition].asString();
            }
            
            // Puntos de salida de JUMP
            for (const auto& transition : jsonMovement["jump_transitions"].getMemberNames()) {
                movement.jump_transitions[std::stoi(transition)] = jsonMovement["jump_transitions"][transition].asString();
            }
            
            // Guardamos el movimiento procesado en nuestro mapa interno
            movements[movement.name] = movement;
        }
    }
}



// ------------------- GETTERS - SETTERS -------------------

// --- VERSIÓN PARA OBTENER REFERENCIA (MODIFICACIÓN) ---
Movement& MovementUtilities::getMovement(const std::string& name) {
    // No devolvemos copia, devolvemos el acceso directo al mapa
    // Nota: Es importante estar seguros de que existe antes de llamar a esta
    return movements[name];
}


// Comprueba si existe la clave en el mapa
bool MovementUtilities::exists(string name) {
    return movements.find(name) != movements.end();
}

// Devuelve referencia al mapa completo (útil para el bucle de carga de texturas en el Manager)
const std::map<string, Movement>& MovementUtilities::getAllMovements() const {
    return movements;
}


