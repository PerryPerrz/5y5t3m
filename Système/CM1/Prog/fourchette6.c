#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork();
pid_t ret;

int main(int argc, char *argv[])
{
    int *pStatut = malloc(sizeof(int)); //On alloue
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        printf("Le fils s'endort\n");
        sleep(5);
        printf("Le fils se reveille\n");
        break;
    default: //Bloc du père
        sleep(1);
        printf("Le pere attend\n");
        WEXITSTATUS(*pStatut); //init pStatut
        wait(pStatut);         //On attend
        printf("Le pere à finit d'attendre\n");
        break;
    }
    exit(0);
}