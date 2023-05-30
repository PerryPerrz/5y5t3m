#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork();
pid_t ret;

int main(int argc, char *argv[])
{
    printf("Bienvenue dans un film!\n");
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        printf("NOOOOOON\n");
        break; //exist(0)
    default:   //Bloc du père
        printf("Luc je suis ton père\n");
    }
    printf("C'est la fin\n");
}