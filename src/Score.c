#include "Score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ManagerGameState gameState;

void S_Render();

void ScoreEventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type){
    switch (function_type)
    {
    case GAME_INPUT:
        if(event_data == ALLEGRO_KEY_ESCAPE){
            gameState = STATE_MENU;
        }
        break;
    
    case GAME_RENDERER:
        S_Render();
        break;
    
    default:
        break;
    }
}

ALLEGRO_FONT *tfont;
ALLEGRO_FONT *sfont;

#define SCORE_NUMBER 5

char score[SCORE_NUMBER][15];
Scores current_score;

void readScore(){
    FILE *fp = fopen("score.txt", "r");
    if(fp){
        int i = 0;
        char buff[15];
        while(fgets(score[i], 15, fp)){
            if(i>=SCORE_NUMBER){
                score[i][strcspn(score[i], "\n")-1] = '\0';
                break;
            }
            i++;
        }
        fclose(fp);
    }
}

void addToScore(int score1, int score2){
    FILE *fp = fopen("score.txt", "w");
    memmove(&score[1], &score[0], (15*(SCORE_NUMBER-1))*sizeof(char));
    char bff[15];
    snprintf(score[0], 15, "%d\% vs %d\%\n", score1, score2);
    int i = 0;
    for (i = 0; i < SCORE_NUMBER; i++)
    {
        fprintf(fp, "%s", score[i]);
    }
    
    fclose(fp);
}


void InitScore(){
    tfont = al_load_font("assets/Ranchers-Regular.ttf", 30, 0);
    sfont = al_load_font("assets/arial.ttf", 22, 0);
    readScore();
    current_score.show = 0;
}

void showResult(Scores s){
    addToScore(s.score1, s.score2);
    readScore();
    current_score = s;
}


void S_Render(){
    al_clear_to_color(al_map_hex(0x446030));
    al_draw_text(tfont, al_map_hex(0xffffff), 450, 100, ALLEGRO_ALIGN_CENTRE, "Pontuação");
    if(current_score.show){
        al_draw_text(tfont, al_map_hex(current_score.winnerColor), 450, 200, ALLEGRO_ALIGN_CENTRE, "Venceu!!");
    } else {
        int i = 0;
        for (i = 0; i < SCORE_NUMBER; i++)
        {
            char bff[18];
            snprintf(bff, 18, "%dº - %s", i+1, score[i]);
            al_draw_text(sfont, al_map_hex(0xffffff), 450, 200 + i*50, ALLEGRO_ALIGN_CENTRE, bff);
        }
    }

    al_draw_text(sfont, al_map_hex(0xffffff), 450, 850, ALLEGRO_ALIGN_CENTRE, "Pressione ESC para voltar para o menu principal.");
    al_flip_display();
}