#include "exo2fichier.h"

int main(int argc, char *argv[])
{
    sleep(1); // On attend que le serveur crée la file de message

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
    message msg_client; //Création du message
    msg_client.mtype = 1;
    msg_client.valeur = atoi(argv[1]); //Pour l'exercice, valeur à envoyé au serveur.
    msg_client.pid = getpid();

    if (msgsnd(mes_id, &msg_client, sizeof(message) - sizeof(long), 0) == -1)
    {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }

    /*Réception du message */
    message msg_recu;

    if (msgrcv(mes_id, &msg_recu, sizeof(message) - sizeof(long), getpid(), 0) == -1) // Message de type getpid()
    {
        perror("Erreur dans la réception du message");
        exit(3);
    }

    printf("Client (%d) : %s * 2 = %d\n", getpid(), argv[1], msg_recu.valeur);
    fflush(stdout);
}