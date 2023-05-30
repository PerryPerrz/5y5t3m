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
        printf("%d, %d, %d", getpid(), getppid(), ret);
        break;
    default: //Bloc du père
        printf("%d, %d, %d", getpid(), getppid(), ret);
        break;
    }
    exit(0);
}