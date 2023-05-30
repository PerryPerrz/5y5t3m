#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char temp; //char qui contient le car lu actuellement
    short n = 0;
    int nb = 0;
    int bitmap_offset = 0;
    short nb_bits_pixels = 0;
    int largeur = 0;
    int hauteur = 0;
    char *message = "Papa ! J'ai fini !\n";

    close(argv[2][1]); //On doit fermer les channels non utilisées, tube = argv[2] et flag = argv[3]
    close(argv[3][0]);

    //On s'occupe des niveaux de rouge.
    int position_actuelle = 0;

    int desc3 = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU);
    if (desc3 == -1)
    {
        perror("Attention, on ne peut pas créer ou l'ouvrir!");
        exit(1);
    }

    //On lit le argv[2]
    read(argv[2][0], &temp, sizeof(char)); //On lit le B
    write(desc3, &temp, sizeof(char));     //On écrit le B
    read(argv[2][0], &temp, sizeof(char)); //On lit le M
    write(desc3, &temp, sizeof(char));     //On écrit le M

    for (int i = 0; i < 9; ++i) //On veut aller jusqu'à la 12éme ligne du tableau (comprise)
    {
        switch (i)
        {
        case 2: //On récupère l'offset de l'image (utilise pour les niveaux de rouge)
            read(argv[2][0], &bitmap_offset, sizeof(int));
            write(desc3, &bitmap_offset, sizeof(int));
            break;
        case 4: //on récupère la largeur
            read(argv[2][0], &largeur, sizeof(int));
            write(desc3, &largeur, sizeof(int));
            break;
        case 5: //on récupère la hauteur
            read(argv[2][0], &hauteur, sizeof(int));
            write(desc3, &hauteur, sizeof(int));
            break;
        case 6: //On affiche le second short
            read(argv[2][0], &n, sizeof(short));
            write(desc3, &n, sizeof(short));
            break;
        case 7: //On récupère le nombre de bits par pixel.
            read(argv[2][0], &nb_bits_pixels, sizeof(short));
            write(desc3, &nb_bits_pixels, sizeof(short));
            break;
        default: //On passe les int
            read(argv[2][0], &nb, sizeof(int));
            write(desc3, &nb, sizeof(int));
            break;
        }
    }
    if (nb_bits_pixels != 24)
    {
        perror("Attention, le fichier doit être un fichier BMP 24 bits !");
        exit(1);
    }

    read(argv[2][0], &position_actuelle, sizeof(int));
    for (int i = position_actuelle; i < bitmap_offset; ++i)
    {
        read(argv[2][0], &temp, sizeof(char));
        write(desc3, &temp, sizeof(char));
    }

    int nb_0_a_rajouter = 0; //Nombre de 0 à rajouter pour que le nombre d'octets de la ligne de pixels soit bien un multiple de 4. (et que se soit bien des pixels pleins (3 octets par pixel))
    while ((largeur * 3 + nb_0_a_rajouter) % 3 != 0 && (largeur * 3 + nb_0_a_rajouter) % 4 != 0)
    { //Tant que l'on ne rajoute pas assez de 0 pour que se soit une ligne de pixels pleins dont le nombre d'octet est un multiple de 4.
        nb_0_a_rajouter += 1;
    }

    unsigned char pixel_b, pixel_v, pixel_r, nouveau_pixel_b, nouveau_pixel_v;
    for (int j = 0; j < hauteur; ++j) //On parcours les lignes en hauteur
    {
        for (int i = 0; i < largeur; ++i) //On s'occupe des pixels d'une ligne de la bitmap
        {
            //On récupère les 3 octets d'un pixel
            read(argv[2][0], &pixel_b, sizeof(unsigned char));
            read(argv[2][0], &pixel_v, sizeof(unsigned char));
            read(argv[2][0], &pixel_r, sizeof(unsigned char));

            //On les mets en rouge
            nouveau_pixel_b = 0;
            nouveau_pixel_v = 0;

            //On les mets dans la copie rouge
            write(desc3, &nouveau_pixel_b, sizeof(unsigned char));
            write(desc3, &nouveau_pixel_v, sizeof(unsigned char));
            write(desc3, &pixel_r, sizeof(unsigned char));
        }
        /*
        //On rajoute les 0 à la fin de chaque ligne de pixel
        unsigned char tp = 0;
        for (int k = 0; k < nb_0_a_rajouter; k++)
        {
            if (write(desc3, &tp, sizeof(char)) == -1)
            {
                perror("OK");
                exit(1);
            }
        }*/
    }
    printf("La copie rouge du fichier à bien été réalisée !\n");
    close(argv[2][0]);
    close(desc3);

    write(argv[3][1], message, sizeof(message));
    close(argv[2][0]); //On ferme les channels utilisés
    close(argv[3][1]);
    return EXIT_SUCCESS;
}