#ifndef CONST
#define CONST 0

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>

typedef union semun
{
    int val;               // valeur pour SETVAL
    struct semid_ds *buf;  // buffer pour IPC_SET et IPC_STAT
    unsigned short *array; // tableau pour GETALL et SETALL
    struct seminfo *__buf; // buffer pour IPC_INFO
} semval;

int sem_creation(int *semid, int nombre_semaphores);

int sem_initialisation(int semid, int num_semaphores, int nbr_jetons);

int P(int semid, int num_semaphores);

int V(int semid, int num_semaphores);

int sem_destruction(int semid);

int sem_recup(int *semid, int nb_semaphores);

#endif