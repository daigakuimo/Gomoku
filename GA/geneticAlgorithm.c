#include "parameterGA.h"
#include "../parameter.h"
#include "../move.h"
#include "../forbiddenMove.h"

extern int evaluationPaternGenetic[GENETIC_NUM][EVALUATION_PATERN_NUM];
extern int GAflag;


int main()
{
    int consecutiveWin              = 0;
    int beforeWinGenetic            = -1;
    int currentWinGenetic           = -1;
    int maxFitness                  = 0;
    int fitness[GENETIC_NUM]        = {0};
    int remainGenetic[REMAIN_GENETIC_NUM] = {0};

    //遺伝子初期化
    init();

    // 評価
    geneticEvaluation(fitness);

    int i;
    for(i = 0; i < GENETIC_NUM; i++)
    {
        if(maxFitness < fitness[i])
        {
            maxFitness        = fitness[i];
            currentWinGenetic = i;
        }

        if(beforeWinGenetic != currentWinGenetic)
        {
            beforeWinGenetic = currentWinGenetic; 
            consecutiveWin   = 0;
        }

        consecutiveWin++;

        printf("%d : %d\n", i, fitness[i]);
    }

    printf("currentWinGenetic : %d\n", currentWinGenetic);

    geneticSelect(fitness,remainGenetic);


    printf("select\n");
    for(i = 0; i < REMAIN_GENETIC_NUM; i++)
    {
        printf("%d : %d\n", i, remainGenetic[i]);
    }

    // // 終了条件を満たすまで繰り返す
    // while(consecutiveWin <= TERMINATION)
    // {
    //     // 選択(バイナリートーナメント)
    //     geneticSelect(fitness,remainGenetic);
        
    //     // 交配 
    //     crossover(remainGenetic);

    //     // 突然変異
    //     mutation();

    //     // 評価
    //     // geneticEvaluation(fitness);

    //     // 勝ち数カウント
    //     int i;
            // for(i = 0; i < GENETIC_NUM; i++)
            // {
            //     if(maxFitness < fitness[i])
            //     {
            //         maxFitness        = fitness[i];
            //         currentWinGenetic = i;
            //     }

            //     if(beforeWinGenetic != currentWinGenetic)
            //     {
            //         beforeWinGenetic = currentWinGenetic; 
            //         consecutiveWin   = 0;
            //     }

            //     consecutiveWin++;

            //     printf("%d : %d\n", i, fitness[i]);
            // }


    // }

    

    return 0;
}

