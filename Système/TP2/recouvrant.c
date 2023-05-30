#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    sleep(atoi(argv[1]));
    printf("Réveil, le fils : %d, le père : %d \n", getpid(), getppid());
    return atoi(argv[1]);
}