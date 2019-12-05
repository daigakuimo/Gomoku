#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"


extern int board[BOARD_MAX][BOARD_MAX];        //�Q�[���p?
extern int tempBoard[BOARD_MAX][BOARD_MAX];    //�T���p�z��

void display();

int main()
{
    	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			if((i==6&&j==11)||(i==8&&j==9)||(i==10&&j==7)||(i==9&&j==8)||(i==10&&j==8)||(i==11&&j==8)||(i==8&&j==8)|(i==8&&j==7)||(i==7&&j==6)||(i==6&&j==5))
			{
			    board[i][j]     = 1;
			    tempBoard[i][j] = 1;
			}else if((i==5&&j==12)||(i==11&&j==6)||(i==7&&j==8)||(i==12&&j==8)||(i==10&&j==9)||(i==10&&j==9)||(i==7&&j==10))
			{
			    board[i][j]     = 2;
			    tempBoard[i][j] = 2;
			}
			else
			{
			    board[i][j]     = 0;
			    tempBoard[i][j] = 0;
			}
		}
	}
	display();
	if(Four(1,8,9) == 1)
	{
	    printf("��������\n");
	}else
	{
	    printf("���s����\n");
	}

}
void display()
{
	int i,j = 0;
	for(i = 0; i < BOARD_MAX; i++)
	{
		for(j = 0; j<BOARD_MAX; j++)
		{
			if(board[j][i] == 0) printf("- ",board[j][i]);
			if(board[j][i] == 1) printf("o ",board[j][i]);
			if(board[j][i] == 2) printf("x ",board[j][i]);
		}
		printf("\n");	
	}
	printf("\n\n");
}