#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

pid_t fork();
pid_t ret;

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

        int cpt = 0;
        int nb_lettres = 0; //Compteur qui compte le nombre de lettres que lit le fils
        char *w = malloc(sizeof(char) * 1024);
        char c;

        int tube[2]; //Ce tube contient le nb de lettres | [1] : écrire, [0] : lire
        pipe(tube);  //On transforme le tableau tube de 2 entiers en pipe.
        int flag[2]; //On dit au père lorsque le fils à terminé, le fils écrit dedans une fois qu'il à fini.
        pipe(flag);
        int tube_bleu[2]; //Ce tube contient le nb de lettres | [1] : écrire, [0] : lire
        pipe(tube_bleu);  //On transforme le tableau tube de 2 entiers en pipe.
        int flag_bleu[2]; //On dit au père lorsque le fils à terminé, le fils écrit dedans une fois qu'il à fini.
        pipe(flag_bleu);
        int tube_vert[2]; //Ce tube contient le nb de lettres | [1] : écrire, [0] : lire
        pipe(tube_vert);  //On transforme le tableau tube de 2 entiers en pipe.
        int flag_vert[2]; //On dit au père lorsque le fils à terminé, le fils écrit dedans une fois qu'il à fini.
        pipe(flag_vert);
        int tube_rouge[2]; //Ce tube contient le nb de lettres | [1] : écrire, [0] : lire
        pipe(tube_rouge);  //On transforme le tableau tube de 2 entiers en pipe.
        int flag_rouge[2]; //On dit au père lorsque le fils à terminé, le fils écrit dedans une fois qu'il à fini.
        pipe(flag_rouge);
        char *message = "Papa ! J'ai fini !\n";

        //On définit ces variables devant le switch car ces variables sont redéfinies dans le fils (on leurs donnent des valeurs dans le fils)
        //Les variables largeur et hauteur sont seulement utilisés par le fils, il faut donc les envoyer à l'aide d'un execl
        char temp; //char qui contient le car lu actuellement
        short n = 0;
        int nb = 0;
        int bitmap_offset = 0;

        int taille_sans_extension = strlen(argv[2]) - 4;
        char *nom_fichier_copie_grise = malloc(sizeof(char) * (taille_sans_extension + 9)); //-4 = .bmp, on met la taille de nom_fichier_copie_grise pour accueillir les données que rentre l'utilisateur sans l'extension .bmp
        char *nom_fichier_copie_bleue = malloc(sizeof(char) * taille_sans_extension);
        char *nom_fichier_copie_verte = malloc(sizeof(char) * taille_sans_extension);
        char *nom_fichier_copie_rouge = malloc(sizeof(char) * taille_sans_extension);

        switch (ret = fork())
        {
        case (pid_t)-1:
            perror("creation impossible\n");
            exit(1);
        case (pid_t)0: //Bloc du fils
            printf("%s\n", argv[2]);
            strncpy(nom_fichier_copie_grise, argv[2], taille_sans_extension); //On copie un nombre de  strlen(argv[2]) - 4 char de argv[2] dans nom_fichier_copie_grise
            strcat(nom_fichier_copie_grise, "_gris.bmp");                     //On rajoute "_gris.bmp" au nom de la copie.
            printf("%s\n", nom_fichier_copie_grise);
            execl("./recouvrant", "recouvrant", nom_fichier_copie_grise, tube, flag, NULL);
            perror("Attention, impossible d'effectuer le recouvrement !\n");
            exit(2);
            break;
        default: //Bloc du père
            switch (ret = fork())
            {
            case (pid_t)-1:
                perror("creation impossible\n");
                exit(1);
            case (pid_t)0:                                                        //Bloc du fils
                strncpy(nom_fichier_copie_bleue, argv[2], taille_sans_extension); //On copie un nombre de  strlen(argv[2]) - 4 char de argv[2] dans nom_fichier_copie_grise
                strcat(nom_fichier_copie_bleue, "_bleu.bmp");                     //On rajoute "_gris.bmp" au nom de la copie.
                printf("%s\n", nom_fichier_copie_bleue);
                execl("./recouvrant_bleu", "recouvrant_bleu", nom_fichier_copie_bleue, tube_bleu, flag_bleu, NULL);
                perror("Attention, impossible d'effectuer le recouvrement !\n");
                exit(2);
                break;
            default: //Bloc du père
                switch (ret = fork())
                {
                case (pid_t)-1:
                    perror("creation impossible\n");
                    exit(1);
                case (pid_t)0:                                                        //Bloc du fils
                    strncpy(nom_fichier_copie_verte, argv[2], taille_sans_extension); //On copie un nombre de  strlen(argv[2]) - 4 char de argv[2] dans nom_fichier_copie_grise
                    strcat(nom_fichier_copie_verte, "_verte.bmp");                    //On rajoute "_gris.bmp" au nom de la copie.
                    printf("%s\n", nom_fichier_copie_verte);
                    execl("./recouvrant_vert", "recouvrant_vert", nom_fichier_copie_verte, tube_vert, flag_vert, NULL);
                    perror("Attention, impossible d'effectuer le recouvrement !\n");
                    exit(2);
                    break;
                default: //Bloc du père
                    switch (ret = fork())
                    {
                    case (pid_t)-1:
                        perror("creation impossible\n");
                        exit(1);
                    case (pid_t)0:                                                        //Bloc du fils
                        strncpy(nom_fichier_copie_rouge, argv[2], taille_sans_extension); //On copie un nombre de  strlen(argv[2]) - 4 char de argv[2] dans nom_fichier_copie_grise
                        strcat(nom_fichier_copie_rouge, "_rouge.bmp");                    //On rajoute "_gris.bmp" au nom de la copie.
                        printf("%s\n", nom_fichier_copie_rouge);
                        execl("./recouvrant_rouge", "recouvrant_rouge", nom_fichier_copie_rouge, tube_rouge, flag_rouge, NULL);
                        perror("Attention, impossible d'effectuer le recouvrement !\n");
                        exit(2);
                        break;

                    default:            //Bloc du père
                        close(tube[0]); //On doit fermer les channels non utilisées
                        close(tube_bleu[0]);
                        close(tube_vert[0]);
                        close(tube_rouge[0]);
                        close(flag[1]);
                        close(flag_bleu[1]);
                        close(flag_vert[1]);
                        close(flag_rouge[1]);

                        int desc = open(argv[1], O_RDONLY);
                        //On gère l'erreur de si il n'arrive pas à ouvrir le fichier
                        if (desc == -1)
                        {
                            perror("Attention, on ne peut pas ouvrir le fichier!");
                            exit(1);
                        }

                        //On nomme la copie selon ce que l'utilisateur rentre dans la ligne de commande : ./stat classic.bmp copie_classic.bmp
                        int desc2 = open(argv[2], O_WRONLY | O_CREAT, S_IRWXU);
                        if (desc2 == -1)
                        {
                            perror("Attention, on ne peut pas créer ou l'ouvrir!");
                            exit(1);
                        }

                        //On regarde d'abord si le format du fichier est le bon. (on check les 2 premiers octets)
                        read(desc, &temp, sizeof(char));

                        if (temp != 'B')
                        {
                            perror("Attention, le fichier n'est pas au bon format! Le fichier doit être un fichier .bmp");
                            exit(1);
                        }

                        write(desc2, &temp, sizeof(char));
                        //On copie également dans le tube, pour donner le fichier au fils. (les octets du fichier 1 par 1)
                        write(tube[1], &temp, sizeof(char));
                        write(tube_bleu[1], &temp, sizeof(char));
                        write(tube_rouge[1], &temp, sizeof(char));
                        write(tube_vert[1], &temp, sizeof(char));
                        read(desc, &temp, sizeof(char));

                        if (temp != 'M')
                        {
                            perror("Attention, le fichier n'est pas au bon format! Le fichier doit être un fichier .bmp");
                            exit(1);
                        }
                        write(desc2, &temp, sizeof(char));
                        write(tube[1], &temp, sizeof(char));
                        write(tube_bleu[1], &temp, sizeof(char));
                        write(tube_rouge[1], &temp, sizeof(char));
                        write(tube_vert[1], &temp, sizeof(char));
                        printf("Le fichier est bien en bmp!\n");

                        short nb_bits_pixels = 0;
                        int largeur = 0;
                        int hauteur = 0;

                        for (int i = 0; i < 9; ++i) //On veut aller jusqu'à la 12éme ligne du tableau (comprise)
                        {
                            switch (i)
                            {
                            case 2: //On récupère l'offset de l'image (utilise pour les niveaux de gris)
                                read(desc, &bitmap_offset, sizeof(int));
                                write(desc2, &bitmap_offset, sizeof(int));   //Fichier de copie
                                write(tube[1], &bitmap_offset, sizeof(int)); //Tube pr donner les informations au fils
                                write(tube_bleu[1], &bitmap_offset, sizeof(int));
                                write(tube_rouge[1], &bitmap_offset, sizeof(int));
                                write(tube_vert[1], &bitmap_offset, sizeof(int));
                                break;
                            case 4:
                                read(desc, &largeur, sizeof(int));
                                printf("Largeur : %d pixels\n", largeur);
                                write(desc2, &largeur, sizeof(int));
                                write(tube[1], &largeur, sizeof(int));
                                write(tube_bleu[1], &largeur, sizeof(int));
                                write(tube_rouge[1], &largeur, sizeof(int));
                                write(tube_vert[1], &largeur, sizeof(int));
                                break;
                            case 5:
                                read(desc, &hauteur, sizeof(int));
                                printf("Hauteur : %d pixels\n", hauteur);
                                write(desc2, &hauteur, sizeof(int));
                                write(tube[1], &hauteur, sizeof(int));
                                write(tube_bleu[1], &hauteur, sizeof(int));
                                write(tube_rouge[1], &hauteur, sizeof(int));
                                write(tube_vert[1], &hauteur, sizeof(int));
                                break;
                            case 6:
                                read(desc, &n, sizeof(short)); //On passe le premier short
                                write(desc2, &n, sizeof(short));
                                write(tube[1], &n, sizeof(short));
                                write(tube_bleu[1], &n, sizeof(short));
                                write(tube_rouge[1], &n, sizeof(short));
                                write(tube_vert[1], &n, sizeof(short));
                                break;
                            case 7:
                                read(desc, &nb_bits_pixels, sizeof(short)); //On affiche le second short
                                printf("Nombre de bits par pixel : %hd\n", nb_bits_pixels);
                                write(desc2, &nb_bits_pixels, sizeof(short));
                                write(tube[1], &nb_bits_pixels, sizeof(short));
                                write(tube_bleu[1], &nb_bits_pixels, sizeof(short));
                                write(tube_rouge[1], &nb_bits_pixels, sizeof(short));
                                write(tube_vert[1], &nb_bits_pixels, sizeof(short));
                                break;
                            case 8:
                                read(desc, &nb, sizeof(int)); //On regarde si le dossier est compressé
                                write(desc2, &nb, sizeof(int));
                                write(tube[1], &nb, sizeof(int));
                                write(tube_bleu[1], &nb, sizeof(int));
                                write(tube_rouge[1], &nb, sizeof(int));
                                write(tube_vert[1], &nb, sizeof(int));
                                if (nb == 0)
                                {
                                    printf("Le fichier n'est pas compressé !\n");
                                }
                                else
                                {
                                    printf("Le fichier est compressé !\n");
                                }
                                break;
                            default:
                                read(desc, &nb, sizeof(int)); //On passe les int
                                write(desc2, &nb, sizeof(int));
                                write(tube[1], &nb, sizeof(int));
                                write(tube_bleu[1], &nb, sizeof(int));
                                write(tube_rouge[1], &nb, sizeof(int));
                                write(tube_vert[1], &nb, sizeof(int));
                                break;
                            }
                        }
                        //On récupère la postion actuelle du père
                        int position_actuelle = lseek(desc, 0, SEEK_CUR); //On initialise le curseur à la position actuelle + 0 octets dans le fichier desc.
                        //On envoie la postion du père au fils par le tube juste après la compression
                        write(tube[1], &position_actuelle, sizeof(int));
                        write(tube_bleu[1], &position_actuelle, sizeof(int));
                        write(tube_rouge[1], &position_actuelle, sizeof(int));
                        write(tube_vert[1], &position_actuelle, sizeof(int));
                        //On recopie le reste du fichier.
                        while (read(desc, &temp, sizeof(char)) > 0) //1 car on prend les char 1 par 1
                        {
                            write(desc2, &temp, sizeof(char));
                            write(tube[1], &temp, sizeof(char));
                            write(tube_bleu[1], &temp, sizeof(char));
                            write(tube_rouge[1], &temp, sizeof(char));
                            write(tube_vert[1], &temp, sizeof(char));
                        }

                        close(desc);
                        close(desc2);
                        close(tube[1]); //On ferme les channels utilisés
                        close(tube_bleu[1]);
                        close(tube_vert[1]);
                        close(tube_rouge[1]);
                        read(flag[0], message, sizeof(message)); //Si il n'y a rien d'écrit, on attend (on att un accusé de réception), On met ce qu'on à lu dans "message"
                        read(flag_bleu[0], message, sizeof(message));
                        read(flag_rouge[0], message, sizeof(message));
                        read(flag_vert[0], message, sizeof(message));
                        close(flag[0]);
                        close(flag_bleu[0]);
                        close(flag_rouge[0]);
                        close(flag_vert[0]);
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