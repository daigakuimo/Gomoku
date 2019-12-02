#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include <limits.h>

#define BOARD_MAX 15
#define EMPTY     0
#define ENEMY     1
#define AI        2

int board[BOARD_MAX][BOARD_MAX];
int tempBoard[BOARD_MAX][BOARD_MAX];