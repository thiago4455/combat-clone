#ifndef MANAGER_H
#define MANAGER_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Utils.h"

void M_Init();
void M_Run();
void M_Cleanup();

static const unsigned int FPS = 60;
// static const unsigned int FULLSCREEN = 0;
static const unsigned int WINDOW_WIDTH = 900;
static const unsigned int WINDOW_HEIGHT = 900;

typedef enum{
    STATE_EXIT = 0,
    STATE_MENU = 1,
    STATE_GAME = 2,
    STATE_SCORE = 3
} ManagerGameState;

typedef enum{
    GAME_INPUT = 1,
    GAME_RENDERER = 2
} ManagerFunctionType;

void EventManager(ManagerFunctionType function_type, unsigned char event_data, char event_type);

#endif //MANAGER_H