#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"
#include "effectiveMove.h"

extern int board[BOARD_MAX][BOARD_MAX];        //ゲーム用?
extern int tempBoard[BOARD_MAX][BOARD_MAX];    //探索用配列

int true = 1,false = 0;

/* ====================================================================== */
/**
 * @brief  勝利判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

int Win(int player,int x,int y)
{

}

/* ====================================================================== */
/**
 * @brief  盤面に五があるかの判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

int Five(int player,int x,int y)
{

}