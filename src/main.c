#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Client.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

int main() {
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();

    display = al_create_display(800, 800);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0/60);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();

    bool shouldClose = false;
    bool render = true;
    al_start_timer(timer);

    float position = 0;


    create_connection();
    send_message(&position);
    while(!shouldClose){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            shouldClose = true;
            break;

        
        case ALLEGRO_EVENT_TIMER:
                al_clear_to_color(al_map_rgb(255,255,255));
                // RENDER CODE
                float pos2 = recive_message();
                al_draw_filled_rectangle(position, 400, 50+position, 450, al_map_rgb(255,0,0));
                position++;
                al_draw_filled_rectangle(position, 600, 50+position, 650, al_map_rgb(0,0,255));

                al_flip_display();
                // send_message(&position);
        default:
            break;
        }
    }

    return 0;
}