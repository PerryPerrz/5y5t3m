#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


//Programme recouvrant | Hugo Iopeti

int main(int argc, char *argv[])
{
    //Affichage de qui je suis
    printf("Je suis %s Dalton\n", argv[0]);

    //Fermeture du tube (écriture)
    close(argv[1][1]);

    //Lecture du tube
    char temp = 's';
    int cpt = 0; //On commence à -1 car la première itération de boucle se fait alors qu'on n'a pas encore read.
    while (temp != 'e')
    {
        cpt += 1;
        read(argv[1][0], &temp, sizeof(char));
    }

    printf("%s à cassé %d cailloux !\n", argv[0], cpt - 1); //Je dois faire -1 au compteur de caillou car sinon, les Daltons cassent un caillou en trop/inéxistant

    //Fermeture du tube (lecture)
    close(argv[1][0]);

    return EXIT_SUCCESS;
}