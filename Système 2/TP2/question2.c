#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

/* Réponse à la question 2
Lorsque sa.sa_flags vaut 0, et que l'on fait 2 ctrl + c d'affilé, le premier ctrl + c s'exécute, on rentre dans le handler, on finit le handler, ensuite seulement, on exécute le handler du second ctrl + c.
Le signal est bien masqué pendant l'exécution du handler.
Or, à aucun moment je n'utilise de masque.

Lorsque sa.sa_flags vaut SA_NODEFER, et que l'on fait 2 ctrl + c d'affilé, les 2 ctrl + c s'exécutent en parallèle puis ils se finissent en même temps.
Le signal n'est pas masqué pendant l'exécution du handler.

Version 1 :
Je vais m'endormir...
^CVous venez d'exécuter un ctrl + c
^CLe handler est finiVous venez d'exécuter un ctrl + c
Le handler est fini

Version 2 :
Je vais m'endormir...
^CVous venez d'exécuter un ctrl + c
^CVous venez d'exécuter un ctrl + c
Le handler est finiLe handler est fini

Pour changer de version :
sa.sa_flags = 0; (V1) ou sa.sa_flags = SA_NODEFER; (V2)
*/

/* handler pour SIGINT */
void handler_pour_signal(int sig)
{
    printf("Vous venez d'exécuter un ctrl + c\n");
    fflush(stdout);
    sleep(5);

    printf("Le handler est fini");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    /* Préparation de la structure */
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal;

    // sa.sa_flags = 0;
    sa.sa_flags = SA_NODEFER;

    sigemptyset(&(sa.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGINT, &sa, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    printf("Je vais m'endormir...\n");
    fflush(stdout);

    sleep(25);
}