#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("%d (père %d) : Début de programme\n", getpid(), getppid());
    //sleep(10);
    int *tab = malloc(sizeof(int) * 1000000);
    for (int i = 0; i < 1000000; ++i)
    {
        tab[i] = 0;
    }
    for (int i = 0; i < 10000; ++i)
    {
        for (int j = 0; j < 1000000; ++j)
        {
            tab[i] += 1;
        }
        if (i % 1000 == 0)
        {
            printf("%d\n", i);
        }
    }
    printf("%d (père %d) : Fin de programme\n", getpid(), getppid());
    exit(0);
}