#include "Menu.h"
#include "BaseGame.h"
#include <stdio.h>

extern ALLEGRO_DISPLAY *display;
extern int gameState;
extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;

void MenuEventHandler(MANAGER_FUNCION_TYPE function_type, unsigned char event_data, char event_type){
    switch (function_type)
    {
    case GAME_INPUT:
        InputManager(event_data, event_type);
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

struct MenuButton buttons[2];
ALLEGRO_FONT *font;

void InitMenu(){
    buttons[0] = createButtonRelativeToCenter(0, -80, 300, 80, START_SIMPLE_GAME, "Iniciar");
    buttons[1] = createButtonRelativeToCenter(0, 80, 300, 80, EXIT, "Sair");
    font = al_load_font("assets/Ranchers-Regular.ttf", 50, 0);
}

void Renderer(){
    al_clear_to_color(al_map_hex(0x212121));
    bool isMouseHover = false;
    int i;
    for (i = 0; i < sizeof(buttons)/sizeof(struct MenuButton); i++)
    {
        if(isButtonHover(buttons[i])){
            al_draw_filled_rectangle(buttons[i].x0, buttons[i].y0, buttons[i].x1, buttons[i].y1, al_map_hex(0x2121f0));
            isMouseHover = true;
        } else 
        al_draw_filled_rectangle(buttons[i].x0, buttons[i].y0, buttons[i].x1, buttons[i].y1, al_map_hex(0x2121a0));
        al_draw_text(font, al_map_hex(0xffffff), buttons[i].x, buttons[i].y - 30, ALLEGRO_ALIGN_CENTRE, buttons[i].text);
    }

    if(isMouseHover)
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
    else
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    al_flip_display();
}

int isButtonHover(struct MenuButton button){
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    int x = al_get_mouse_state_axis(&state, 0);
    int y = al_get_mouse_state_axis(&state, 1);
    if (x >= button.x0 && x <= button.x1 && y >= button.y0 && y <= button.y1){
        if(state.buttons & 1)
            handleClick(button.action);
        return true;
    }
    return false;
}

void handleClick(MENU_ACTION action){
    al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    switch (action)
    {
        case EXIT:
            gameState = STATE_EXIT;
            printf("Exit\n");
            break;
        
        case START_SIMPLE_GAME:
            G_Start();
            gameState = STATE_GAME;
            break;
        
        default:
            break;
    }
}

struct MenuButton createButtonRelativeToCenter(int x, int y, int w, int h, MENU_ACTION action, char const *text){
    int x0 = WINDOW_WIDTH/2 + x;
    int y0 = WINDOW_HEIGHT/2 + y;
    return (struct MenuButton) {
        x0 - w/2,
        x0 + w/2,
        y0 - h/2,
        y0 + h/2,
        action,
        x0,
        y0,
        text
    };
}