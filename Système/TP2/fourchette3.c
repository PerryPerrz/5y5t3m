#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define N 300

pid_t fork();

void nbOccurences(int *tab, int n, int val)
{
    int cpt = 0;
    for (int i = 0; i < n; ++i)
    {
        if (tab[i] == val)
        {
            cpt++;
        }
    }
    printf("Nombre de %d dans tab : %d , sa fréquence : %d/100\n", val, cpt, cpt * 100 / n);
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pid_t ret;
    int *tab = malloc(sizeof(int) * N);

    for (int i = 0; i < N; ++i)
    {
        tab[i] = rand() % 20; //Nombre entre 0 et 20
    }
    for (int i = 0; i < 20; ++i) //On créer 20 fils
    {
        switch (ret = fork())
        {
        case (pid_t)-1:
            perror("creation impossible\n");
            exit(1);
        case (pid_t)0:               //Bloc du fils
            nbOccurences(tab, N, i); //Chaque fils s'occupe d'un nombre
            free(tab);
            exit(0); //On tue le fils avant de passer au prochain
            break;
        default: //Bloc du père

            break;
        }
    }
    for (int i = 0; i < 22; ++i)
    {
        int *status = malloc(sizeof(int));
        pid_t res = wait(status);
    }
    free(tab);
    exit(0);
}