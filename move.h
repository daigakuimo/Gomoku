#pragma once

//評価値
#define MOVE_WIN                10000    //勝ち確
#define MOVE_WIN_FOUR           800     //勝利四   0  
#define MOVE_FOUR               50     //四　　   1
#define MOVE_THREE              40     //三　　　 2
#define MOVE_JUMP_FOUR          53     //飛び四　 3
#define MOVE_JUMP_THREE         39     //飛び三   4 
#define MOVE_FOUR_FOUR          75     //四四     5
#define MOVE_FOUR_THREE         70     //四三     6
#define MOVE_THREE_THREE        65     //三三     7
#define MOVE_FORBIDDEN          0      //禁じ手   
#define MOVE_NO_POINT           5      //評価無し 8

#define PREVENT_WIN             900     //相手の勝ちを防ぐ
#define PREVENT_THREE           700     //相手の三を防ぐ       9
#define PREVENT_J_THREE         600     //相手の三を防ぐ       10
#define PREVENT_B_THREE         500     //相手の三を挟んで防ぐ  11
#define PREVENT_B_J_THREE       400     //相手の三を挟んで防ぐ  12
#define PREVENT_TWO             300     //相手の二を防ぐ       13
#define PREVENT_J_TWO           200      //相手の二を防ぐ       14
#define PREVENT_B_TWO           7      //相手の二を挟んで防ぐ  15
#define PREVENT_B_J_TWO         6      //相手の二を挟んで防ぐ  16


#define SEARCH_DEPTH             2     //探索の深さ
#define DIRECTION                8     //評価時の検索8方向
#define TOTAL_DIRECTION          4     //評価時の4方向
#define MAX_SEARCH_STONE_NUM     6     //評価時の石検索の数

//評価時の石の状態のパターン
#define STATE_A                  0
#define STATE_X                  1
#define STATE_B                  2
#define STATE_C                  3
#define STATE_D                  4
#define STATE_E                  5 
#define STATE_F                  6
#define STATE_O                  7
#define STATE_V                  8

void move(char *input, int player);
int  minMax(int level, int player, int putX, int putY, int beforeBranchScore, int evaluationTotalScore);
int  evaluation(int putX, int putY, int player, int level);
int  countStone(unsigned int sta[], int player, int putX, int putY, int direction);
int  getPatern(unsigned int sta[], int direction);
int  getEvaluationPoint(int player, int total,int patern1, int patern2);
int  getPreventPatern(int player, int putX, int putY, int direction);
int  getPreventEvaluationPoint(int patern1, int patern2);

