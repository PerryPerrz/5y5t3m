#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork();
pid_t ret;

int main(int argc, char *argv[])
{
    int cpt = 0;
    int nb_lettres = 0; //Compteur qui compte le nombre de lettres que lit le fils
    char *w = malloc(sizeof(char) * 1024);
    char c;
    int tube[2]; //Ce tube contient le nb de lettres | [1] : écrire, [0] : lire
    pipe(tube);  //On transforme le tableau tube de 2 entiers en pipe.
    int flag[2]; //On dit au père lorsque le fils à terminé, le fils écrit dedans une fois qu'il à fini.
    pipe(flag);
    char *message = "Papa ! J'ai fini !\n";

    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0:      //Bloc du fils
        close(tube[1]); //On doit fermer les channels non utilisées
        close(flag[0]);

        while (read(tube[0], &c, sizeof(char)) != 0) //On lit ce que le père à envoyé jusqu'à ce que le message soit lu entièrement.
        {
            if ('A' <= c && c <= 'Z')
            {
                w[cpt] = c;
                cpt++;
            }
            nb_lettres++;
        }
        printf("Nombre de lettres que lit le fils : %d\n",nb_lettres);
        write(flag[1], message, sizeof(message));
        close(tube[0]); //On ferme les channels utilisés
        close(flag[1]);
        execl("./recouvrant", "recouvrant", w, NULL);
        perror("Le recouvrement est impossible!\n");
        exit(1);
        break;
    default: //Bloc du père
        //On lit la chaine de car
        close(tube[0]); //On doit fermer les channels non utilisées
        close(flag[1]);
        while (argv[1][cpt] != '\0')
        {
            write(tube[1], &argv[1][cpt], sizeof(char));
            cpt++;
        }
        close(tube[1]); //On ferme les channels utilisés
        printf("Nb de lettre écrites : %d\n", cpt);
        read(flag[0], message, sizeof(message)); //Si il n'y a rien d'écrit, ça attend (on att un accusé de réception), On met ce qu'on à lu dans "message"
        printf("%s\n", message);
        close(flag[0]);
        break;
    }
    exit(0);
}