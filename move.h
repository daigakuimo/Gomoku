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
#define DIRECTION                8     //評価の方向
#define TOTAL_DIRECTION          4     //
#define MAX_SEARCH               5     //評価時の石検索の数


void move(char *input);
int  minMax(int level, int player, int x, int y);
int  evaluation(int x, int y);
int  countBlack(unsigned char sta, int player, int putX, int putY, int direction);
int  getPatern(unsigned char sta);