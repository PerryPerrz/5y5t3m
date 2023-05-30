#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int cpt = 0;

/* handler pour SIGINT */
void handler_pour_signal(int sig)
{
    printf("Vous venez d'exécuter un ctrl + c");
    fflush(stdout);
    cpt++;
    if (cpt == 2)
        exit(1);
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

    int sigprocmask(int op, const sigset_t *pNouveauMasque, sigset_t *pAncienMasque); //Masque pour le signal SIGINT

    sigset_t masque, masque_avant;
    sigaddset(&masque, SIGINT);

    /* Installation effective : */
    sigprocmask(SIG_SETMASK, &masque, &masque_avant);
    /* Le signal SIGINT est maintenant bloqué */

    for (int i = 0; i < 40; ++i)
    {
        printf("%d", i);
        fflush(stdout);
        usleep(500000); //500ms
    }

    /* Désinstallation : */
    sigprocmask(SIG_SETMASK, &masque_avant, NULL);
    // a replacé le masque tel qu'il était avant l'installation du blocage

    for (int j = 40; j < 60; ++j)
    {
        printf("%d", j);
        fflush(stdout);
        usleep(500000); //500ms
    }
}
