#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

extern int board[BOARD_MAX][BOARD_MAX];        //ゲーム用?
extern int tempBoard[BOARD_MAX][BOARD_MAX];    //探索用配列