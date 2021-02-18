#include "Manager.h"

#include "Menu.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;
int gameState;

void M_Init(){
    gameState = STATE_MENU;
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_hex(0x212121));
    al_flip_display();
}

void M_Run(){
    bool shouldClose = false;
    bool render = true;
    al_start_timer(timer);

    // G_Start();

    while(!shouldClose){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                shouldClose = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                EventManager(GAME_INPUT, event, 0);
                break;
            case ALLEGRO_EVENT_KEY_UP:
                EventManager(GAME_INPUT, event, 1);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                EventManager(GAME_INPUT, event, 2);

            case ALLEGRO_EVENT_TIMER:
                EventManager(GAME_RENDERER, event, 0);
            
            default:
                break;
        }
    }
}

void M_Cleanup(){
    al_destroy_display(display);
    al_destroy_event_queue(queue);
}

void EventManager(MANAGER_FUNCION_TYPE function_type, ALLEGRO_EVENT event, char event_type){
    switch (gameState)
    {
        case STATE_MENU:
            MenuEventHandler(function_type, event, event_type);
            break;
        
        default:
            break;
    }
}