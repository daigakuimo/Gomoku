#pragma once

//評価値
#define MOVE_WIN                 100   //勝ち確
#define MOVE_PREVENT_ENEMY_WIN   80    //相手の勝ちを防ぐ
#define MOVE_PREVENT_ENEMY_THREE 55    //相手の三を防ぐ   
#define MOVE_FOUR                50    //四
#define MOVE_THREE               40    //三
#define MOVE_FOUR_FOUR           75    //四四
#define MOVE_FOUR_THREE          70    //四三
#define MOVE_THREE_THREE         65    //三三
#define MOVE_MY_FORBIDDEN_MOVE   0     //禁じ手

#define SEARCH_DEPTH             3     //探索の深さ
#define DIRECTION                8     //評価時の検索8方向
#define TOTAL_DIRECTION          4     //評価時の4方向
#define MAX_SEARCH_STONE_NUM     6     //評価時の石検索の数

//評価時の石の状態のパターン
#define STATE_A                  1
#define STATE_X                  2
#define STATE_B                  3
#define STATE_C                  4
#define STATE_D                  5
#define STATE_E                  6 
#define STATE_F                  7
#define STATE_O                  8
#define STATE_V                  9


void move(char *input);
int  minMax(int level, int player, int putX, int putY);
int  evaluation(int putX, int putY, int player);
int  countStone(unsigned int sta[], int player, int putX, int putY, int direction);
int  getPatern(unsigned int sta[], int direction);