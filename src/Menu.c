#include "Menu.h"
#include "BaseGame.h"
#include <stdio.h>

extern ALLEGRO_DISPLAY *display;
extern ManagerGameState gameState;

struct ALLEGRO_BITMAP *background_bitmap;

void MenuEventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type){
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

struct MenuButton buttons[3];
ALLEGRO_FONT *font;

void InitMenu(){
    buttons[0] = createButtonRelativeToCenter(250, -300, 300, 50, START_SIMPLE_GAME, "Local");
    buttons[1] = createButtonRelativeToCenter(250, -210, 300, 50, START_ONLINE_GAME, "Online");
    buttons[2] = createButtonRelativeToCenter(250, -120, 300, 50, EXIT, "Sair");
    font = al_load_font("assets/Ranchers-Regular.ttf", 30, 0);
    background_bitmap = al_load_bitmap("assets/bg.png");
}

void Renderer(){
    al_clear_to_color(al_map_hex(0x212121));
    al_draw_scaled_bitmap(background_bitmap,0,0,800,800,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0);
    bool isMouseHover = false;
    int i;
    for (i = 0; i < sizeof(buttons)/sizeof(struct MenuButton); i++)
    {
        if(isButtonHover(buttons[i])){
            al_draw_filled_rectangle(buttons[i].x0, buttons[i].y0, buttons[i].x1, buttons[i].y1, al_map_hex(0x8B4513));
            isMouseHover = true;
        } else 
        al_draw_filled_rectangle(buttons[i].x0, buttons[i].y0, buttons[i].x1, buttons[i].y1, al_map_hex(0xA0522D));
        al_draw_text(font, al_map_hex(0xffffff), buttons[i].x, buttons[i].y - 20, ALLEGRO_ALIGN_CENTRE, buttons[i].text);
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
    switch (action)
    {
        case EXIT:
            gameState = STATE_EXIT;
            printf("Exit\n");
            break;
        
        case START_SIMPLE_GAME:
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
            G_StartLocal();
            gameState = STATE_GAME;
            break;
        
        case START_ONLINE_GAME:
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
            G_StartOnline();
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