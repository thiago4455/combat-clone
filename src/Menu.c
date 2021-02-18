#include "Menu.h"

void MenuEventHandler(MANAGER_FUNCION_TYPE function_type, ALLEGRO_EVENT event, char event_type){
    switch (function_type)
    {
    case GAME_INPUT:
        InputManager(event.keyboard.keycode, event_type);
        break;
    
    case GAME_RENDERER:
        Renderer();
        break;
    
    default:
        break;
    }
}


void InputManager(unsigned char key, char type){

}


void Renderer(){
    al_clear_to_color(al_map_hex(0x210000));


    al_flip_display();
}