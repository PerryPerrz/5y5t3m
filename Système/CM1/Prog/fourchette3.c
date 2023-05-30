#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork();
pid_t ret;

int main(int argc, char *argv[])
{
    int i = 1;
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: //Bloc du fils
        sleep(5);
        exit(0);
    default: //Bloc du père
        while (i == 1)
        {
        }
    }
}