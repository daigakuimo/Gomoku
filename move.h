#pragma once

//評価値
#define MOVE_MY_WIN              30    //勝ち確
#define MOVE_PREVENT_ENEMY_WIN   25    //相手の勝ちを防ぐ
#define MOVE_MY_CHECKMATE        20    //自分が王手
#define MOVE_MY_FORBIDDEN_MOVE   0    //

void move();
int  maxLevel(int limit);
int  minLevel(int limit);
