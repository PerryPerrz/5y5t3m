#include "exo1fichier.h"

/*
Compiler :
    make
    ./exo1serveur & ./exo1client 18
*/

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

    if (msgrcv(mes_id, &msg_serveur, sizeof(message) - sizeof(long), 1, 0) == -1) // Message de type 1 (Permet de différencier les messages envoyés)
    {
        perror("Erreur dans la réception du message");
        exit(3);
    }

    /* Création du message */
    message msg_accuse_de_reception;
    msg_accuse_de_reception.mtype = 2;
    msg_accuse_de_reception.valeur = msg_serveur.valeur * 2; // On envoie la valeur reçue x2

    if (msgsnd(mes_id, &msg_accuse_de_reception, sizeof(message) - sizeof(long), 0) == -1)
    {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }

    sleep(4); /* On laisse le temps au client de récupérer le message */

    /* Suppression de la file de messages*/
    msgctl(mes_id, IPC_RMID, 0);
}