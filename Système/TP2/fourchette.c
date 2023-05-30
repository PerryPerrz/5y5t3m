#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

pid_t fork();

int main(int argc, char *argv[])
{
    pid_t ret, retbis;
    int status;

    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        printf("%d , %d \n", getpid(), getppid());
        fflush(stdout);
        execl("./recouvrant", "recouvrant", argv[1], NULL);
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
            execl("./recouvrant", "recouvrant", argv[2], NULL);
            perror("Le recouvrement est impossible!");
            exit(1);
            break;
        default: //Bloc du père
            printf("%d, %d \n", getpid(), getppid());
            waitpid(retbis, &status, 0);
            status = WEXITSTATUS(status);
            printf("%d\n", status);
            fflush(stdout);
            break;
        }
        break;
    }

    exit(0);
}