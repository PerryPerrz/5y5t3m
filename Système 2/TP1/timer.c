#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int tps_restant = 0;

/* handler pour SIGINT */
void handler_pour_signal_sigint(int sig)
{
    //Lorsque l'on Ctrl + C
    tps_restant = alarm(0);
    printf("\n%d", tps_restant);
    fflush(stdout);
}

/* handler pour SIGALRM */
void handler_pour_signal_sigalrm(int sig)
{
    //Fin du timer
    printf("\nPerdu !");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    /* Préparation de la structure */
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal_sigint;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGINT, &sa, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    /* Préparation de la structure */
    struct sigaction sa_bis;
    sa_bis.sa_handler = handler_pour_signal_sigalrm;
    sa_bis.sa_flags = 0;
    sigemptyset(&(sa_bis.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGALRM, &sa_bis, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    int n = 10;
    alarm(n);
    pause(); //on stop le prog jusqu'a un signal
}