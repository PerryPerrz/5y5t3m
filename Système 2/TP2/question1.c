#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

/* Réponse à la question 1
Oui, le fait que le processus dorme n'empêche pas le processus de capter un signal, ici, le signal d'arrêt de processus (ctrl + c)
A chaque fois qu'un processus reçoit un signal, il stoppe ses activités et fait ce que le signal souhaite avant de reprendre là ou il en était (sauf si le signal ferme le programme).
*/

/* handler pour SIGINT */
void handler_pour_signal(int sig)
{
    printf("Vous venez d'exécuter un ctrl + c\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    /* Préparation de la structure */
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGINT, &sa, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    printf("Je vais m'endormir...\n");
    fflush(stdout);

    sleep(25);

    printf("Je reprend mes activités\n");
    fflush(stdout);
}