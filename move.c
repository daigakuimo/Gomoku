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


    //初期化
    unsigned char sta[DIRECTION];
    int row[DIRECTION] = {0};
    int patern[DIRECTION] = {0};
    int total[TOTAL_DIRECTION] = {0};

    int i = 0;
    for(i = 0; i < DIRECTION; i++)
    {
        sta[i] = 0x0000;
    }

    int directionX = 0;
    int directionY = 1;
    
    int direction = 0;
    for(direction = 0; direction < DIRECTION; direction++)
    {
        row[i] = countBlack(sta[i],player,putX,putY,direction);
    }

    int m = 0;
    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        total[m] = sta[m] + sta[m + 4] -1;
    }


    


    

    return returnScore;
}

/* ====================================================================== */
/**
 * @brief  打った手から1つの方向の黒の数と状態を得る
 *
 * @param[in,out] sta     打った手のある方向による状態
 * @param[in]             評価したいプレイヤー
 * @param[in] putX,putY   評価したい手の座標
 * @param[in] direction   検索したい方向
 *
 * @return  黒の数
 */
/* ====================================================================== */
int countBlack(unsigned char sta, int player, int putX, int putY, int direction)
{
    int blackCount = 0;
    int emptyCount = 0;
    int k = 0;
    int directionX = 0;
    int directionY = 0;

    switch(direction)
    {
        case 0:
            directionX = 0;
            directionY = 1;
            break;
        
        case 1:
            directionX = 1;
            directionY = 1;
            break;
        case 2:
            directionX = 1;
            directionY = 0;
            break;
        case 3:
            directionX = 1;
            directionY = -1;
            break;
        case 4:
            directionX = 0;
            directionY = -1;
            break;
        case 5:
            directionX = -1;
            directionY = -1;
            break;
        case 6:
            directionX = -1;
            directionY = 0;
            break;
        case 7:
            directionX = -1;
            directionY = 1;
            break;
    }

    for(k = 0; k < MAX_SEARCH; k++)
    {
        if(tempBoard[putY + directionY * k][putX + directionX * k] == player)
        {
            sta = sta << 2;
            blackCount++;
        }
        else if(tempBoard[putY + directionY * k][putX + directionX * k] == EMPTY)
        {
            sta = sta << 2;
            sta = sta + 0x0001;
            emptyCount++;
            if(emptyCount >= 3) break;
        }
        else
        {
            sta = sta + 0x0400;
            break;
        }
    }

    return blackCount;
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
int getPatern(unsigned char sta)
{
    int patern;

    switch(sta)
    {
        case 0x0000:
            patern = 0;
    }

    
    return patern;
}
