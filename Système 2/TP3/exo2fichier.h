#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define PATH "fichier_commun_serveur_client.txt"
#define ID 6

typedef struct message
{
    long mtype; //Type du message
    int valeur; //Valeur du message
    int pid; //Pid
}message;