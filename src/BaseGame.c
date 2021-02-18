#include "BaseGame.h"
#include <math.h>
#include <stdio.h>


extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;

struct Player Players[PLAYERS_NUMBER];
struct PLAYER_CONTROLS C_Player[PLAYERS_NUMBER];

struct Bullet bullets[MAX_BULLET_QUANTITY];


void G_EventHandler(MANAGER_FUNCION_TYPE function_type, unsigned char event_data, char event_type){
    switch (function_type)
    {
    case GAME_INPUT:
        G_ProcessInput(event_data, event_type);
        break;
    
    case GAME_RENDERER:
        if(G_Update() == 0){
            al_clear_to_color(al_map_hex(BACKGROUND_COLOR));
            G_Render();
            al_flip_display();
        }
        break;
    
    default:
        break;
    }
}

void G_Start(){
    CreatePlayer(90, WINDOW_HEIGHT/2, 0,
                (struct PLAYER_CONTROLS) {ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_SPACE},
                "assets/tank.png", 0x0000ff);

    CreatePlayer(WINDOW_WIDTH - 90, WINDOW_HEIGHT/2, ALLEGRO_PI,
                (struct PLAYER_CONTROLS) {ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_ENTER},
                "assets/tank2.png", 0xff0000);
}

void G_ProcessInput(unsigned char key, char type){
    int i;
    for (i = 0; i < PLAYERS_NUMBER; i++)
    {
        if(!type)
            PlayerInputDown(&Players[i], C_Player[i], key);
        else
            PlayerInputUp(&Players[i], C_Player[i], key);
    }
}

int G_Update(){
    int i;
    for (i = 0; i < PLAYERS_NUMBER; i++)
    {
        PlayerUpdate(&Players[i]);
        if(Players[i].Health <= 0){
            // return 1;
        }
    }
    UpdateBullets();

    return 0;
}

void G_Render(){
    int i;
    for (i = 0; i < PLAYERS_NUMBER; i++){
        // 
        // Draw health
        // 
        float max_size = al_get_bitmap_width(Players[i].TankBitmap) > al_get_bitmap_height(Players[i].TankBitmap)? al_get_bitmap_width(Players[i].TankBitmap) : al_get_bitmap_height(Players[i].TankBitmap);
        const float bar_height = 15;
        al_draw_rectangle(Players[i].PosX - max_size/2, Players[i].PosY - max_size/2, Players[i].PosX + max_size/2, Players[i].PosY - max_size/2 - bar_height, al_map_hex(Players[i].BulletColor), 1);
        al_draw_filled_rectangle(Players[i].PosX - max_size/2, Players[i].PosY - max_size/2, Players[i].PosX - max_size/2 + ((float) Players[i].Health/MAX_HEALTH)*max_size, Players[i].PosY - max_size/2 - bar_height, al_map_hex(Players[i].BulletColor));

        // 
        // Draw Power
        // 
        al_draw_filled_rectangle(Players[i].PosX - max_size/2, Players[i].PosY - max_size/2 + 5, Players[i].PosX - max_size/2 + ((float) Players[i].Power/MAX_POWER)*max_size, Players[i].PosY - max_size/2 + 3, al_map_hex(0x00ff00));


        // 
        // Draw Player
        // 
        al_draw_scaled_rotated_bitmap(Players[i].TankBitmap, al_get_bitmap_width(Players[i].TankBitmap) / 2, al_get_bitmap_height(Players[i].TankBitmap) / 2, 
                           Players[i].PosX, Players[i].PosY, .5f, .5f, Players[i].Rotation, 0);
    }

    // 
    // Draw Bullet
    // 
    for (i = 0; i < MAX_BULLET_QUANTITY; i++)
    {
        if(bullets[i].Active){
            float r = 3 + 30*bullets[i].Power/MAX_POWER;
            al_draw_filled_circle(bullets[i].PosX, bullets[i].PosY, r, al_map_hex(Players[bullets[i].Owner].BulletColor));
        }
    }           
}

// 
// Initialize player
// 
int initializedPlayers = 0;
void CreatePlayer(float initialPosX, float initialPosY, float initialRot, struct PLAYER_CONTROLS controller, const char *playerImage, unsigned int bulletColor){
    Players[initializedPlayers] = (struct Player) { 0, 0, initialPosX, initialPosY, initialRot, 0, 0, 0, bulletColor, MAX_HEALTH, al_load_bitmap(playerImage)};
    C_Player[initializedPlayers] = controller;
    initializedPlayers++;
}


// 
// Manage inputs
// 

void PlayerInputDown(struct Player *player, struct PLAYER_CONTROLS controller, unsigned char key){
    if (key == controller.up)
        player->TranslationV = 1;
    else if(key == controller.down)
        player->TranslationV = -1;
    else if(key == controller.left)
        player->RotationV = 1;
    else if(key == controller.right)
        player->RotationV = -1;
    else if(key == controller.fire)
        player->WillFire = 1;
}

void PlayerInputUp(struct Player *player, struct PLAYER_CONTROLS controller, unsigned char key){
    if ((key == controller.up && player->TranslationV == 1) || (key == controller.down && player->TranslationV == -1))
        player->TranslationV = 0;
    else if((key == controller.left && player->RotationV == 1) || (key == controller.right && player->RotationV == -1))
        player->RotationV = 0;
    else if(key == controller.fire)
        player->WillFire = 0;
}


// 
// Calculations before rendering
// 

void VerifyPlayerColisions(struct Player *player){
    int i = 0;

    for (i = 0; i < MAX_BULLET_QUANTITY; i++)
    {
        if(bullets[i].Active){
            float dx = abs(bullets[i].PosX - player->PosX);
            float dy = abs(bullets[i].PosY - player->PosY);
            float r = 3 + 30*bullets[i].Power/MAX_POWER;
            float max_size = .6 * (al_get_bitmap_width(player->TankBitmap) > al_get_bitmap_height(player->TankBitmap)? al_get_bitmap_width(player->TankBitmap) : al_get_bitmap_height(player->TankBitmap));

            if((dx < max_size/2 + r) && (dy < max_size/2 + r) && (player != &Players[bullets[i].Owner])){
                float minD = ((COOLDOWN * FPS)/MAX_POWER);
                player->Health -= MAX_HEALTH * (minD + ((1-minD) * bullets[i].Power/MAX_POWER));
                if(player->Health<0){
                    player->Health = 0;
                }
                bullets[i].Active = 0;
            }
        }
    }
    
}


void PlayerUpdate(struct Player *player){
    player->Rotation += -ALLEGRO_PI * 0.001 * player->RotationV * PLAYER_ROTATION_VELOCITY;
    player->PosX += player->TranslationV * PLAYER_VELOCITY * cos(player->Rotation);
    player->PosY += player->TranslationV * PLAYER_VELOCITY * sin(player->Rotation);

    if(player->Cooldown > 0)
        player->Cooldown--;
    else {
        if(player->WillFire)
            player->Power = player->Power < MAX_POWER? player->Power + 1 : player->Power;
        else
        {
            if(player->Power>0){
                int i;
                for (i = MAX_BULLET_QUANTITY - 1; i > 0; i--) {
                    bullets[i] = bullets[i-1];
                }
                bullets[0] = (struct Bullet) {1, player->Rotation, player->PosX, player->PosY, player->Power, player-Players};
                player->Power = 0;
                player->Cooldown = COOLDOWN * FPS;

            }
        }
        
    }

    VerifyPlayerColisions(player);
}

void UpdateBullets(){
    int i;
    for (i = 0; i < MAX_BULLET_QUANTITY; i++)
    {
        if(bullets[i].Active){
            bullets[i].PosX += BULLET_VELOCITY * cos(bullets[i].Direction);
            bullets[i].PosY += BULLET_VELOCITY * sin(bullets[i].Direction);
        }
    }
    
}