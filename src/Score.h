#ifndef SCORE_H
#define SCORE_H
#include "Manager.h"


void ScoreEventHandler(ManagerFunctionType function_type, unsigned char event_data, char event_type);
void Renderer();
void InitScore();

typedef struct{
    int score1;
    int score2;
    char *winner;
    unsigned int winnerColor;
    char show;
} Scores;


void showResult(Scores s);

#endif //SCORE_H'