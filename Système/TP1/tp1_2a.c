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
        printf("%d, %d", getpid(), getppid());
        fflush(stdout);
        execl("./recouvrant", "recouvrant", argv[1], argv[2], NULL);
        perror("Le recouvrement est impossible!");
        exit(1);
        break;
    default: //Bloc du père
        printf("%d, %d", getpid(), getppid());
        fflush(stdout);
        break;
    }
    exit(0);
}
