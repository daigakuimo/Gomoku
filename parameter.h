#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include <limits.h>

#define BOARD_MAX       15              // 盤の目の数
#define EMPTY           0               // 空
#define ENEMY           1               // 敵の石
#define AI              2               // 自分の石

#define END             100
#define CONTINUE        110
#define ENEMY_FORBIDDEN 120

int board[BOARD_MAX][BOARD_MAX];        // ゲーム用
int tempBoard[BOARD_MAX][BOARD_MAX];    // 探索用盤面

int battingFirstPlayer;                 // 先攻のプレイヤー
int turnCount;
int winFlag;

void display(int boardFlag);
void checkBattingFirst();
int  insertBoard(char *data, int player);
int  judgeFinish(char *data);
