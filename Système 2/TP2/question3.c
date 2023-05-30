#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

/* Réponse à la question 3
Le masque créer dans le père est toujours actif dans le fils (sans execel)
On peut comprendre cela car le fils ne reçoit le signal qu'après avoir désinstallé le masque, donc le masque était bien présent (il bloquait le message).
*/

int usr = 0;

/* handler pour SIGUSR1 */
void handler_pour_signal(int sig)
{
    if (usr == 1)
        printf("Le signal vient du %s", "père\n");
    if (usr == 2)
        printf("Le signal vient du %s", "fils\n");
}

int main(int argc, char *argv[])
{
    /* Préparation de la structure */
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGUSR1, &sa, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    // Mise en place du masque
    sigset_t masque, masque_avant;
    sigaddset(&masque, SIGUSR1);

    // Installation effective du masque
    sigprocmask(SIG_SETMASK, &masque, &masque_avant);

    pid_t ret;

    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: // Bloc du fils
        usr = 2;

        printf("J'envoie le signal au père\n");
        fflush(stdout);
        kill(getppid(), SIGUSR1);

        printf("Je suis le fils et je vais m'endormir...\n");
        fflush(stdout);
        sleep(9);

        printf("Je suis le fils et je désinstalle le masque\n");
        fflush(stdout);
        // Désinstallation du masque
        sigprocmask(SIG_SETMASK, &masque_avant, NULL);

        // execl("./exo2fils", "exo2fils", NULL);
        // perror("Le recouvrement est impossible!");
        // exit(1);
        break;
    default: // Bloc du père
        usr = 1;

        printf("Je suis le père et je vais m'endormir...\n");
        fflush(stdout);
        sleep(3); // On att que le fils s'init

        printf("J'envoi le signal au fils\n");
        fflush(stdout);
        kill(ret, SIGUSR1);

        printf("Je suis le père et je désinstalle le masque\n");
        fflush(stdout);
        // Désinstallation du masque
        sigprocmask(SIG_SETMASK, &masque_avant, NULL);

        waitpid(ret, NULL, 0); // On att que le fils est fini
        break;
    }
    exit(0);
}