#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct stat s;
    char ref[1024];
    //On copie un argument dans une chaine de car
    strcpy(ref, argv[1]);
    //On regarde si le fichier/répertoire existe
    if (stat(ref, &s) == -1)
    {
        perror("Le fichier/répertoire n'existe pas");
        exit(1);
    }
    if (S_ISREG(s.st_mode))
    {
        printf("C'est un fichier ! Le n° d'i-node est : %ld\n", s.st_ino);
        printf("taille : %ld Octets\n", s.st_size);
        printf("modif : %s", ctime(&(s.st_mtime)));
        printf("accès : %s\n", ctime(&(s.st_atime)));

        printf("~~~~~ Extrait ~~~~~\n");
        int desc = open(argv[1], O_RDONLY);
        char temp; //char qui contient le car lu actuellement
        int cpt = 0;
        while (read(desc, &temp, 1) > 0 && cpt < 100) //1 car on prend les char 1 par 1
        {
            putchar(temp); //On affiche le car dans la sortie standard
            cpt++;
        }
        close(desc);
        printf("\n~~~~~~~~~~~~~~~~~~~\n");
    }
    if (S_ISDIR(s.st_mode))
    {
        printf("C'est un répertoire ! Le n° d'i-node est : %ld\n", s.st_ino);
    }
    /* utilisation de la structure s */
    return 0;
}