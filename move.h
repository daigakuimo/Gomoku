#pragma once

//評価値
#define MOVE_MY_WIN              30    //勝ち確
#define MOVE_PREVENT_ENEMY_WIN   25    //相手の勝ちを防ぐ
#define MOVE_MY_CHECKMATE        20    //王手
#define MOVE_MY_FORBIDDEN_MOVE   0     //禁じ手

#define SEARCH_DEPTH             3     //探索の深さ


void move(char *input);
int  minMax(int level, int player, int x, int y);
int  evaluation(int x, int y);