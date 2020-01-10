#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"
#include "GA/parameterGA.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;
extern int turnCount;
extern int winFlag;
extern int GAflag;
extern int genetic;

/* ====================================================================== */
/**
 * @brief  ミニマックス法で打つ手を決める(2019/12/19 次の手の評価のみ)
 * 
 * @param[in,out] input 打つ手を返す
 * 
 */
/* ====================================================================== */
void move(char *input, int player)
{
    int maxScore     = INT_MIN;
    int minScore     = INT_MAX;
    int score;
    int bestX = 0, bestY = 0;
    int searchStartX  = 0;
    int searchStartY  = 0;
    int searchFinishX = BOARD_MAX;
    int searchFinishY = BOARD_MAX;

    if(turnCount < 3)
    {
        searchStartX  = 6;
        searchStartY  = 6;
        searchFinishX = 9;
        searchFinishY = 9;
    }
    else if(turnCount < 6)
    {
        searchStartX  = 5;
        searchStartY  = 5;
        searchFinishX = 10;
        searchFinishY = 10;
    }

    int y,x;
    for(y = searchStartY; y < searchFinishY; y++)
    {
        for(x = searchStartX; x < searchFinishX; x++)
        {
            //石が置いてある場所は探索しない
            if(board[y][x] != EMPTY) continue;

            //仮で石を打つ
            tempBoard[y][x] = player;

            //ミニマックス法で評価値を得る
            //score = minMax(SEARCH_DEPTH, ENEMY, x, y, minScore);
            score = evaluation(x,y,player);

            // printf("%d,%d\n", x, y);
            // printf("move-score:%d\n",score);

            //評価値が一番高い手を選ぶ
            if(score > maxScore)
            {
                maxScore = score;
                bestX = x;
                bestY = y;
                //printf("maxScore:%d\n",maxScore);
            }

            //仮で打った石を消す
            tempBoard[y][x] = EMPTY;
            
        }
    }
    


    if(maxScore == MOVE_WIN || maxScore == MOVE_WIN_FOUR)
    {
        sprintf(input,"%d,%d,win", bestX + 1, bestY + 1,"win");
        winFlag = 1;
    }
    else
    {
        sprintf(input,"%d,%d", bestX + 1, bestY + 1);
    }
    
    //printf("%s\n",input);

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

    int fourCount   = 0;
    int threeCount  = 0;


    // ここから自分の攻めを評価
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
    
    int direction = 0;
    for(direction = 0; direction < DIRECTION; direction++)
    {
        row[direction] = countStone(sta,player,putX,putY,direction);
    }

    for(direction = 0; direction < DIRECTION; direction++)
    {
        patern[direction] = getPatern(sta,direction);
    }

    int m = 0;
    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        total[m] = row[m] + row[m + 4] -1;
    }


    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        directionPoint[m] =getEvaluationPoint(player,total[m],patern[m],patern[m + 4]);
        
        if(directionPoint[m] > maxScore)
        {
            maxScore = directionPoint[m];
        }

        if(directionPoint[m] == MOVE_FOUR)
        {
            fourCount++;
        }
        else if(directionPoint[m] == MOVE_THREE)
        {
            threeCount++;
        }
    }

    // ここから相手の手を防ぐか評価
    //初期化
    unsigned int preventPatern[DIRECTION]      = {0};
    int directionPreventPoint[TOTAL_DIRECTION] = {0};


    for(direction = 0; direction < DIRECTION; direction++)
    {
        preventPatern[direction] = getPreventPatern(player,putX,putY,direction);
    }

    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        directionPreventPoint[m] = getPreventEvaluationPoint(preventPatern[m],preventPatern[m + 4]);

        if(directionPreventPoint[m] > maxScore)
        {
            maxScore = directionPreventPoint[m];
        }
    }
    

    // 禁じ手か判断
    if(battingFirstPlayer == player && (fourCount >= 2 || threeCount >= 2))
    {
        returnScore = MOVE_FORBIDDEN;
    }
    else
    {
        returnScore = maxScore;
    }

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
            sta[direction] = sta[direction] + 0x0400;
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
int getEvaluationPoint(int player, int total,int patern1, int patern2)
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
    int evaluationPointGA[4][8][8] = {
        {
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][4]},
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  }
        },
        {
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][4]},
            {evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] },
            {evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][6],evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][4],evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  }
        },
        {
            {evaluationPaternGenetic[genetic][2]     ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][2]     },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][5] ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][3] },
            {evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  },
            {evaluationPaternGenetic[genetic][2]     ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][3] ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  }
        },
        {
            {evaluationPaternGenetic[genetic][0]  ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][0]  },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][8]  ,evaluationPaternGenetic[genetic][1]      },
            {evaluationPaternGenetic[genetic][0]  ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][1]      ,evaluationPaternGenetic[genetic][0]  }
        }
    };
    

    if(total == 5)
    {
        return MOVE_WIN;
    }
    else if(battingFirstPlayer == player && total >= 6)
    {
        return MOVE_FORBIDDEN;
    }

    if(GAflag)
    {
        return evaluationPointGA[total-1][patern1][patern2];
    }

    return evaluationPoint[total-1][patern1][patern2];
}


int  getPreventPatern(int player, int putX, int putY, int direction)
{
    unsigned int preventState = 0x0000;
    int k = 0;
    int enemy = 0;
    int directionX = 0;
    int directionY = 0;

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

    for(k = 1; k < MAX_SEARCH_STONE_NUM; k++)
    {
        if((putY + directionY * k) < 0 || (putX + directionX * k) < 0 || (putY + directionY * k) >= BOARD_MAX || (putX + directionX * k) >= BOARD_MAX)
        {
            break;
        }

        if(tempBoard[putY + directionY * k][putX + directionX * k] == player)
        {
            preventState = preventState + 0x4000;
            break;
        }
        else if(tempBoard[putY + directionY * k][putX + directionX * k] == EMPTY)
        {
            break;
        }
        else
        {
            preventState = preventState << 2;
            preventState = preventState + 0x0001;
        }
    }

    if(preventState == 0x4000)
    {
        preventState = 0x0000;
    }

    int patern = 10;

    switch(preventState)
    {
        case 0x0000:
            patern = STATE_A;
            break;

        case 0x0001:
            patern = STATE_X;
            break;
        
        case 0x4001:
            patern = STATE_B;
            break;
        
        case 0x0005:
            patern = STATE_C;
            break;

        case 0x4005:
            patern = STATE_D;
            break;
        
        case 0x0015:
            patern = STATE_E;
            break;

        case 0x4015:
            patern = STATE_F;
            break;
        
        case 0x0055:
            patern = STATE_O;
            break;
        
        case 0x4055:
            patern = STATE_V;
            break;
        
        default:
            patern = 0;

    }

    return patern;
}

int  getPreventEvaluationPoint(int patern1, int patern2)
{
    
    static int preventEvaluationPoint[9][9] = {
        {MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,PREVENT_TWO      ,PREVENT_B_TWO    ,PREVENT_THREE    ,PREVENT_B_THREE  ,MOVE_NO_POINT    ,PREVENT_WIN  },
        {MOVE_NO_POINT    ,PREVENT_J_TWO    ,PREVENT_B_J_TWO  ,PREVENT_J_THREE  ,PREVENT_B_J_THREE,PREVENT_WIN      ,PREVENT_WIN      ,MOVE_NO_POINT    ,PREVENT_WIN  },
        {MOVE_NO_POINT    ,PREVENT_B_J_TWO  ,MOVE_NO_POINT    ,PREVENT_B_J_THREE,MOVE_NO_POINT    ,PREVENT_WIN      ,PREVENT_WIN      ,MOVE_NO_POINT    ,PREVENT_WIN  },
        {PREVENT_TWO      ,PREVENT_J_THREE  ,PREVENT_B_J_THREE,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {PREVENT_B_TWO    ,PREVENT_B_J_THREE,MOVE_NO_POINT    ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {PREVENT_THREE    ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {PREVENT_B_THREE  ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT    ,MOVE_NO_POINT},
        {PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
    };

    int preventEvaluationPointGA[9][9] = {
        {evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][13]      ,evaluationPaternGenetic[genetic][15]    ,evaluationPaternGenetic[genetic][9]    ,evaluationPaternGenetic[genetic][11]  ,evaluationPaternGenetic[genetic][8]    ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][14]    ,evaluationPaternGenetic[genetic][15]  ,evaluationPaternGenetic[genetic][10]  ,evaluationPaternGenetic[genetic][12],PREVENT_WIN      ,PREVENT_WIN      ,evaluationPaternGenetic[genetic][8]    ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][15]  ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][12],evaluationPaternGenetic[genetic][8]    ,PREVENT_WIN      ,PREVENT_WIN      ,evaluationPaternGenetic[genetic][8]    ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][13]      ,evaluationPaternGenetic[genetic][10]  ,evaluationPaternGenetic[genetic][12],PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][15]    ,evaluationPaternGenetic[genetic][12],evaluationPaternGenetic[genetic][8]    ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][9]    ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][11]  ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
        {evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]    ,evaluationPaternGenetic[genetic][8]},
        {PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN      ,PREVENT_WIN  },
    };
 
    if(GAflag)
    {
        return preventEvaluationPointGA[patern1][patern2];
    }
    
    
    return preventEvaluationPoint[patern1][patern2];
}