#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("Hello World !\n");
    if (argc == 2)
    { //Si le bon nombre d'arguments est donnés
        printf("Bonjour %s, Je suis le programme %s\n", argv[1], argv[0]);
    }
    exit(0);
}