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
int  minMax(int level, int player, int putX, int putY)
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
        return evaluation(putX,putY,player);
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
    unsigned int sta[DIRECTION];
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
        row[direction] = countStone(sta,player,putX,putY,direction);
        printf("sta[%d]:%04x\n",direction,sta[direction]);
        printf("row[%d]:%d\n",direction,row[direction]);
    }

    for(direction = 0; direction < DIRECTION; direction++)
    {
        patern[direction] = getPatern(sta,direction);
        printf("patern[%d]:%d\n",direction,patern[direction]);
    }

    int m = 0;
    for(m = 0; m < TOTAL_DIRECTION; m++)
    {
        total[m] = row[m] + row[m + 4] -1;
        printf("total[%d]:%d\n",m,total[m]);
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
