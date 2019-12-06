#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"


extern int board[BOARD_MAX][BOARD_MAX];        //ƒQ[ƒ€—p?
extern int tempBoard[BOARD_MAX][BOARD_MAX];    //’Tõ—p”z—ñ

void display();

int main()
{
    	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			if((i==7&&j==10)||(i==8&&j==9)||(i==9&&j==8)||(i==10&&j==7)||(i==8&&j==7)||(i==8&&j==6)||(i==8&&j==5))
			{
			    board[j][i]     = 1;
			    tempBoard[j][i] = 1;
			}else if((i==8&&j==4)||(i==8&&j==10)||(i==6&&j==11))
			{

			    board[j][i]     = 2;
			    tempBoard[j][i] = 2;
			}
			else
			{
			    board[j][i]     = 0;
			    tempBoard[j][i] = 0;
			}
		}
	}
	display();
	if(forbidden(1,8,9) == 1)
	{
	    printf("¬Œ÷‚¾‚æ\n");
	}else
	{
	    printf("¸”s‚¾‚æ\n");
	}

}
void display()

{
	int i,j = 0;
	for(i = 0; i < BOARD_MAX; i++)
	{
		for(j = 0; j<BOARD_MAX; j++)
		{
			if(board[i][j] == 0) printf("- ",board[i][j]);
			if(board[i][j] == 1) printf("o ",board[i][j]);
			if(board[i][j] == 2) printf("x ",board[i][j]);
		}
		printf("\n");	
	}
	printf("\n\n");
}