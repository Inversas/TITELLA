#pragma once

#include <map>
#include <string>

#include "ofMain.h"
#include "ofxJSON.h"

#include "Movimiento.h"


// ****************************************** CLASE MOVEMENT UTILITIES ****************************************** //
// Clase encargada de la carga, almacenamiento y consulta de los datos de movimientos (JSON)
class MovementUtilities {
    
public:
    // !!! MOTIVO CONSTR / DESTR !!!
    // Aunque el std::map gestiona su memoria, inicializamos aquí para asegurar un estado limpio.
    // En el destructor, vaciamos el mapa para garantizar que no queden referencias colgadas
    // antes de que el programa se cierre, especialmente útil si en el futuro Movement tuviera punteros.
    
    // *** CONSTRUCTOR ***
    MovementUtilities();
    
    // *** DESTRUCTOR ***
    ~MovementUtilities();
    
    // *** CARGA DE DATOS ***
    // Carga los movimientos desde un archivo JSON
    // Devuelve true si la carga fue exitosa.
    void loadMovements(const std::string& filename);

    

    
    // ------------------- GETTERS - SETTERS -------------------
    // Si se volviera a cargar el json y cambiara durante la ejecución
    // Deberíamos tener una versión para consulta:
    // Devuelve una copia del struct Movement asociado a un nombre.
    // Si no existe, devuelve un objeto Movement vacío (o maneja el error).
    //Movement getMovement(string name);
    /*// Devuelve una copia del struct Movement
     // ¿Por qué getMovement devuelve una copia y no una referencia?
     // Porque normalmente, cuando el personaje cambia de estado, quieres una "foto" de esos datos para trabajar.
     // Si devolviéramos una referencia y por algún motivo el mapa cambiara o se limpiara,
     // el puntero del personaje podría quedar apuntando a basura.
     // Al devolver el struct por valor, es seguro y ligero (tus structs Movement no son gigantescos).
     Movement MovementUtilities::getMovement(string name) {
         if (exists(name)) {
            return movements[name]; // Retorna una copia exacta
         }
     
         ofLogError("MovementUtilities::getMovement") << "No existe el movimiento: " << name;
         return Movement();
     } */
    
    // Y habría una versión para modificar, tendría otro nombre getMovementRef.
    // Dejamos solo la versión para modificar, porque sabemos que nunca cargaremos el Load JSON durante la ejecución
    // Además nos ahorramos memoria de las copias
    Movement& getMovement(const std::string& name);
    
    // Comprueba si un movimiento existe en el mapa
    bool exists(string name);
    
    // Acceso al mapa completo (por si el Manager necesita iterar sobre todos)
    const std::map<string, Movement>& getAllMovements() const;
        
    
private:
    // *** ALMACENAMIENTO ***
    // El mapa "sagrado" con todos los datos extraídos del JSON
    std::map<string, Movement> movements;
    
    // Método interno para procesar el JSON (evita duplicar código si cargamos varios archivos)
    void parseMovementsJson(ofxJSONElement& json);
};