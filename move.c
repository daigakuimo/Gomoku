#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;

/* ====================================================================== */
/**
 * @brief  ミニマックス法で打つ手を決める
 * 
 * @param[in,out] input 打つ手を返す
 * 
 */
/* ====================================================================== */
void move(char *input)
{
    int maxScore = INT_MIN;
    int minScore    = INT_MAX;
    int score;
    int bestX = 0, bestY = 0;


    int y,x;
    for(y = 5; y < BOARD_MAX-6; y++)
    {
        for(x = 5; x < BOARD_MAX-6; x++)
        {
            //石が置いてある場所は探索しない
            if(board[y][x] != EMPTY) continue;

            //仮で石を打つ
            tempBoard[y][x] = AI;

            //ミニマックス法で評価値を得る
            score = minMax(SEARCH_DEPTH, ENEMY, x, y, minScore);

            printf("%d,%d\n", x, y);
            printf("move-score:%d\n",score);
            Sleep(1000);

            //評価値が一番高い手を選ぶ
            if(score > maxScore)
            {
                maxScore = score;
                bestX = x;
                bestY = y;
                printf("maxScore:%d\n",maxScore);
            }

            //仮で打った石を消す
            tempBoard[y][x] = EMPTY;
            
        }
    }

    //ミニマックス法で選ばれた手を返す
    sprintf(input,"%d,%d", bestX, bestY);
    printf("%s\n",input);
    
}


/* ====================================================================== */
/**
 * @brief  ミニマックス法で手を評価する
 *
 * @param[in] level  残りの探索の深さ
 * @param[in] player どちらのプレイヤーの評価をするか
 * @param[in] x,y    評価したい手の座標
 *
 * @return  評価値(整数)
 */
/* ====================================================================== */
int  minMax(int level, int player, int putX, int putY, int beforeBranchScore)
{
    int maxScore    = INT_MIN;
    int minScore    = INT_MAX;
    int returnScore = 0;
    int score;
    int nextPlayer;

    if(player == AI)
    {
        nextPlayer = ENEMY;
    }
    else
    {
        nextPlayer = AI;
    }

    //printf("%d\n",level);
    //printf("beforeBranchScore : %d\n", beforeBranchScore);

    //禁じ手ならこれ以上探索せずに評価値を返す

    //探索する深さの制限に達すると手を評価する
    if(level == 0)
    {
        return evaluation(putX,putY,nextPlayer);
    }


    int y,x;
    for(y = 5; y < BOARD_MAX-6; y++)
    {
        for(x = 5; x < BOARD_MAX-6; x++)
        {
            //石が置いてある場所は探索しない
            if(tempBoard[y][x] != EMPTY) continue;

            //仮で石を打つ
            tempBoard[y][x] = player;

            //ミニマックス法で評価値を得る
            if(nextPlayer == AI)
            {
                score = minMax(level-1, nextPlayer, x, y, maxScore);
            }
            else
            {
                score = minMax(level-1, nextPlayer, x, y, minScore);
            }

            if(level == 1)
            {
                tempBoard[y][x] = EMPTY;
                printf("level-score:%d\n",score);
                return score;
            }
            
        
            if(player == AI)
            {
                //printf("AI\n");
                //評価値が一番低い手を選ぶ
                if(score < minScore)
                {
                    printf("AI:%d\n",score);
                    minScore = score;
                }

                if(beforeBranchScore > minScore)
                {
                    //仮で打った石を消す
                    printf("αカット\n");
                    printf("beforeBranchScore : %d > minScore : %d\n", beforeBranchScore, minScore);
                    tempBoard[y][x] = EMPTY;
                    break;
                }


            }
            else
            {
                //printf("ENEMY\n");
                //評価値が一番高い手を選ぶ
                if(score > maxScore)
                {
                    printf("ENEMY:%d\n",score);
                    maxScore = score;
                }

                if(beforeBranchScore < maxScore)
                {
                    //仮で打った石を消す
                    printf("βカット\n");
                    printf("beforeBranchScore : %d < maxScore : %d\n", beforeBranchScore, maxScore);
                    tempBoard[y][x] = EMPTY;
                    break;
                }
            }

            //仮で打った石を消す
            tempBoard[y][x] = EMPTY;
            
        }
    }

    if(player == AI)
    {
        returnScore = minScore;
    }
    else
    {
        returnScore = maxScore;
    }

    printf("retutnScore:%d\n",returnScore);
    return returnScore;
}

/* ====================================================================== */
/**
 * @brief  打った手を評価する
 *
 * @param[in] x,y    評価したい手の座標
 *
 * @return  評価値(整数)
 */
/* ====================================================================== */
int evaluation(int putX, int putY, int player)
{
    int score;
    int maxScore    = INT_MIN;
    int minScore    = INT_MAX;
    int returnScore = 0;


    //初期化
    unsigned int sta[DIRECTION];
    int row[DIRECTION]                  = {0};
    int patern[DIRECTION]               = {0};
    int total[TOTAL_DIRECTION]          = {0};
    int directionPoint[TOTAL_DIRECTION] = {0};

    int i = 0;
    for(i = 0; i < DIRECTION; i++)
    {
        sta[i] = 0x0000;
    }

    printf("%d\n",player);
    printf("%d,%d\n",putX,putY);
    display(0);
    
    int direction = 0;
    for(direction = 0; direction < DIRECTION; direction++)
    {
        row[direction] = countStone(sta,player,putX,putY,direction);
        //printf("sta[%d]:%04x\n",direction,sta[direction]);
        //printf("row[%d]:%d\n",direction,row[direction]);
    }

    for(direction = 0; direction < DIRECTION; direction++)
    {
        patern[direction] = getPatern(sta,direction);
        //printf("patern[%d]:%d\n",direction,patern[direction]);
    }

    int m = 0;
    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        total[m] = row[m] + row[m + 4] -1;
        //printf("total[%d]:%d\n",m,total[m]);
    }


    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        directionPoint[m] =getEvaluationPoint(total[m],patern[m],patern[m + 4]);
        //printf("directionPoint[%d]:%d\n",m,directionPoint[m]);
        if(directionPoint[m] > maxScore)
        {
            maxScore = directionPoint[m];
        }
    }

    returnScore = maxScore;
    printf("directionPoint:%d\n",returnScore);

    tempBoard[putY][putX] = EMPTY;

    return returnScore;
}

/* ====================================================================== */
/**
 * @brief  打った手から1つの方向の石の数と状態を得る
 *
 * @param[in,out] sta     打った手のある方向による状態
 * @param[in]             評価したいプレイヤー
 * @param[in] putX,putY   評価したい手の座標
 * @param[in] direction   検索したい方向
 *
 * @return  連続した石の数
 */
/* ====================================================================== */
int countStone(unsigned int sta[], int player, int putX, int putY, int direction)
{
    int stoneCount = 0;
    int emptyCount = 0;
    int k = 0;
    int directionX = 0;
    int directionY = 0;
    int continuanceStoneFlag = 1;

    switch(direction)
    {
        case 0:
            directionX = 0;
            directionY = -1;
            break;
        
        case 1:
            directionX = 1;
            directionY = -1;
            break;
        case 2:
            directionX = 1;
            directionY = 0;
            break;
        case 3:
            directionX = 1;
            directionY = 1;
            break;
        case 4:
            directionX = 0;
            directionY = 1;
            break;
        case 5:
            directionX = -1;
            directionY = 1;
            break;
        case 6:
            directionX = -1;
            directionY = 0;
            break;
        case 7:
            directionX = -1;
            directionY = -1;
            break;
    }

    for(k = 0; k < MAX_SEARCH_STONE_NUM; k++)
    {
        if((putY + directionY * k) < 0 || (putX + directionX * k) < 0 || (putY + directionY * k) >= BOARD_MAX || (putX + directionX * k) >= BOARD_MAX)
        {
            break;
        }

        if(tempBoard[putY + directionY * k][putX + directionX * k] == player)
        {
            sta[direction] = sta[direction] << 2;
            if(continuanceStoneFlag == 1)
            {
                stoneCount++;
            }
        }
        else if(tempBoard[putY + directionY * k][putX + directionX * k] == EMPTY)
        {
            continuanceStoneFlag = 0;
            sta[direction] = sta[direction] << 2;
            sta[direction] = sta[direction] + 0x0001;
            emptyCount++;
            if(emptyCount >= 3) break;
        }
        else
        {
            continuanceStoneFlag = 0;
            sta[direction] = sta[direction] + 0x0400;
            break;
        }
    }

    return stoneCount;
}

/* ====================================================================== */
/**
 * @brief  打った手の方向の状態がどのパターンか得る
 *
 * @param[in,out] sta     打った手のある方向による状態
 *
 * @return  パターン
 */
/* ====================================================================== */
int getPatern(unsigned int sta[], int direction)
{
    int patern = 10;

    switch(sta[direction])
    {
        case 0x0000:
            patern = STATE_V;
            break;

        case 0x0001:
        case 0x0005:
        case 0x0015:
            patern = STATE_A;
            break;
        
        case 0x0004:
        case 0x0010:
        case 0x0040:
        case 0x0100:
        case 0x0400:
            patern = STATE_X;
            break;
        
        case 0x0011:
        case 0x0045:
        case 0x0114:
        case 0x0115:
        case 0x0411:
        case 0x0445:
            patern = STATE_B;
            break;

        case 0x0105:
        case 0x0441:
            patern = STATE_C;
            break;
        
        case 0x0041:
        case 0x0104:
        case 0x0410:
            patern = STATE_D;
            break;

        case 0x0044:
        case 0x0110:
        case 0x0111:
        case 0x0404:
        case 0x0444:
            patern = STATE_E;
            break;
        
        case 0x0101:
        case 0x0440:
            patern = STATE_F;
            break;
        
        case 0x0014:
        case 0x0050:
        case 0x0051:
        case 0x0140:
        case 0x0141:
        case 0x0401:
        case 0x0405:
        case 0x0414:
        case 0x0450:
            patern = STATE_O;
            break;
        
        default:
            patern = 0;

    }

    
    return patern;
}

/* ====================================================================== */
/**
 * @brief  打った手がどのような手か判定する
 *
 * @param[in] total           黒が連続で置かれている数
 * @param[in] patern1,patern2 
 *
 * @return  パターン
 */
/* ====================================================================== */
int getEvaluationPoint(int total,int patern1, int patern2)
{
    static int evaluationPoint[4][8][8] = {
        {
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_THREE,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR_FOUR ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  },
            {MOVE_JUMP_THREE,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_JUMP_THREE},
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  },
            {MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_FOUR ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_THREE,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  }
        },
        {
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_THREE,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR_THREE,MOVE_FOUR_THREE,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_JUMP_THREE,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR_THREE,MOVE_FOUR_THREE,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_THREE},
            {MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_THREE,MOVE_FOUR_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR },
            {MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_THREE,MOVE_FOUR_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_THREE,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  }
        },
        {
            {MOVE_THREE     ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_THREE     },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR ,MOVE_FOUR_FOUR ,MOVE_JUMP_FOUR ,MOVE_JUMP_FOUR },
            {MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  },
            {MOVE_THREE     ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_JUMP_FOUR ,MOVE_NO_POINT  ,MOVE_NO_POINT  }
        },
        {
            {MOVE_WIN_FOUR  ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_WIN_FOUR  },
            {MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      },
            {MOVE_FOUR      ,MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR      },
            {MOVE_FOUR      ,MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR      },
            {MOVE_FOUR      ,MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR      },
            {MOVE_FOUR      ,MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR      },
            {MOVE_FOUR      ,MOVE_FOUR      ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_NO_POINT  ,MOVE_FOUR      },
            {MOVE_WIN_FOUR  ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_FOUR      ,MOVE_WIN_FOUR  }
        }
    };

    if(total == 5)
    {
        return MOVE_WIN;
    }
    else if(battingFirstPlayer == AI && total >= 6)
    {
        return MOVE_FORBIDDEN;
    }

    return evaluationPoint[total-1][patern1][patern2];
}