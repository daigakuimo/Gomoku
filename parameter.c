#include "parameter.h"
#include "move.h"
#include "forbiddenMove.h"

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
					int h = j%10 + 1;
					if(h == 10) h = 0;
                    printf("%d ",h);
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
                    int h = i%10 + 1;
					if(h == 10) h = 0;
                    printf("%d ",h);
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

/* ====================================================================== */
/**
 * @brief  打たれた手をboardに格納
 *
 * @param[in] data   打たれた手
 * @param[in] player どちらのプレイヤーが打ったか
 *
 * @return  ゲームの終了判定結果(整数)
 */
/* ====================================================================== */
int insertBoard(char *data, int player)
{
	int x = 99;
	int y = 99;
	char *token;

	//"end"が入力されたら終了
	if(strcmp(data,"end") == 0){
		return END;
	}
		
	token = strtok(data,",");	
	x = atoi(token);
		
	while(token != NULL)
	{
		token = strtok(NULL,",");	
		y = atoi(token);
		break;
	}
	
	if(x != 99)
	{
		board[y-1][x-1] = player;
		tempBoard[y-1][x-1] = player;
	}

	if(battingFirstPlayer == player)
	{
		if(evaluation(x-1,y-1,player,0) == MOVE_FORBIDDEN)
		{
			return ENEMY_FORBIDDEN;
		}
	}
	

	return CONTINUE;
}

int  judgeFinish(char *data)
{
	char *token;
		
	token = strtok(data,",");	
		
	while(token != NULL)
	{
		token = strtok(NULL,",");	
        printf("%s\n",token);
		if(strcmp(token,"win") == 0)
        {
            return END;
        }
	}
	
	return CONTINUE;
}

