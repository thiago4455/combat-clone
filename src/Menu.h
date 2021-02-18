#ifndef MENU_H
#define MENU_H
#include "Manager.h"

void MenuEventHandler(MANAGER_FUNCION_TYPE function_type, ALLEGRO_EVENT event, char event_type);
void InputManager(unsigned char key, char type);
void Renderer();

#endif //MENU_H