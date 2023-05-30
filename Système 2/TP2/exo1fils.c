#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int nb_signal = 0;
/* handler pour SIGUSR1 */
void handler_pour_signal_sigusr1(int sig)
{
    nb_signal += 1;
}

/* handler pour SIGTERM */
void handler_pour_signal_sigterm(int sig)
{
    printf("Nombre de signaux reçus : %d\n", nb_signal);
    printf("Pid du fils : %d\n", getppid());
    fflush(stdout);

    //On stop la boucle infinie/on quitte le programme
    exit(0);
}

int main(int argc, char *argv[])
{
    /* Préparation de la structure */
    struct sigaction sa_usr1;
    sa_usr1.sa_handler = handler_pour_signal_sigusr1;
    sa_usr1.sa_flags = 0;
    sigemptyset(&(sa_usr1.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGUSR1, &sa_usr1, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    /* Préparation de la structure */
    struct sigaction sa_term;
    sa_term.sa_handler = handler_pour_signal_sigterm;
    sa_term.sa_flags = 0;
    sigemptyset(&(sa_term.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGTERM, &sa_term, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    //On attend les signaux
    while (1 == 1)
    {
        pause();
    }
}