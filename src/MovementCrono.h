#pragma once

#include "ofMain.h"

// ****************************************** CLASE MOVEMENT CRONO ****************************************** //
// Clase encargada de la gestión del tiempo y el avance de los fotogramas (regiones).
// Actúa como un motor de animación independiente que el MovementManager consulta.
class MovementCrono {
    
public:
    // !!! MOTIVO CONSTR / DESTR !!!
    // Con el CONSTRUCTOR Inicializamos el timer y la region actual a cero de forma explícita.
    // El constructor asegura que el reloj (timer) y la región actual empiecen en cero.
    // Sin esto, podrías empezar una animación en un frame aleatorio o con tiempo acumulado.
    
    // Para el DESTRUCTOR, en esta clase no manejamos memoria dinámica (punteros), por lo que no hay
    // recursos que liberar manualmente. Sin embargo, lo definimos para mantener
    // la estructura del motor y facilitar la depuración si en el futuro
    // necesitáramos registrar el cierre del crono.
    // Se declara por consistencia y futuros logs.
    
    // *** CONSTRUCTOR ***
    MovementCrono();
    // *** DESTRUCTOR ***
    ~MovementCrono();
    
    // *** UPDATE REGION ***
    // Actualiza el contador de tiempo.
    // Retorna TRUE si la región ha cambiado en este frame.
    // dt: El deltaTime (ofGetLastFrameTime).
    // interval: El tiempo que debe durar cada región (desde MovementUtilities).
    bool updateRegion(float dt, float interval);
  
    
    // ------------------- GETTERS - SETTERS -------------------
    // Devuelve la región actual
    int getCurrentRegion() const;
    // Modifica la región actual
    void setCurrentRegion(int region);
    // Devuelve el Frame Interval Global
    float getFrameInterval() const;
    // Modifica el Frame Interval Global
    void setFrameInterval(float interval);
    
    
private:
    // *** VARIABLES DE CONTROL ***
    
    // Acumulador de tiempo transcurrido desde el último cambio de región.
    float timer;
    
    // El índice de la región (fotograma) en la que nos encontramos.
    int currentRegion;
    
    // Intervalo de fotogramas general
    float frameInterval = 0.1f;
};