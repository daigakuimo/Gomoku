#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];

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
    int score;
    int bestX = 0, bestY = 0;


    int y,x;
    for(y = 0; y < BOARD_MAX; y++)
    {
        for(x = 0; x < BOARD_MAX; x++)
        {
            //石が置いてある場所は探索しない
            if(board[y][x] != EMPTY) continue;

            //仮で石を打つ
            tempBoard[y][x] = AI;

            //ミニマックス法で評価値を得る
            score = minMax(SEARCH_DEPTH, AI, x, y);

            //評価値が一番高い手を選ぶ
            if(score > maxScore)
            {
                maxScore = score;
                bestX = x;
                bestY = y;
            }

            //仮で打った石を消す
            tempBoard[y][x] = EMPTY;
            
        }
    }

    //ミニマックス法で選ばれた手を返す
    sprintf(input,"%d,%d", bestX, bestY);
    
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
int  minMax(int level, int player, int x, int y)
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


    //禁じ手ならこれ以上探索せずに評価値を返す


    //探索する深さの制限に達すると手を評価する
    if(level == 0)
    {
        return eveluation(x,y,player);
    }

    int y,x;
    for(y = 0; y < BOARD_MAX; y++)
    {
        for(x = 0; x < BOARD_MAX; x++)
        {
            //石が置いてある場所は探索しない
            if(tempBoard[y][x] != EMPTY) continue;

            //仮で石を打つ
            tempBoard[y][x] = player;

            //ミニマックス法で評価値を得る
            score = minMax(level-1, nextPlayer, x, y);

            if(player == ENEMY)
            {
                //評価値が一番低い手を選ぶ
                if(score < minScore)
                {
                    minScore = score;
                }
            }
            else
            {
                //評価値が一番高い手を選ぶ
                if(score > maxScore)
                {
                    maxScore = score;
                }
            }

            //仮で打った石を消す
            tempBoard[y][x] = EMPTY;
            
        }
    }

    if(player == ENEMY)
    {
        returnScore = minScore;
    }
    else
    {
        returnScore = maxScore;
    }
    

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

    

    return returnScore;
}
