#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork();
pid_t ret;

int main(int argc, char *argv[])
{
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        execl("fils", "fils", NULL); //("chemin" (ici, on est dans le fichier source) ,"nom du fichier executable",NULL)
        //fonction execl : On relance un autre programme à l'aide d'un éxecutable
        perror("Le recouvrement impossible ");
        exit(2);
        break;
    default: //Bloc du père
        printf("Mammouth écrase les prix");
    }
    exit(0);
}