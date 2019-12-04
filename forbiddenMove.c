#include "move.h"
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
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

/*int forbidden(int player)
{
    	     
}*/

/* ====================================================================== */
/**
 * @brief  盤面に四四があるかの判定
 * 
 * @param[in] player 判定するプレイヤー(先行)
 *
 * @return 判定　true or false
 * 
 */
/* ====================================================================== */

int Four(int player)
{
    int y,x,z;
    for(y = 0; y < BOARD_MAX; y++)
    {
        for(x = 0; x < BOARD_MAX; x++)
	{
	    if(board[y][x] == player)
	    {
		if(Four_yoko(player,x,y) == true)
		{
		    printf("a\n");
		    for(z=0; z<5;z++)
		    {
			
			if(Four_tate(player,x,y+z) == true) return true;
			if(Four_right(player,x,y+z) == true) return true;
			if(Four_left(player,x,y+z) == true) return true;
		    }
		}
		if(Four_tate(player,x,y) == true)
		{
		    printf("b\n");
		    for(z=0; z<5;z++)
		    {
			if(Four_yoko(player,x+z,y) == true) return true;
			if(Four_right(player,x+z,y) == true) return true;
			if(Four_left(player,x+z,y) == true) return true;
		    }
		}
		if(Four_right(player,x,y) == true)
		{
		    printf("c\n");
		    for(z=0; z<5;z++)
		    {
			
			if(Four_tate(player,x+z,y+z) == true) return true;
			if(Four_yoko(player,x+z,y+z) == true) return true;
			if(Four_left(player,x+z,y+z) == true) return true;
		    }
		}
		/*if(Four_left(player,x,y) == true)
		{
		    printf("d\n");
		    for(z=0; z<5;z++)
		    {
			if(Four_tate(player,x+z,y-z) == true) return true;
			if(Four_right(player,x+z,y-z) == true) return true;
			if(Four_yoko(player,x+z,y-z) == true) return true;
		    }
		}*/
	    }
	}
    } 
    return false;
}

int Four_tate(int player,int x,int y)
{
    int k;
    int disc[4];
    disc[3] = x;
    int cnt = 0,emp1 = 0,emp2 = 0;

    for(k = 1;k < 5;k++)
    {
	if(x+k < BOARD_MAX && emp1 < 2)
	{
	    if(board[y][x+k] == player)
	    { 
		if(cnt < 3) disc[cnt] = x+k;
		cnt++;
	    }
	    else emp1++;
	}	    
    }

    //printf("g\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[y][disc[k]+1] != EMPTY) return false;
	    }
        }
	
	return true;
    }

    for(k = -1;k > -5;k--)
    {
	if(cnt != 3 && x+k >= 0  && emp2 < 2)
	{
	    if(board[y][x+k] == player)
	    {
		if(cnt < 3) disc[cnt] = x+k;
		cnt++;
	    }
	    else emp2++;
	}	    
    }

	//printf("h\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[y][disc[k]+1] != EMPTY) return false;
	    }
        }
	
	return true;
    }

    
}

int Four_yoko(int player,int x,int y)
{
    int k;
    int disc[4];
    disc[3] = y;
    int cnt = 0,emp1 = 0,emp2 = 0;

    for(k = 1;k < 5;k++)
    {
	if(y+k < BOARD_MAX && emp1 < 2)
	{
	    if(board[y+k][x] == player)
	    { 
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp1++;
	}	    
    }

    //printf("e\n");
    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][x] != EMPTY) return false;
	    }
        }
	
	if(board[y-1][x] != EMPTY) return false;
	else return true;

    }

    for(k = -1;k > -5;k--)
    {
	if(cnt != 3 && y+k >= 0  && emp2 < 2)
	{
	    if(board[y+k][x] == player)
	    {
		if(board[y+k][x] != EMPTY)
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp2++;
	}	    
    }
	//printf("f\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][x] != EMPTY) return false;
	    }
        }
	
	return true;
    }

    
}

int Four_right(int player,int x,int y)
{
    int k;
    int disc[4];
    disc[3] = y;
    int cnt = 0,emp1 = 0,emp2 = 0;

    for(k = 1;k < 5;k++)
    {
	if(y+k < BOARD_MAX && x+k < BOARD_MAX && emp1 < 2)
	{
	    if(board[y+k][x+k] == player)
	    { 
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp1++;
	}	    
    }

printf("i\n");

    if(cnt == 3)
    {

	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][disc[k]+1] != EMPTY) return false;
	    }
        }

	if(board[y-1][x-1] != EMPTY) return false;
	else return true;
    }

    for(k = -1;k > -5;k--)
    {
	if(cnt != 3 && y+k >= 0 && x+k >= 0  && emp2 < 2)
	{
	    if(board[y+k][x+k] == player)
	    {
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp2++;
	}	    
    }

	printf("j\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][disc[k]+1] != EMPTY) return false;
	    }
        }
	
	return true;
    }    
}

int Four_left(int player,int x,int y)
{
    int k;
    int disc[4];
    disc[3] = x;
    int cnt = 0,emp1 = 0,emp2 = 0;

    for(k = 1;k < 5;k++)
    {
	if(y+k < BOARD_MAX && x-k >= 0 && emp1 < 2)
	{
	    if(board[y+k][x-k] == player)
	    { 
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp1++;
	}	    
    }

	printf("k\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][disc[k]] != EMPTY) return false;
	    }
        }
	
	return true;
    }

    for(k = -1;k > -5;k--)
    {
	if(cnt != 3 && y+k >= 0  && emp2 < 2)
	{
	    if(board[y+k][x] == player)
	    {
		if(cnt < 3) disc[cnt] = y+k;
		cnt++;
	    }
	    else emp2++;
	}	    
    }

	printf("l\n");

    if(cnt == 3)
    {
	BubSort(disc,4);
    	for(k = 0;k < 3;k++)
    	{
	    if(disc[k+1] - disc[k] != 1)
	    {
	        if(board[disc[k]+1][x] != EMPTY) return false;
	    }
        }
	
	return true;
    }

    
}

void BubSort(int *x , int length){
	int temp;
	int i,j;
	for (i = 0; i < length-1; i++) {
		for (j = 0; j < length-1-i; j++) {
			if (x[j] > x[j+1]) {
				temp = x[j];
				x[j] = x[j+1];
				x[j+1]= temp;
			}
		}
	}
}

