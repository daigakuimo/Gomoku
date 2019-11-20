#include <stdlib.h>
#include <stdio.h>

void init(char *ar[]);

int main(int argc, char *argv[]) 
{	
    init(argv);

    return 0;
}

void init(char *ar[])
{
    printf("%s",ar[1]);
}