#include "mes_semaphores.h"

/* qui crée un ensemble de sémaphores (semid) de nombre_semaphores . Elle renvoie 0 si la création a bien fonctionné. Elle sera utilisée par le père .*/
int sem_creation(int *semid, int nombre_semaphores)
{
    int key = ftok("./fichier_qui_existe_deja.txt", 5);
    *semid = semget(key, nombre_semaphores, IPC_CREAT | 0660 | IPC_EXCL); // Semget renvoie un truc positif qd ça marche et -1 qd ça marche pas
    return *semid;
}

/*initialise le semaphore numéro num_semaphore de l'ensemble de sémaphore semid par la valeur nbr_jetons .*/
int sem_initialisation(int semid, int num_semaphore, int nbr_jetons)
{
    semval initsem;
    initsem.val = nbr_jetons; // le nombre de jeton
    return semctl(semid, num_semaphore, SETVAL, initsem);
}

/* Primitive P pour le sémaphore numéro num_semaphore . (pour cet exercice le numéro sera toujours 0 mais nous réutiliserons ces fonctions dans l'exercice suivant)*/
int P(int semid, int num_semaphore)
{
    /* P sur le sémaphore numéro 2 dans un ensemble de sémphore d'ID semid */
    struct sembuf smbf;
    smbf.sem_num = num_semaphore;
    smbf.sem_op = -1; //+1 pour V
    smbf.sem_flg = 0;
    return semop(semid, &smbf, 1);
}

/* Primitive V pour le sémaphore numéro num_semaphore .*/
int V(int semid, int num_semaphore)
{
    /* V sur le sémaphore numéro 2 dans un ensemble de sémphore d'ID semid */
    struct sembuf smbf;
    smbf.sem_num = num_semaphore;
    smbf.sem_op = +1; //-1 pour P
    smbf.sem_flg = 0;
    return semop(semid, &smbf, 1);
}

/* qui détruit un ensemble de sémaphores (semid). Elle sera utilisée par le père .*/
int sem_destruction(int semid)
{
    return semctl(semid, IPC_RMID, 0);
}

/* qui récupère le numéro de l'ensemble de sémaphores (semid) (généralement utilisée par le processus le fils, mais ici nous ne l'utiliserons pas puisque nous n'utilisons pas execl ...mais faites-là quand même :-) elle servira dans le prochain exercice). En entrée, il faut indiquer le nombre de sémaphore qu'il y a dans l'ensemble à récupérer. La fonction est identique à la création sauf pour l'option de semget qui doit être à 0.*/
int sem_recup(int *semid, int nb_semaphores)
{
    int key = ftok("./mes_semaphores", 5);
    *semid = semget(key, nb_semaphores, 0);
    return *semid;
}