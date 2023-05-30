#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nivo(int niveau)
{
    for (int i = 0; i < niveau; ++i)
    {
        printf("    ");
    }
}

int taille(char *nameFolder, int niveau)
{
    DIR *pDir = opendir(nameFolder);
    if (pDir == NULL)
    {
        perror("Le répertoire n'existe pas");
        exit(1);
    }
    struct dirent *strukt = readdir(pDir);
    char str[1024];
    int size = 0;
    struct stat s;
    nivo(niveau);
    printf("Répertoire %s\n", nameFolder);
    while (strukt != NULL)
    {
        sprintf(str, "%s/%s", nameFolder, strukt->d_name); //On récupère le chemin
        if (stat(str, &s) == -1)                           //Stat cherche toutes les infos du fichier et les mets dans "s"
        {
            perror("Le fichier/répertoire n'existe pas");
            exit(1);
        }
        //Si c'est un fichier, on incrémente la taille avec la taille du fichier
        if (S_ISREG(s.st_mode))
        {
            nivo(niveau + 1);
            printf("Fichier %s (%ld Octets)\n", strukt->d_name, s.st_size);
            size += s.st_size;
        }
        //Si c'est un répertoire, on rappelle la fonction taille
        if (S_ISDIR(s.st_mode) && strcmp(strukt->d_name, ".") != 0 && strcmp(strukt->d_name, "..") != 0)
        {
            size += taille(str, niveau + 1);
        }
        strukt = readdir(pDir); //On passe au suivant
    }
    nivo(niveau + 1);
    printf("(Taille : %d Octets)\n", size);
    closedir(pDir);
    return size;
}

int main(int argc, char *argv[])
{
    printf("~~~~~~ La taille du répertoire et de ses sous répertoires ~~~~~~\n");
    taille(argv[1], 0);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return 0;
}