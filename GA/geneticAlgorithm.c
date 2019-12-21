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

    int i,j;
    for(i = 0; i < GENETIC_NUM; i++)
    {
        if(maxFitness < fitness[i])
        {
            maxFitness        = fitness[i];
            currentWinGenetic = i;
        }

        printf("%d : %d\n", i, fitness[i]);
    }

    if(beforeWinGenetic == currentWinGenetic)
    {
        consecutiveWin++;
    }
    else
    {
        consecutiveWin = 0;
    }

    beforeWinGenetic = currentWinGenetic;

    printf("currentWinGenetic : %d\n", currentWinGenetic);


    // // 終了条件を満たすまで繰り返す
    int turn = 0;
    while(consecutiveWin <= TERMINATION)
    {

        // 選択(バイナリートーナメント)
        geneticSelect(fitness,remainGenetic);

        printf("select\n");
        for(i = 0; i < REMAIN_GENETIC_NUM; i++)
        {
            printf("%d : %d\n", i, remainGenetic[i]);
        }

        printf("\n");

        
        // 交配 
        crossover(remainGenetic);

        // 突然変異
        mutation();
        

        for(i = 0; i < GENETIC_NUM; i++)
        {
            fitness[i] = 0;
        }
        
        for(i = 0; i < REMAIN_GENETIC_NUM; i++)
        {
            remainGenetic[i] = 0;
        }

        for(i = 0; i < GENETIC_NUM; i++)
        {
            for(j = 0; j < EVALUATION_PATERN_NUM; j++)
            {
                printf("%d,",evaluationPaternGenetic[i][j]);
            }
            printf("\n");
        }

        // 評価
        geneticEvaluation(fitness);

        // 勝ち数カウント
        int i;
        maxFitness = 0;
        for(i = 0; i < GENETIC_NUM; i++)
        {
            if(maxFitness < fitness[i])
            {
                maxFitness        = fitness[i];
                currentWinGenetic = i;
            }

            printf("%d : %d\n", i, fitness[i]);
        }

        if(beforeWinGenetic == currentWinGenetic)
        {
            consecutiveWin++;
        }
        else
        {
            consecutiveWin = 0;
        }

        beforeWinGenetic = currentWinGenetic;

        printf("before:%d\n",beforeWinGenetic);
        printf("current:%d\n",currentWinGenetic);
        printf("win num:%d\n",consecutiveWin);

    }

    for(i = 0; i < GENETIC_NUM; i++)
    {
        for(j = 0; j < EVALUATION_PATERN_NUM; j++)
        {
            printf("%d,",evaluationPaternGenetic[i][j]);
        }
        printf("\n");
    }

    

    return 0;
}

