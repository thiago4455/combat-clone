#include "BaseGame.h"
#include <math.h>
#include <stdio.h>
#include "Client.h"
#include "Score.h"

extern ManagerGameState gameState;
int initializedPlayers = 0;
int initializedOnlinePlayers = 0;
int localPlayerIndex = -1;

ALLEGRO_SAMPLE *sample_hit;
ALLEGRO_SAMPLE *sample_shoot;

struct Player Players[MAX_PLAYERS_NUMBER];
struct PLAYER_CONTROLS C_Player[MAX_PLAYERS_NUMBER];
struct OnlinePlayer OnlinePlayers[MAX_PLAYERS_NUMBER];

struct Bullet bullets[MAX_BULLET_QUANTITY];

const struct Obstacle obstacles[] = {
                                      (struct Obstacle) {900/2, 900/2, 20, 150},
                                      (struct Obstacle) {900/2, 150, 250, 20},
                                      (struct Obstacle) {900/2, 900-150, 250, 20},
                                    };


const struct PlayerCustomization customizaitons[2] = {
    (struct PlayerCustomization) {90, 900/2, 0,0x0000ff, "assets/tank.png"},
    (struct PlayerCustomization) {900 - 90, 900/2, ALLEGRO_PI, 0xff0000, "assets/tank2.png"},
};

ALLEGRO_FONT *gfont;

void G_EventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type){
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
        }else{
            Scores scores;
            int i = 0;
            for (i = 0; i < initializedPlayers; i++)
            {
                if(Players[i].Health<=0){
                    char *winner = malloc(sizeof(char)*20);
                    if(i)
                        strcpy(winner, "Azul");
                    else
                        strcpy(winner, "Vermelho");
                    scores = (Scores) { Players[0].Health/MAX_HEALTH * 100, Players[1].Health/MAX_HEALTH * 100, winner, Players[i-1].BulletColor, 1};
                }
            }
            ResetBullets();
            InitScore();
            showResult(scores);
            gameState = STATE_SCORE;
        }
        break;
    
    default:
        break;
    }
}

void G_StartLocal(){
    sample_hit = al_load_sample("assets/cannon.wav");
    sample_shoot = al_load_sample("assets/hit.wav");
    gfont = al_load_font("assets/arial.ttf", 22, 0);
    initializedPlayers = 0;
    CreatePlayer(90, WINDOW_HEIGHT/2, 0,
                (struct PLAYER_CONTROLS) {ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_SPACE},
                "assets/tank.png", 0x0000ff);

    CreatePlayer(WINDOW_WIDTH - 90, WINDOW_HEIGHT/2, ALLEGRO_PI,
                (struct PLAYER_CONTROLS) {ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_ENTER},
                "assets/tank2.png", 0xff0000);
}

void G_StartOnline(){
    sample_hit = al_load_sample("assets/hit.wav");
    sample_shoot = al_load_sample("assets/cannon.wav");
    gfont = al_load_font("assets/arial.ttf", 22, 0);
    initializedPlayers = 0;
    reset_connection();
}

void G_ProcessInput(unsigned char key, char type){
    int i;
    if(initializedPlayers==MAX_PLAYERS_NUMBER)
        for (i = 0; i < initializedPlayers; i++)
        {
            if(!type)
                PlayerInputDown(&Players[i], C_Player[i], key);
            else
                PlayerInputUp(&Players[i], C_Player[i], key);
        }
}

int G_Update(){
    int i;
    for (i = 0; i < initializedPlayers; i++)
    {
        PlayerUpdate(&Players[i]);
        VerifyBulletColisions();
        if(Players[i].Health <= 0){
            return 1;
        }
    }
    UpdateBullets();
    if(localPlayerIndex > -1)
        send_input_data((struct InputPackage) {
            Players[localPlayerIndex].TranslationV,
            Players[localPlayerIndex].RotationV,
            Players[localPlayerIndex].WillFire,
            Players[localPlayerIndex].PosX,
            Players[localPlayerIndex].PosY,
            Players[localPlayerIndex].Rotation
        });

    return 0;
}

void G_Render(){
    int i;
    for (i = 0; i < initializedPlayers; i++){
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

    // 
    // Draw obstacles
    // 
    for (i = 0; i < sizeof(obstacles)/sizeof(struct Obstacle); i++)
    {
        al_draw_rectangle(obstacles[i].x - obstacles[i].width/2, obstacles[i].y - obstacles[i].height/2, obstacles[i].x + obstacles[i].width/2, obstacles[i].y + obstacles[i].height/2, al_map_hex(0x523A28), 1);
        al_draw_filled_rectangle(obstacles[i].x - obstacles[i].width/2, obstacles[i].y - obstacles[i].height/2, obstacles[i].x + obstacles[i].width/2, obstacles[i].y + obstacles[i].height/2, al_map_hex(0x964B00));
    }
    
    if(initializedPlayers<MAX_PLAYERS_NUMBER)
        al_draw_text(gfont, al_map_hex(0xffffff), 450, 850, ALLEGRO_ALIGN_CENTRE, "Aguarde todos os jogadores se conectarem");
}

// 
// Initialize player
// 
void CreatePlayer(float initialPosX, float initialPosY, float initialRot, struct PLAYER_CONTROLS controller, const char *playerImage, unsigned int bulletColor){
    Players[initializedPlayers] = (struct Player) { 0, 0, initialPosX, initialPosY, initialRot, 0, 0, 0, bulletColor, MAX_HEALTH, al_load_bitmap(playerImage)};
    C_Player[initializedPlayers] = controller;
    initializedPlayers++;
}

int getOnlinePlayer(unsigned char player_id);

struct PlayerCustomization getOnlinePlayerCustomization(unsigned char instance_id){
    int i;
    for (i = 0; i < initializedOnlinePlayers; i++)
    {
        if(instance_id==OnlinePlayers[i].instance_index){
            return customizaitons[OnlinePlayers[i].player_index];
        }
    }
    return customizaitons[0];
}

void MultiplayerInput(int i, struct InputPackage input){

        // Trying to set player postion on start
        // if(Players[i].TranslationV == 0){
        //     struct PlayerCustomization c = getOnlinePlayerCustomization(i);
        //     if(c.initialPosX == Players[i].PosX &&
        //        c.initialPosY == Players[i].PosY &&
        //        (c.initialRotation - 1 < Players[i].Rotation) || (c.initialRotation + 1 > Players[i].Rotation)){
        //         Players[i].PosX = input.PosX;
        //         Players[i].PosY = input.PosY;
        //         Players[i].Rotation = input.Rotation;
        //     }
        // }

        Players[i].TranslationV = input.TranslationV;
        Players[i].RotationV = input.RotationV;
        Players[i].WillFire = input.WillFire;
        
        // Set position and rotation causes player to lag
        // if(input.PosX > Players[i].PosX + 2 || input.PosX < Players[i].PosX - 2)
        //     Players[i].PosX = input.PosX;
        // if(input.PosY > Players[i].PosY + 2 || input.PosY < Players[i].PosY - 2)
        //     Players[i].PosY = input.PosY;
        // if(input.Rotation > Players[i].Rotation + .2 || input.Rotation < Players[i].Rotation - .2)
        //     Players[i].Rotation = input.Rotation;
}

void HandleMultiplayerInput(unsigned char player_id, struct InputPackage input){ 
    int currentPlayer = getOnlinePlayer(player_id);
    if(input.TranslationV){
        printf("received from: %d\n",player_id);
    }
    if(currentPlayer == -1){
        // send_init_package();
    }
    else {
        MultiplayerInput(currentPlayer, input);
    }
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

void VerifyBulletColisions(){
    int i = 0;

    for (i = 0; i < MAX_BULLET_QUANTITY; i++)
    {
        if(bullets[i].Active){

            // Collision with players
            int j;
            for (j = 0; j < initializedPlayers; j++)
            {
                float dx = abs(bullets[i].PosX - Players[j].PosX);
                float dy = abs(bullets[i].PosY - Players[j].PosY);
                float r = 3 + 30*bullets[i].Power/MAX_POWER;
                float max_size = .6 * (al_get_bitmap_width(Players[j].TankBitmap) > al_get_bitmap_height(Players[j].TankBitmap)? al_get_bitmap_width(Players[j].TankBitmap) : al_get_bitmap_height(Players[j].TankBitmap));

                if((dx < max_size/2 + r) && (dy < max_size/2 + r) && (j != bullets[i].Owner)){
                    float minD = ((COOLDOWN * FPS)/MAX_POWER);
                    Players[j].Health -= MAX_HEALTH * (minD + ((1-minD) * bullets[i].Power/MAX_POWER));
                    if(Players[j].Health<0){
                        Players[j].Health = 0;
                    }
                    bullets[i].Active = 0;
                    al_play_sample(sample_hit, .3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                }
            }

            j = 0;
            for (j = 0; j < sizeof(obstacles)/sizeof(struct Obstacle); j++)
            {
                float r = 3 + 30*bullets[i].Power/MAX_POWER;
                if((bullets[i].PosX+r > obstacles[j].x-obstacles[j].width/2 && bullets[i].PosX-r < obstacles[j].x+obstacles[j].width/2) && (bullets[i].PosY+r > obstacles[j].y-obstacles[j].height/2 && bullets[i].PosY-r < obstacles[j].y+obstacles[j].height/2)){
                    bullets[i].Active = 0;
                    // al_play_sample(sample_hit, .3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                    break;
                }
            }
        }
    }
    
}

int VerifyMovementColisions(float x, float y, struct Player *player){
    float will_collide = 0;
    float max_size = .25 * (al_get_bitmap_width(player->TankBitmap) > al_get_bitmap_height(player->TankBitmap)? al_get_bitmap_width(player->TankBitmap) : al_get_bitmap_height(player->TankBitmap));
    
    will_collide = will_collide || (x-max_size < 0 || y-max_size < 0 || x+max_size > WINDOW_WIDTH || y+max_size > WINDOW_HEIGHT);
    
    int i = 0;
    for (i = 0; i < sizeof(obstacles)/sizeof(struct Obstacle); i++)
    {
        will_collide = will_collide || ((x+max_size > obstacles[i].x-obstacles[i].width/2 && x-max_size < obstacles[i].x+obstacles[i].width/2) && (y+max_size > obstacles[i].y-obstacles[i].height/2 && y-max_size < obstacles[i].y+obstacles[i].height/2));
    }

    for (i = 0; i < initializedPlayers; i++)
    {
        if(&Players[i] != player){
            float _max_size = .25 * (al_get_bitmap_width(Players[i].TankBitmap) > al_get_bitmap_height(Players[i].TankBitmap)? al_get_bitmap_width(Players[i].TankBitmap) : al_get_bitmap_height(Players[i].TankBitmap));
            will_collide = will_collide || ((x+max_size > Players[i].PosX-_max_size && x-max_size < Players[i].PosX+_max_size) && (y+max_size > Players[i].PosY-_max_size && y-max_size < Players[i].PosY+_max_size));
        }
    }
    
    
    return !will_collide;
}


void PlayerUpdate(struct Player *player){
    player->Rotation += -ALLEGRO_PI * 0.001 * player->RotationV * PLAYER_ROTATION_VELOCITY;
    float nX = player->PosX + player->TranslationV * PLAYER_VELOCITY * cos(player->Rotation);
    float nY = player->PosY + player->TranslationV * PLAYER_VELOCITY * sin(player->Rotation);
    if(VerifyMovementColisions(nX, nY, player)){
        player->PosX = nX;
        player->PosY = nY;
    }

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
                al_play_sample(sample_shoot, .3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            }
        }
        
    }

    VerifyBulletColisions(player);
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

void ResetBullets(){
    int i;
    for (i = 0; i < MAX_BULLET_QUANTITY; i++)
    {
        bullets[i].Active = 0;
    }
}

int getOnlinePlayer(unsigned char player_id)
{
    int i;
    for (i = 0; i < initializedOnlinePlayers; i++)
    {
        if(player_id==OnlinePlayers[i].player_id){
            return OnlinePlayers[i].instance_index;
        }
    }
    return -1;
}

void InitOnlinePlayer(InitPlayerPackage player_package, unsigned char local_id){
    int currentPlayer = getOnlinePlayer(player_package.player_id);
    if(currentPlayer == -1){
        struct OnlinePlayer player = (struct OnlinePlayer) {player_package.player_id, player_package.player_index, initializedPlayers};
        OnlinePlayers[initializedOnlinePlayers++] = player;
        if(player.player_id == local_id){
            localPlayerIndex = player.instance_index;
            CreatePlayer(customizaitons[player.player_index].initialPosX, customizaitons[player.player_index].initialPosY, customizaitons[player.player_index].initialRotation, (struct PLAYER_CONTROLS) {ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_SPACE}, customizaitons[player.player_index].sprite, customizaitons[player.player_index].color);
        }
        else{
            CreatePlayer(customizaitons[player.player_index].initialPosX, customizaitons[player.player_index].initialPosY, customizaitons[player.player_index].initialRotation, (struct PLAYER_CONTROLS) {0, 0, 0, 0, 0}, customizaitons[player.player_index].sprite, customizaitons[player.player_index].color);
        }
        printf("np:%d\n", player_package.player_id);
    }
}