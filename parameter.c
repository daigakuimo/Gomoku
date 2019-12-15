#include "parameter.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;


/* ====================================================================== */
/**
 * @brief  盤面を表示
 */
/* ====================================================================== */
void display(int boardFlag)
{
	int i,j = 0;
	for(i = -1; i < BOARD_MAX; i++)
	{
		for(j = -1; j<BOARD_MAX; j++)
		{
            if(i == -1)
            {
                if(j != -1)
                {
                    printf("%d ",j%10);
                } 
                else
                {
                     printf("  ");
                }
                

            }
            else
            {
                if(j == -1)
                {
                    printf("%d ",i%10);
                }
                else
                {
                    if(boardFlag == 1)
                    {
                        if(board[i][j] == 0) printf("- ",board[i][j]);
                        if(board[i][j] == 1) printf("o ",board[i][j]);
                        if(board[i][j] == 2) printf("x ",board[i][j]);
                    }
                    else
                    {
                        if(tempBoard[i][j] == 0) printf("- ");
                        if(tempBoard[i][j] == 1) printf("o ");
                        if(tempBoard[i][j] == 2) printf("x ");
                    }
                }
            }
            
		}
		printf("\n");	
	}
	printf("\n\n");
}

void checkBattingFirst()
{
	int errorFlag = 1;

	while(errorFlag == 1)
	{
		char checkTurn[256];
		printf("black or white\n");
		scanf("%s",checkTurn);

		if(strcmp(checkTurn,"black") == 0)
		{
			battingFirstPlayer = AI;
			errorFlag = 0;
		}
		else if(strcmp(checkTurn,"white") == 0)
		{
			battingFirstPlayer = ENEMY;
			errorFlag = 0;
		}

		if(errorFlag == 1)
		{
			printf("one more input\n");
		}
	}
	
}
