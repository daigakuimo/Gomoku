﻿#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

extern int board[BOARD_MAX][BOARD_MAX];        //ゲーム用?
extern int tempBoard[BOARD_MAX][BOARD_MAX];    //探索用配列

int true = 1,false = 0;

/* ====================================================================== */
/**
 * @brief  禁じ手の判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

int forbidden(int player,int x,int y)
{
    if(Long(player,x,y) == true) return true;
    if(Four(player,x,y) == 2) return true;
    if(Three(player,x,y) == 2) return true;
}

/* ====================================================================== */
/**
 * @brief  盤面に長連があるかの判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

int Long(int player,int x,int y)
{
    int cnt = 0;
    if(Long_yoko(player,x,y) == true) cnt++;
    if(Long_tate(player,x,y) == true) cnt++;
    if(Long_right(player,x,y) == true) cnt++;
    if(Long_left(player,x,y) == true) cnt++;

    if(cnt == 1) return true;
    else return false;
}

/* ====================================================================== */
/**
 * @brief  直前に置いた手によって四が何個出来たかの判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 0 or 1 or 2 四の数(2の場合は四四)　
 * 
 */
/* ====================================================================== */

int Four(int player,int x,int y)
{
    int cnt = 0;
    if(Four_yoko(player,x,y) == true) cnt++;
    if(Four_tate(player,x,y) == true) cnt++;
    if(Four_right(player,x,y) == true) cnt++;
    if(Four_left(player,x,y) == true) cnt++;

    if(cnt == 2) return 2;
    if(cnt == 1) return 1;
    if(cnt == 0) return 0;
}

/* ====================================================================== */
/**
 * @brief  直前に置いた手によって三が何個出来たかの判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 * @param[in] x,y 盤面上のx座標,盤面上のy座標 頂点は左上(検索する座標)
 *
 * @return 0 or 1 or 2 三の数(2の場合は三三)
 * 
 */
/* ====================================================================== */

int Three(int player,int x,int y)
{
    int cnt = 0;
    if(Three_yoko(player,x,y) == true) cnt++;
    if(Three_tate(player,x,y) == true) cnt++;
    if(Three_right(player,x,y) == true) cnt++;
    if(Three_left(player,x,y) == true) cnt++;

    if(cnt == 2) return 2;
    if(cnt == 1) return 1;
    if(cnt == 0) return 0;
}



int Long_yoko(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y][x+k] == player) maru++;
	else if(maru >= 1 && board[y][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 1)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 6) return true;
    }
    
    return false;
}

int Long_tate(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y+k][x] == player) maru++;
	else if(maru >= 1 && board[y+k][x] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 1)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 6) return true;
    }
    
    return false;
}

int Long_right(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y+k][x+k] == player) maru++;
	else if(maru >= 1 && board[y+k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 1)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 6) return true;
    }
    
    return false;
}

int Long_left(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y-k][x+k] == player) maru++;
	else if(maru >= 1 && board[y-k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 1)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 6) return true;
    }
    
    return false;
}

int Four_yoko(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y][x+k] == player){
	    if(x+k-1 >= 0 && maru == 0) top = board[y][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 4)
	{
	    if(x+k+1 < BOARD_MAX) bottom = board[y][x+k+1];

	    if(peke == 1 ) return false;
	    if(nashi == 0 && top != EMPTY && bottom != EMPTY) return false;

	    return true;
	}
    }
    
    return false;
}

int Four_tate(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y+k][x] == player){
	    if(y+k-1 >= 0 && maru == 0) top = board[y+k-1][x];
	    maru++;
	}
	else if(maru >= 1 && board[y+k][x] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 4)
	{
	    if(y+k+1 < BOARD_MAX) bottom = board[y+k+1][x];

	    if(peke == 1 ) return false;
	    if(nashi == 0 && top != EMPTY && bottom != EMPTY) return false;

	    return true;
	}
    }
    
    return false;
}

int Four_right(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y+k][x+k] == player){
	    if(y+k-1 >= 0 && x+k-1 >= 0 && maru == 0) top = board[y+k-1][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y+k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 4)
	{
	    if(y+k+1 < BOARD_MAX && x+k+1 < BOARD_MAX) bottom = board[y+k+1][x+k+1];

	    if(peke == 1 ) return false;
	    if(nashi == 0 && top != EMPTY && bottom != EMPTY) return false;

	    return true;
	}
    }
    
    return false;
}

int Four_left(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -4;k <= 4;k++)
    {
	if(board[y-k][x+k] == player){
	    if(x+k-1 >= 0 && y-k+1 < BOARD_MAX && maru == 0) top = board[y-k+1][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y-k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 4)
	{
	    if(x+k+1 < BOARD_MAX && y-k-1 >= 0) bottom = board[y-k-1][x+k+1];

	    if(peke == 1 ) return false;
	    if(nashi == 0 && top != EMPTY && bottom != EMPTY) return false;

	    return true;
	}
    }
    
    return false;
}

int Three_yoko(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -3;k <= 3;k++)
    {
	if(board[y][x+k] == player){
	    if(x+k-1 >= 0 && maru == 0) top = board[y][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 3)
	{
	    if(x+k+1 < BOARD_MAX) bottom = board[y][x+k+1];

	    if(peke == 1 ) return false;
	    if(top == EMPTY && bottom == EMPTY) return true;

	    return false;
	}
    }
    
    return false;
}

int Three_tate(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -3;k <= 3;k++)
    {
	if(board[y+k][x] == player){
	    if(y+k-1 >= 0 && maru == 0) top = board[y+k-1][x];
	    maru++;
	}
	else if(maru >= 1 && board[y+k][x] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 3)
	{
	    if(y+k+1 < BOARD_MAX) bottom = board[y+k+1][x];

	    if(peke == 1 ) return false;
	    if(top == EMPTY && bottom == EMPTY) return true;

	    return false;
	}
    }
    
    return false;
}

int Three_right(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -3;k <= 3;k++)
    {
	if(board[y+k][x+k] == player){
	    if(y+k-1 >= 0 && x+k-1 >= 0 && maru == 0) top = board[y+k-1][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y+k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 3)
	{
	    if(y+k+1 < BOARD_MAX && x+k+1 < BOARD_MAX) bottom = board[y+k+1][x+k+1];

	    if(peke == 1 ) return false;
	    if(top == EMPTY && bottom == EMPTY) return true;

	    return false;
	}
    }
    
    return false;
}

int Three_left(int player,int x,int y)
{
    int k;
    int maru = 0,peke = 0,nashi = 0,top = 0,bottom = 0;

    for(k = -3;k <= 3;k++)
    {
	if(board[y-k][x+k] == player){
	    if(x+k-1 >= 0 && y-k+1 < BOARD_MAX && maru == 0) top = board[y-k+1][x+k-1];
	    maru++;
	}
	else if(maru >= 1 && board[y-k][x+k] != EMPTY) peke++;
	else if(maru >= 1) nashi++;

	if((peke + nashi) == 2)
	{
	    maru = 0;
	    peke = 0; 
	    nashi = 0;
	}
	
	if(maru == 3)
	{
	    if(x+k+1 < BOARD_MAX && y-k-1 >= 0) bottom = board[y-k-1][x+k+1];

	    if(peke == 1 ) return false;
	    if(top == EMPTY && bottom == EMPTY) return true;

	    return false;
	}
    }
    
    return false;
}