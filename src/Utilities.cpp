#include "Utilities.h"
#include "ofMain.h"

// Mapea una cadena de texto que representa una tecla a su correspondiente código de tecla.

int mapKeyStringToCode(const std::string& keyString) {
    
    // Mapa estático que asocia cadenas de texto con códigos de teclas.
    static const std::unordered_map<std::string, int> keyMap = {
        {"OF_KEY_RIGHT", OF_KEY_RIGHT}, // Asocia "OF_KEY_RIGHT" con el código OF_KEY_RIGHT
        {"OF_KEY_LEFT", OF_KEY_LEFT},   // Asocia "OF_KEY_LEFT" con el código OF_KEY_LEFT
        {"OF_KEY_UP", OF_KEY_UP},       // Asocia "OF_KEY_UP" con el código OF_KEY_UP
        {"OF_KEY_DOWN", OF_KEY_DOWN},   // Asocia "OF_KEY_DOWN" con el código OF_KEY_DOWN
        {"1", 49},                     // Asocia "1" con el código ASCII de '1'
        {"NONE", 0}                     // Asocia "NONE" con el código 0 (ninguna tecla)
    };
    
    // Busca la cadena de texto en el mapa.
    auto it = keyMap.find(keyString);
    
    // Si la cadena se encuentra en el mapa,
    if (it != keyMap.end()) {
        // Devuelve el código de tecla correspondiente.
        return it->second;
        
    // Si la cadena no se encuentra en el mapa,
    } else {
        // Registra un error y devuelve 0.
        ofLogError("mapKeyStringToCode") << "Tecla desconocida: " << keyString;
        
        // Valor por defecto si la tecla no se encuentra
        return 0;
    }
}
