#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

pid_t fork();

int main(int argc, char *argv[])
{
    pid_t ret;
    int nb = atoi(argv[1]); //On récupère le nombre de signaux à envoyer passés en argument.
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        /*printf("%d , %d \n", getpid(), getppid());
        fflush(stdout);*/
        execl("./exo1fils", "exo1fils", NULL);
        perror("Le recouvrement est impossible!");
        exit(1);
        break;
    default:      //Bloc du père
        sleep(1); //On endort le père, on attend que le fils se prépare
        /*printf("%d, %d \n", getpid(), getppid());
        fflush(stdout);*/

        //Envoie des signaux sigusr1 au fils
        for (int i = 0; i < nb; ++i)
        {
            kill(ret, SIGUSR1);
        }
        //Envoie du signal sigterm
        kill(ret, SIGTERM);

        waitpid(ret, NULL, 0);
        break;
    }
    exit(0);
}