#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

pid_t fork();

int tps_reception_usr1 = 0;
int tps_reception_usr2 = 0;
int tps_depart_du_signal = 0;

/* handler pour SIGUSR1 */
void handler_pour_signal_sigusr1(int sig)
{
    int tps_arrive_du_signal = time(NULL);
    tps_reception_usr1 = tps_arrive_du_signal - tps_depart_du_signal;
    pause(); //On pause le prog en attendant le prochain signal (alarm) ou possiblement le fils 2
}

/* handler pour SIGUSR2 */
void handler_pour_signal_sigusr2(int sig)
{
    int tps_arrive_du_signal = time(NULL);
    tps_reception_usr2 = tps_arrive_du_signal - tps_depart_du_signal;
    pause(); //On pause le prog en attendant le prochain signal (alarm) ou possiblement le fils 1
}

/* handler pour SIGALRM */
void handler_pour_signal_sigalrm(int sig)
{
    printf("%d", tps_reception_usr1);
    printf("%d", tps_reception_usr2);
    //Fin du timer
    if (tps_reception_usr1 == 0 && tps_reception_usr2 == 0)
    {
        printf("\nLes 2 fils n'ont pas répondu !");
    }
    else
    {
        if (tps_reception_usr1 > tps_reception_usr2)
            printf("\nLe fils 1 à gagné !");
        if (tps_reception_usr2 > tps_reception_usr1)
            printf("\nLe fils 2 à gagné !");
        if (tps_reception_usr1 == tps_reception_usr2)
            printf("\nLe fils 1 & le fils 2 sont à égalité !");
    }
    fflush(stdout);

    //Le timer est fini = génocide, on tue les 2 fils pour éviter qu'ils se rattachent à un proccessus/ qu'ils continuent à run en fond
    kill(0, SIGINT); //On tue tout le monde
    exit(0);
}

int main(int argc, char *argv[])
{
    pid_t ret, retbis;

    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        printf("%d , %d \n", getpid(), getppid());
        fflush(stdout);
        execl("./timerplusplus_recouvrant", "timerplusplus_recouvrant", "1", NULL);
        perror("Le recouvrement est impossible!");
        exit(1);
        break;
    default: //Bloc du père
        printf("%d, %d \n", getpid(), getppid());
        fflush(stdout);
        switch (retbis = fork())
        {
        case (pid_t)-1:
            perror("creation impossible\n");
            exit(1);
        case (pid_t)0: //Bloc du fils
            printf("%d, %d \n", getpid(), getppid());
            fflush(stdout);
            execl("./timerplusplus_recouvrant", "timerplusplus_recouvrant", "2", NULL);
            perror("Le recouvrement est impossible!");
            exit(1);
            break;
        default: //Bloc du père
            /* Préparation de la structure */
            printf("Je ne suis pas une déclaration de variable, enchanté !\n"); //Résolution du "bug" spécifique à C
            fflush(stdout);
            struct sigaction sa_usr1;
            sa_usr1.sa_handler = handler_pour_signal_sigusr1;
            sa_usr1.sa_flags = 0;
            sigemptyset(&(sa_usr1.sa_mask));
            /* Installation effective du handler */
            sigaction(SIGUSR1, &sa_usr1, NULL); //Le handler sa_usr1 va récupérer les signaux de type SIGUSR1
            /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

            /* Préparation de la structure */
            struct sigaction sa_usr2;
            sa_usr2.sa_handler = handler_pour_signal_sigusr2;
            sa_usr2.sa_flags = 0;
            sigemptyset(&(sa_usr2.sa_mask));
            /* Installation effective du handler */
            sigaction(SIGUSR2, &sa_usr2, NULL);
            /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

            /* Préparation de la structure */
            struct sigaction sa_alrm;
            sa_alrm.sa_handler = handler_pour_signal_sigalrm;
            sa_alrm.sa_flags = 0;
            sigemptyset(&(sa_alrm.sa_mask));
            /* Installation effective du handler */
            sigaction(SIGALRM, &sa_alrm, NULL);
            /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

            int n = 10;
            alarm(n); //On lance l'alarme
            printf("On lance l'alarme !\n");
            fflush(stdout);
            tps_depart_du_signal = time(NULL); //On initialise le tps de départ du signal
            pause();                           //on stop le prog jusqu'a un signal
            break;
        }
        break;
    }

    exit(0);
}
