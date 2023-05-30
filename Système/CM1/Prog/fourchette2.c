#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int i = 0;
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
        i += 50;
        break; //exist(0)
    default:   //Bloc du père
        i += 10;
    }
    i += 4;
    printf("%d\n", i);
}