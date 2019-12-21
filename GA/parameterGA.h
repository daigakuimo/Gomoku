#pragma once

#define EVALUATION_PATERN_NUM 17
#define GENETIC_NUM           11
#define TERMINATION           10
#define REMAIN_GENETIC_NUM       5

int evaluationPaternGenetic[GENETIC_NUM][EVALUATION_PATERN_NUM];

int GAflag;

int genetic;

void init();
void geneticEvaluation(int fitness[]);
void geneticSelect(int fitness[], int remainGenetic[]);
void crossover(int remainGenetic[]);
void mutation();

int  battle(int s, int k);