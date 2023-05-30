#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("J'ai %d %s", atoi(argv[1]) * atoi(argv[2]), argv[3]);
}