#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    DIR *pDir = opendir(argv[1]);
    if (pDir == NULL)
    {
        perror("Le répertoire n'existe pas");
        exit(1);
    }
    struct dirent *strukt = readdir(pDir);
    int cptF = 1;
    int cptR = 1;
    char str[1024];
    int taille = 0;
    struct stat s;
    while (strukt != NULL)
    {
        sprintf(str, "%s/%s", argv[1], strukt->d_name); //On récupère le chemin
        if (stat(str, &s) == -1)                        //Stat cherche toutes les infos du fichier et les mets dans "s"
        {
            perror("Le fichier/répertoire n'existe pas");
            exit(1);
        }
        if (S_ISREG(s.st_mode))
        {
            printf("Fichier n° %d, ", cptF);
            taille += s.st_size;
            cptF++;
        }
        if (S_ISDIR(s.st_mode))
        {
            printf("Répertoire n° %d, ", cptR);
            cptR++;
        }
        printf("Nom : %s, N° i-node : %ld\n", strukt->d_name, strukt->d_ino);
        strukt = readdir(pDir); //On passe au suivant
    }
    printf("Taille du répertoire (sans les sous-répertoires) : %d Octets\n", taille);
    closedir(pDir);
    return 0;
}