//http://blog.livedoor.jp/akf0/archives/51585502.html
#include "parameter.h"
#include "move.h"
#include "forbiddenMove.h"
#include <time.h>

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;
extern int turnCount;
extern int winFlag;


int main(int argc, char *argv[]) {

	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
		}
	}

	battingFirstPlayer = AI;

	board[7][7]     = 1;
	tempBoard[7][7] = 1;

	board[6][8]     = 1;
	tempBoard[6][8] = 1;

	board[5][9]     = 1;
	tempBoard[5][9] = 1;

	board[6][6]     = 2;
	tempBoard[6][6] = 2;

	board[5][5]     = 2;
	tempBoard[5][5] = 2;

	display(1);

	turnCount = 4;

	char input[256];
	move(input,AI);

	insertBoard(input,AI);
	display(1);
	
	return 0;

}


	
