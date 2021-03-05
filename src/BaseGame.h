#ifndef BASE_GAME_H
#define BASE_GAME_H

#include "Manager.h"

static const unsigned int BACKGROUND_COLOR = 0x769971;

#define PLAYERS_NUMBER 2
#define MAX_BULLET_QUANTITY 100

static const int MAX_HEALTH = 100;
static const float PLAYER_VELOCITY = 3;
static const float PLAYER_ROTATION_VELOCITY = 10;
static const float COOLDOWN = .1;
static const unsigned int MAX_POWER = 200;
static const int BULLET_VELOCITY = 20;

struct Player
{
    char TranslationV;
    char RotationV;
    float PosX;
    float PosY;
    float Rotation;
    char WillFire;
    unsigned int Power;
    unsigned int Cooldown;
    unsigned int BulletColor;
    int Health;

    ALLEGRO_BITMAP *TankBitmap;
};

struct Bullet
{
    int Active;
    float Direction;
    float PosX;
    float PosY;
    unsigned int Power;
    int Owner;
};

struct PLAYER_CONTROLS{
    unsigned char up;
    unsigned char down;
    unsigned char left;
    unsigned char right;
    unsigned char fire;
};

void G_EventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type);


/**
 * Inicializa as variaveis de jogo.
**/
void G_Start();


/**
 * Processa os eventos de input e define as variaveis de intenção do player, para serem utilizadas no próximo frame.
 * 
 * @param key Código da tecla ALLEGRO_KEY_*
 * @param type Tipo de Evento. 0 para KeyDown e 1 para KeyUp
**/
void G_ProcessInput(unsigned char key, char type);

/**
 * Realiza o calculo e atualiza o estado de todas as posições e ações dos players. 
**/
int G_Update();

/**
 * Renderiza os gráficos na tela
**/
void G_Render();

void CreatePlayer(float initialPosX, float initialPosY, float initialRot, struct PLAYER_CONTROLS controller, const char *playerImage, unsigned int bulletColor);
void PlayerInputDown(struct Player *player, struct PLAYER_CONTROLS controller, unsigned char key);
void PlayerInputUp(struct Player *player, struct PLAYER_CONTROLS controller, unsigned char key);
void PlayerUpdate(struct Player *player);
void UpdateBullets();
void ResetBullets();

#endif //BASE_GAME_H