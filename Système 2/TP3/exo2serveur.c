#include "exo2fichier.h"

/*
Compiler :
    make
    ./exo2serveur & ./exo2client 18
*/

int nb = 0;

void handler_pour_signal(int sig)
{
    nb = 1;
}

int main(int argc, char *argv[])
{
    printf("Serveur (%d) : ", getpid());
    fflush(stdout);

    /* Création d'une clef */
    key_t cle = ftok(PATH, ID);

    /* Création de la file de messages */
    int mes_id = msgget(cle, IPC_CREAT | 0660);

    if (mes_id == -1)
    {
        perror("Erreur dans création de la file de messages");
        exit(-1);
    }

    printf("file de messages créée\n");
    fflush(stdout);

    /* Utilisation de la file */

    /* Réception du message */
    message msg_serveur;

    // Mise en place de la structure du handler
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));

    // Mise en fonctionnement du handler
    sigaction(SIGINT, &sa, NULL);

    while (nb != 1)
    {
        if (msgrcv(mes_id, &msg_serveur, sizeof(message) - sizeof(long), 1, 0) == -1) // Message de type 1 (Permet de différencier les messages envoyés)
        {
            perror("Erreur dans la réception du message");
            exit(3);
        }

        /* Création du message */
        message msg_accuse_de_reception;
        msg_accuse_de_reception.mtype = msg_serveur.pid;         // On envoi au client dont on à reçu le message juste avant
        msg_accuse_de_reception.valeur = msg_serveur.valeur * 2; // On envoie la valeur reçue x2
        msg_accuse_de_reception.pid = getpid();

        if (msgsnd(mes_id, &msg_accuse_de_reception, sizeof(message) - sizeof(long), 0) == -1)
        {
            perror("Erreur lors de l'envoi du message");
            exit(1);
        }
    }

    sleep(4); /* On laisse le temps au client de récupérer le message */

    /* Suppression de la file de messages*/
    msgctl(mes_id, IPC_RMID, 0);
}