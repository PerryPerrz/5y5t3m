#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

pid_t fork();

int main(int argc, char *argv[])
{
    pid_t ret;
    int status;
    int *tab = malloc(sizeof(int) * 4);

    for (int i = 0; i < 4; ++i)
    {
        tab[i] = 1;
    }

    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        printf("Les valeurs du tableau du fils : [");
        for (int i = 0; i < 3; ++i)
        {
            printf("%d, ", tab[i]);
        }
        printf("%d]\n", tab[3]);

        for (int i = 0; i < 4; ++i)
        {
            tab[i] = 4;
        }
        fflush(stdout);
        sleep(2);

        printf("L'adresse du tableau du fils : %p\n", &tab[0]);

        printf("Les valeurs du tableau du fils : [");
        for (int i = 0; i < 3; ++i)
        {
            printf("%d, ", tab[i]);
        }
        printf("%d]\n", tab[3]);

        break;
    default: //Bloc du père
        printf("Les valeurs du tableau du père : [");
        for (int i = 0; i < 3; ++i)
        {
            printf("%d, ", tab[i]);
        }
        printf("%d]\n", tab[3]);

        for (int i = 0; i < 4; ++i)
        {
            tab[i] = 2;
        }

        //On attend que le fils meurt, status récup le status de renvoie (status que renvoie le fils quand il meurt)
        int *status = malloc(sizeof(int));
        pid_t res = wait(status);

        printf("L'adresse du tableau du père : %p\n", &tab[0]);

        printf("Les valeurs du tableau du père : [");
        for (int i = 0; i < 3; ++i)
        {
            printf("%d, ", tab[i]);
        }
        printf("%d]\n", tab[3]);
        break;
    }
    exit(0);
}