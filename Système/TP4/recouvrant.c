#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i = 0;
    printf("Les lettres : ");
    while (argv[1][i] != '\0')
    {
        printf("%c%c | ", argv[1][i] + 32, argv[1][i] + 32); //On affiche toutes les lettres en majuscules en minuscules 2 fois
        ++i;
    }
    return EXIT_SUCCESS;
}