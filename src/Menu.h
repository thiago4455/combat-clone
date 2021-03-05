#ifndef MENU_H
#define MENU_H
#include "Manager.h"

void InitMenu();
void MenuEventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type);
void InputManager(unsigned char key, char type);
void Renderer();

typedef enum{
    START_SIMPLE_GAME = 1,
    EXIT = 0
} MENU_ACTION;

struct MenuButton{
    int x0;
    int x1;
    int y0;
    int y1;
    int action;
    int x;
    int y;
    char const *text;
};

int isButtonHover(struct MenuButton button);
struct MenuButton createButtonRelativeToCenter(int x, int y, int w, int h, MENU_ACTION action, char const *text);
void handleClick(MENU_ACTION action);
#endif //MENU_H