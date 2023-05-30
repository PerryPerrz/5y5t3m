#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

pid_t fork();

void handler_pour_signal_sigusr2(int sig)
{
}

int main(int argc, char *argv[])
{
    pid_t ret;
    int nb = atoi(argv[1]); // On récupère le nombre de signaux à envoyer passés en argument.
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: // Bloc du fils
        /*printf("%d , %d \n", getpid(), getppid());
        fflush(stdout);*/
        execl("./exo2fils", "exo2fils", NULL);
        perror("Le recouvrement est impossible!");
        exit(1);
        break;
    default: // Bloc du père
        // sleep(1); //On endort le père, on attend que le fils se prépare
        /*printf("%d, %d \n", getpid(), getppid());
        fflush(stdout);*/
        printf("Je ne suis pas une déclaration de variable, enchanté !\n"); // Résolution du "bug" spécifique à C
        /* Préparation de la structure */
        struct sigaction sa_usr2;
        sa_usr2.sa_handler = handler_pour_signal_sigusr2;
        sa_usr2.sa_flags = 0;
        sigemptyset(&(sa_usr2.sa_mask));
        /* Installation effective du handler */
        sigaction(SIGUSR2, &sa_usr2, NULL);
        /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

        // On attend le signal du fils
        pause();

        // Envoie des signaux sigusr1 au fils
        for (int i = 0; i < nb; ++i)
        {
            // printf("%d\n", i);
            // fflush(stdout);
            kill(ret, SIGUSR1);
            pause();
        }

        // Envoie du signal sigterm
        kill(ret, SIGTERM);

        waitpid(ret, NULL, 0);
        break;
    }
    exit(0);
}