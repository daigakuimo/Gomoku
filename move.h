#pragma once

//評価値
#define MOVE_MY_WIN              15    //勝ち確
#define MOVE_PREVENT_ENEMY_WIN   14    //相手の勝ちを防ぐ
#define MOVE_MY_CHECKMATE        13    //自分が王手

void move();
int  maxLevel(int limit);
int  minLevel(int limit);
