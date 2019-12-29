#include "parameterGA.h"
#include "../parameter.h"
#include "../move.h"
#include "../forbiddenMove.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;
extern int turnCount;
extern int winFlag;
extern int evaluationPaternGenetic[GENETIC_NUM][EVALUATION_PATERN_NUM];

extern int GAflag;
extern int genetic;


/* ====================================================================== */
/**
 * @brief  遺伝子を初期化
 */
/* ====================================================================== */
void init()
{
    //遺伝子初期化
    int tempGenetic[GENETIC_NUM][EVALUATION_PATERN_NUM] = {
        {90 ,50 ,40 ,53 ,39 ,75 ,70 ,65 ,5,55,53,50,49,10,9,7,6},
        {29 ,32 ,39 ,49 ,62 ,78 ,98 ,21 ,47,76,9,45,84,26,72,21,73},
        {2  ,5  ,12 ,22 ,35 ,51 ,71 ,94 ,20,49,82,18,57,99,45,94,46},
        {77 ,80 ,87 ,97 ,10 ,26 ,46 ,69 ,95,24,57,93,32,74,20,69,21},
        {16 ,20 ,26 ,36 ,49 ,65 ,85 ,8  ,34,63,96,32,71,14,59,8,61},
        {49 ,52 ,59 ,69 ,82 ,98 ,18 ,40 ,67,96,29,65,4,46,92,41,93},
        {82 ,85 ,91 ,1  ,14 ,31 ,50 ,73 ,99,29,61,97,36,79,25,74,26},
        {21 ,24 ,31 ,40 ,54 ,70 ,89 ,12 ,38,68,19,36,76,18,64,13,65},
        {63 ,67 ,73 ,83 ,96 ,12 ,32 ,55 ,81,10,43,79,18,61,6,55,7},
        {32 ,35 ,42 ,52 ,65 ,81 ,16 ,23 ,49,79,12,47,87,29,75,24,76},
        {33 ,36 ,43 ,53 ,66 ,82 ,2  ,25 ,51,80,13,49,88,30,76,25,77},
    };

    int i,j;
    int count = 0;
    for(i = 0; i < GENETIC_NUM; i++)
    {
        for(j = 0; j < EVALUATION_PATERN_NUM; j++)
        {
            if(i == 0)
            {
                evaluationPaternGenetic[i][j] = tempGenetic[i][j];
                continue;
            }
            srand((unsigned int)time(NULL) + count); 
            evaluationPaternGenetic[i][j] = rand() % 99 + 1;
            count++;
        }
    }

    GAflag = 1;
    
}



/* ====================================================================== */
/**
 * @brief  遺伝子の評価
 * 
 * @param[in, out] fitness　トーナメント形式で遺伝子に評価値を付ける
 * 
 */
/* ====================================================================== */
void geneticEvaluation(int fitness[])
{
    int s,k;
    for(s = 0; s < GENETIC_NUM; s++)
    {
        for(k = 0; k < GENETIC_NUM; k++)
        {
            if(s == k){
                continue;
            }

            int winner;

            //printf("%d vs %d\n",s,k);
            winner = battle(s,k);

            //printf("\n\n");

            if(winner == s)
            {
                fitness[s] += 3;
            }
            else if(winner == k)
            {
                fitness[k] += 5;
            }
            else if(winner = -1)
            {
                fitness[s] += 1;
                fitness[k] += 1;
            }
        }
    }    
}


/* ====================================================================== */
/**
 * @brief  バイナリートーナメントで交叉する遺伝子を決まる
 * 
 * @param[in] fitness  遺伝子の評価値
 * 
 */
/* ====================================================================== */
void geneticSelect(int fitness[], int remainGenetic[])
{
    int alreadyUseNum[GENETIC_NUM]  = {1,0,0,0,0,0,0,0,0,0,0}; 
    int i;
    int r , n;
    for(i = 0; i < REMAIN_GENETIC_NUM - 1; i++)
    {
        int count = 0;

        // ランダムでバイナリートーナメントを行う
        while(1)
        {
            srand((unsigned int)time(NULL) + count); 
            r = rand() % 10 + 1;

            if(alreadyUseNum[r] == 0)
            {
                alreadyUseNum[r] = 1;
                break;
            }

            count++;
        }
        
        while(1)
        {
            srand((unsigned int)time(NULL) + count); 
            n = rand() % 10 + 1;

            if(alreadyUseNum[n] == 0)
            {
                alreadyUseNum[n] = 1;
                break;
            }
            count++;
        }

        if(fitness[r] > fitness[n])
        {
            remainGenetic[i] = r;
        }
        else
        {
            remainGenetic[i] = n;
        }

    }

}


/* ====================================================================== */
/**
 * @brief  交叉を行い新しい遺伝子を作る
 */
/* ====================================================================== */
void crossover(int remainGenetic[])
{
    int loserGenetic[GENETIC_NUM - REMAIN_GENETIC_NUM] = {0};

    int i,j;
    int index = 0; 
    int flag  = 1;

    for(i = 0; i < GENETIC_NUM; i++)
    {
        flag = 1;
        for(j = 0; j < REMAIN_GENETIC_NUM; j++)
        {
            if(remainGenetic[j] == i){
                flag = 0;
                break;
            }
        }

        if(flag)
        {
            loserGenetic[index] = i;
            index++;
        }
    }

    int remainIndex = 0;
    srand((unsigned int)time(NULL));
    int t = rand() % 3;
    for(i = 0; i < GENETIC_NUM - REMAIN_GENETIC_NUM; i++)
    {
        if(i < 3)
        {
            if(i == t)
            {
                for(j = 0; j < EVALUATION_PATERN_NUM; j++)
                {
                    if(j < 4 || j > 12)
                    {
                        evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[remainIndex + 1]][j];
                    }
                    else
                    {
                        evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[remainIndex]][j];
                    }
                }
            }
            else
            {
                for(j = 0; j < EVALUATION_PATERN_NUM; j++)
                {
                    if(j < 4 || j > 12)
                    {
                        evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[remainIndex]][j];
                    }
                    else
                    {
                        evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[remainIndex + 1]][j];
                    }
                }
            }
            
            remainIndex += 2;
        }
        else
        {
            int r,n;
            srand((unsigned int)time(NULL)); 
            r = rand() % 5; 

            n = r;

            int turn = 0;
            int selectFlag = 1;
            while(selectFlag)
            {
                srand((unsigned int)time(NULL) + turn); 
                n = rand() % 5;

                int c;
                for(c = 0; c < REMAIN_GENETIC_NUM; c++)
                {
                    if(r == remainGenetic[c])
                    {
                        if(c % 2 == 0)
                        {
                            if(n != remainGenetic[c + 1])
                            {
                                selectFlag = 0;
                                break;
                            }
                        }
                        else
                        {
                            if(n != remainGenetic[c - 1])
                            {
                                selectFlag = 0;
                                break;
                            }
                        }
                    }
                }
                turn++;
            }

            
            
            for(j = 0; j < EVALUATION_PATERN_NUM; j++)
            {
                if(j < 4 || j > 12)
                {
                    evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[r]][j];
                }
                else
                {
                    evaluationPaternGenetic[loserGenetic[i]][j] = evaluationPaternGenetic[remainGenetic[n]][j];
                }
            }
        }

    }

}


/* ====================================================================== */
/**
 * @brief  遺伝子に突然変異を起こす 
 */
/* ====================================================================== */
void mutation()
{
    int mutationGenetic;
    int mutationIndex;
    int mutationValue;

    srand((unsigned int)time(NULL)); 
    mutationGenetic = rand() % (GENETIC_NUM - 1) + 1;

    srand((unsigned int)time(NULL) + 1); 
    mutationIndex = rand() % EVALUATION_PATERN_NUM;

    srand((unsigned int)time(NULL) + 2); 
    mutationValue = rand() % 99 + 1;

    evaluationPaternGenetic[mutationGenetic][mutationIndex] = mutationValue;

}


/* ====================================================================== */
/**
 * @brief  遺伝子同士で五目並べ戦う
 *
 * @param[in] s   先攻の遺伝子
 * @param[in] k   後攻の遺伝子
 *
 * @return  勝った遺伝子
 */
/* ====================================================================== */
int  battle(int s, int k)
{
    int winner = -1;

    //配列初期化
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
		}
	}

    turnCount = 0;
    int controller = 0;
    winFlag = 0;
    while(turnCount <= BOARD_MAX * BOARD_MAX)
    {
        // printf("--------------------------\n");
        // printf("genetic:%d turn\n",s);
        char input[256];
        if(turnCount == 0)
        {
            sprintf(input,"%d,%d", 8, 8);
        }
        else
        {
            genetic = s;
            move(input,AI);
        }

        if(winFlag)
		{
            // printf("%d vs %d\n",s,k);
			// printf("genetic:%d win\n",s);
            winner = s;
			break;
		}

        insertBoard(input,AI);

		if(controller == ENEMY_FORBIDDEN)
		{
            // printf("%d vs %d\n",s,k);
            // printf("genetic:%d forbidden\n",s);
			// printf("genetic:%d win\n",k);
            winner = k;
			break;
		}

        //display(1);

		turnCount++;

        // printf("--------------------------\n");
        // printf("genetic:%d turn\n",k);

        char buffer[256];
        genetic = k;
        move(buffer,ENEMY);

        if(winFlag)
		{
            // printf("%d vs %d\n",s,k);
			// printf("genetic:%d win\n",k);
            winner = k;
			break;
		}
        
        insertBoard(buffer,ENEMY);

        if(controller == ENEMY_FORBIDDEN)
		{
            // printf("genetic:%d forbidden\n",k);
			// printf("genetic:%d win\n",s);
            winner = s;
			break;
		}

        //display(1);

        turnCount++;


    }

    return winner;
    
}