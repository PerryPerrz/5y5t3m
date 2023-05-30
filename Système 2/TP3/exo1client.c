#include "exo1fichier.h"

int main(int argc, char *argv[])
{
    sleep(1);//On attend que le serveur crée la file de message

    printf("Client (%d) : ", getpid());
    fflush(stdout);

    /* Récupération de la clef */
    key_t cle = ftok(PATH, ID);

    /* Récupération de la file de messages */
    int mes_id = msgget(cle, 0);

    if (mes_id == -1)
    {
        perror("Erreur dans la réception de la file de messages");
        exit(-1);
    }

    /* Utilisation de la file */

    /*Envoi du message */
    message msg_client;
    msg_client.mtype = 1;
    msg_client.valeur = atoi(argv[1]);

    if (msgsnd(mes_id, &msg_client, sizeof(message) - sizeof(long), 0) == -1)
    {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }

    /*Réception du message */
    message msg_recu;

    if (msgrcv(mes_id, &msg_recu, sizeof(message) - sizeof(long), 2, 0) == -1) // Message de type 2
    {
        perror("Erreur dans la réception du message");
        exit(3);
    }

    printf("%s * 2 = %d\n",argv[1],msg_recu.valeur);
}