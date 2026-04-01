#include "Interactor.h"

// Constructor siguiendo tu estilo de Movimiento.cpp
Interactor::Interactor(
            ofVec2f p1,
            ofVec2f p2,
            InteractorType type,
            string name,
            float influenceRadius,
            bool hit,
            bool gui
            ) : p1(p1), p2(p2), type(type), name(name), influenceRadius(influenceRadius), hit(hit), gui(gui){}
