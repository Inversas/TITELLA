#include "InputManager.h"

// Iniciar el InputManager
void InputManager::setup(MovementManager& movementManager) {
     // Asigna el gestor de movimientos al puntero miembro de la clase
    this->movementManager = &movementManager;

}

void InputManager::keyPressed(int key) {
    
    //***  CONTROL PRIMERA VEZ TECLA PRESIONADA ***//
    if (pressedKeys.count(key) == 0 ) {

              
        //*** TECLA ÚNICA ***//
        //Si no había ninguna tecla presionada
        if (pressedKeys.empty()) {
                      
            // -->
            if (key == OF_KEY_RIGHT) {
                movementManager->setControlKeys("-->");
            }
            // <--
            else if(key == OF_KEY_LEFT){
                movementManager->setControlKeys("<--");
            }
            else if(key == OF_KEY_ESC){
                movementManager->setControlKeys("Salir");
            }
            // OTRA TECLA
            else {
                movementManager->setControlKeys(std::string(1, static_cast<char>(key)));
            }
            
            //Se inserta la tecla en el conjunto de teclas presionadas
            pressedKeys.insert(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //SALIR EN CASO DE TECLA ÚNICA
            return;
        }

        //*** COMBO DOBLE ***//
        //Si solo había una tecla presionada
        if (pressedKeys.size() == 1) {
            
            //Si se estaba presionando "-->" o "<--" y se presiona 1
            if (key == 49 && (pressedKeys.count(OF_KEY_RIGHT) || pressedKeys.count(OF_KEY_LEFT))) {
                // "--> + 1"
                if (pressedKeys.count(OF_KEY_RIGHT) ) {
                    movementManager->setControlKeys("--> + 1");
                }
                // "<-- + 1"
                else if(pressedKeys.count(OF_KEY_LEFT)){
                    movementManager->setControlKeys("<-- + 1");
                }
            }
            
            //Si se estaba presionando "<--" y se presiona "-->"
            if ( key == OF_KEY_RIGHT  && pressedKeys.count(OF_KEY_LEFT) ) {
                movementManager->setControlKeys("<-- + -->");
            }
            
            //Si se estaba presionando "-->" y se presiona "<--"
            if (key == OF_KEY_LEFT  && pressedKeys.count(OF_KEY_RIGHT) ) {
                movementManager->setControlKeys("--> + <--");
            }
            
            //Si se estaba presionando 1 y se presiona "-->" o "<--"
            if ( (key == OF_KEY_RIGHT || key == OF_KEY_LEFT)  && pressedKeys.count(49)) {
                //"1 + -->"
                if( key == OF_KEY_RIGHT ) {
                    movementManager->setControlKeys("1 + -->");
                }
                //"1 + <--"
                if( key == OF_KEY_LEFT ) {
                    movementManager->setControlKeys("1 + <--");
                }
            }
            
            //Se inserta la tecla en el conjunto de teclas presionadas
            pressedKeys.insert(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //FINALIZAR EN CASO DE COMBO DOBLE
            return;
        }
        
        //*** COMBO TRIPLE ***//
        if (pressedKeys.size() == 2) {
            
            //Si se estaba presionando 1 y "-->" y se presiona "<--"
            if (key == OF_KEY_LEFT  && (pressedKeys.count(49) && pressedKeys.count(OF_KEY_RIGHT)) ) {
                movementManager->setControlKeys("1 + --> + <--");
            }
            //Si se estaba presionando 1 y "<--" y se presiona "-->"
            if (key == OF_KEY_RIGHT  && (pressedKeys.count(49) && pressedKeys.count(OF_KEY_LEFT)) ) {
                movementManager->setControlKeys("1 + <-- + -->");
            }
            
            //Se inserta la tecla en el conjunto de teclas presionadas
            pressedKeys.insert(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //FINALIZAR EN CASO DE COMBO TRIPLE
            return;
        }
    }
}


void InputManager::keyReleased(int key) {

    // Si la tecla liberada estaba presionada
    if (pressedKeys.count(key) > 0) {
        
        //*** TECLA ÚNICA LIBERADA ***//
        if (pressedKeys.size() == 1) {
            
            //"--> OFF"
            if (key == OF_KEY_RIGHT) {
                movementManager->setControlKeys("--> OFF");
            }
            //"<-- OFF"
            else if(key == OF_KEY_LEFT){
                movementManager->setControlKeys("<-- OFF");
            }
            //OTRA TECLA OFF
            else {
                movementManager->setControlKeys( std::string(1, static_cast<char>(key)) + " OFF");
            }
            
            // SACAR el OFF DE LA TECLA QUE SE HA DEJADO DE PRESSIONAR
            std::string keys = movementManager->getControlKeys();
            keys = keys.substr(0, keys.length() - 4);
            
            // Se elimina la tecla en el conjunto de teclas presionadas
            pressedKeys.erase(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //SALIR EN CASO DE TECLA ÚNICA LIBERADA
            return;
        }

        
        //*** LIBERACIÓN COMBO DOBLE ***//
        if(pressedKeys.size() == 2){
            
            //Si se libera "-->" y se sigue presionando "<--"
            if (key == OF_KEY_RIGHT && pressedKeys.count(OF_KEY_LEFT)) {
                movementManager->setControlKeys("<-- + --> OFF");
            }
            
            //Si se libera "<--" y se sigue presionando "-->"
            if (key == OF_KEY_LEFT && pressedKeys.count(OF_KEY_RIGHT)) {
                movementManager->setControlKeys("--> + <-- OFF");
            }
            
            //Si se libera 1 y se sigue presionando "-->" o "<--"
            if (key == 49 && (pressedKeys.count(OF_KEY_RIGHT) || pressedKeys.count(OF_KEY_LEFT))) {
                // "--> + 1 OFF"
                if (pressedKeys.count(OF_KEY_RIGHT) ) {
                    movementManager->setControlKeys("--> + 1 OFF");
                }
                // "<-- + 1 OFF"
                else if(pressedKeys.count(OF_KEY_LEFT)){
                    movementManager->setControlKeys("<-- + 1 OFF");
                }
            }
            
            
            //Si se libera "-->" o "<--" y se sigue presionando 1
            if ((key == OF_KEY_RIGHT || key == OF_KEY_LEFT) && pressedKeys.count(49)) {
                //"1 + --> OFF"
                if (key == OF_KEY_RIGHT ) {
                    movementManager->setControlKeys("1 + --> OFF");
                }
                //"1 + <-- OFF"
                else if(pressedKeys.count(OF_KEY_LEFT)){
                    movementManager->setControlKeys("1 + <-- OFF");
                }
            }
            
            // Se inserta la tecla en el conjunto de teclas presionadas
            pressedKeys.erase(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //SALIR EN CASO DE TECLA COMBO DOBLE LIBERADA
            return;
        }
        
        
        //*** COMBO TRIPLE ***//
        if(pressedKeys.size() == 3){
            //Si se libera "-->" y se sigue presionando 1 y "<--"
            if (key == OF_KEY_RIGHT && (pressedKeys.count(OF_KEY_LEFT) && pressedKeys.count(49))) {
                movementManager->setControlKeys("1 + <-- + --> OFF");
            }
            if(key == OF_KEY_LEFT && (pressedKeys.count(OF_KEY_RIGHT) && pressedKeys.count(49))) {
                movementManager->setControlKeys("1 + --> + <-- OFF");
            }
            
            // Se inserta la tecla en el conjunto de teclas presionadas
            pressedKeys.erase(key);
            
            //Si el movimiento actual no es una transición
            if(!movementManager->currentMovement->isTransition){
                //Gestiona Movimientos por combinación de teclas
                movementManager->handleControlKeys();
            }
            
            //SALIR EN CASO DE TECLA COMBO TRIPLE LIBERADA
            return;
        }
    }    
}










