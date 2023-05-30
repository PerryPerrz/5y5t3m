#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

//Programme principal | Hugo Iopeti

pid_t fork();
pid_t joe, jack, william, averell; //Le fils Joe, Jack ,William ,Averell

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
        //Je créer un tube par fils
        int tube_joe[2], tube_jack[2], tube_averell[2], tube_william[2];
        char temp; //le char lu actuellement

        //Je regarde si la création des tubes se passe correctement
        if (pipe(tube_joe) == -1)
        {
            perror("Impossible d'ouvrir le tube de Joe");
            exit(1);
        }
        if (pipe(tube_jack) == -1)
        {
            perror("Impossible d'ouvrir le tube de Jack");
            exit(1);
        }
        if (pipe(tube_averell) == -1)
        {
            perror("Impossible d'ouvrir le tube d'Averell");
            exit(1);
        }
        if (pipe(tube_william) == -1)
        {
            perror("Impossible d'ouvrir le tube de William");
            exit(1);
        }

        switch (joe = fork())
        {
        case (pid_t)-1:
            //Gestion des erreurs
            perror("creation impossible\n");
            exit(1);
        case (pid_t)0:                                    //Bloc de Joe
            execl("./TP6_freres", "Joe", tube_joe, NULL); //On envoie le nom du fils en plus, ainsi, il est récupérable dans le programme recouvrant.
            perror("Attention, impossible d'effectuer le recouvrement !\n");
            exit(2);
            break;
        default:
            switch (jack = fork())
            {
            case (pid_t)-1:
                perror("creation impossible\n");
                exit(1);
            case (pid_t)0: //Bloc de Jack
                execl("./TP6_freres", "Jack", tube_jack, NULL);
                perror("Attention, impossible d'effectuer le recouvrement !\n");
                exit(2);
                break;
            default:
                switch (william = fork())
                {
                case (pid_t)-1:
                    perror("creation impossible\n");
                    exit(1);
                case (pid_t)0: //Bloc de William
                    execl("./TP6_freres", "William", tube_william, NULL);
                    perror("Attention, impossible d'effectuer le recouvrement !\n");
                    exit(2);
                    break;
                default:
                    switch (averell = fork())
                    {
                    case (pid_t)-1:
                        perror("creation impossible\n");
                        exit(1);
                    case (pid_t)0: //Bloc d'Averell
                        execl("./TP6_freres", "Averell", tube_averell, NULL);
                        perror("Attention, impossible d'effectuer le recouvrement !\n");
                        exit(2);
                        break;
                    default: //Bloc de Ma
                        //On ferme les accès inutilisés (0 = lecture du pipe, 1 = écriture du pipe)
                        close(tube_joe[0]);
                        close(tube_jack[0]);
                        close(tube_william[0]);
                        close(tube_averell[0]);

                        //Ouverture du fichier TP6_cailloux.txt
                        int file = open("TP6_cailloux.txt", O_RDONLY);
                        //On test si l'ouverture se passe bien
                        if (file == -1)
                        {
                            perror("Impossible d'ouvrir le fichier");
                            exit(1);
                        }

                        //Lecture du fichier char par char
                        while (read(file, &temp, sizeof(char)) > 0)
                        {
                            switch (temp)
                            {
                            case 'r': //Si le caillou est rouge, on l'envoie à Joe
                                write(tube_joe[1], &temp, sizeof(char));
                                break;
                            case 'n': //Si le caillou est noir, on l'envoie à Jack
                                write(tube_jack[1], &temp, sizeof(char));
                                break;
                            case 'g': //Si le caillou est gris, on l'envoie à William
                                write(tube_william[1], &temp, sizeof(char));
                                break;
                            case 'v': //Si le caillou est vert, on l'envoie à Averell
                                write(tube_averell[1], &temp, sizeof(char));
                                break;
                            default:
                                break;
                            }
                        }

                        //On envoie le char e aux fils pour indiquer que Ma à finie de lire
                        temp = 'e';
                        write(tube_joe[1], &temp, sizeof(char)); //tube_joe[1] = on écrit de le tube de Joe
                        write(tube_jack[1], &temp, sizeof(char));
                        write(tube_william[1], &temp, sizeof(char));
                        write(tube_averell[1], &temp, sizeof(char));

                        //La mère attend les fils
                        int status;
                        waitpid(joe, &status, 0);
                        waitpid(jack, &status, 0);
                        waitpid(william, &status, 0);
                        waitpid(averell, &status, 0);

                        //On ferme les tubes utilisés
                        close(file);
                        close(tube_joe[1]);
                        close(tube_jack[1]);
                        close(tube_averell[1]);
                        close(tube_william[1]);

                        //On ferme la mère
                        printf("Fin\n");
                        exit(0);
                        break;
                    }
                    break;
                }
                break;
            }
            break;
        }
    }
    if (S_ISDIR(s.st_mode))
    {
        printf("C'est un répertoire ! Le n° d'i-node est : %ld\n", s.st_ino);
    }
    /* utilisation de la structure s */
    return 0;
}