#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[])
{
    /* Initialisation du générateur au début de chaque fils*/
    srand(getpid()); //PID car si le nb random serait basé sur le tps, étant donné que les 2 fils sont crées quasiment simultanément, le nombre random serait identique.
    /* génération d'un nombre */
    int n = (rand() % 8) + 5; //un nombre entre 5 et 12
    printf("Le fils numéro %s attend %d seconde(s) !\n", argv[1], n);
    fflush(stdout);
    usleep(n * 1000000); //n en micro-secondes

    //On est dans le fils 1
    if (atoi(argv[1]) == 1)
        //On envoie le signal USR1 à tous les proccessus qui font partie du grp du proccessus qui à envoyé ce signal (0)
        kill(getppid(), SIGUSR1);

    //On est dans le fils 2
    if (atoi(argv[1]) == 2)
        //On envoie le signal USR2 à tous les proccessus qui font partie du grp du proccessus qui à envoyé ce signal (0)
        kill(getppid(), SIGUSR2);
}