#include "serveur.h"
#include "mes_semaphores.h"

int main(int argc, char *argv[])
{
    srand(getpid());

    int *p_variable_partagee; // pointera sur la case mémoire partagée

    // Récupération de la clé
    key_t key = ftok(PATH, ID);

    // creation d'un segment de memoire partagée privé
    fflush(stdout);
    int mem_id = shmget(key, 1 * sizeof(int *), IPC_CREAT | 0660);
    if (mem_id == -1)
    {
        perror("Erreur création segment memoire partagé");
        exit(1);
    }

    // Attachement du segment pour pouvoir l'utiliser
    p_variable_partagee = (int *)shmat(mem_id, NULL, 0);

    if (p_variable_partagee == NULL)
    {
        perror("Erreur d'attachement au segment memoire partagé");
        exit(2);
    }

    int num_pilote = atoi(argv[1]); // On récupère le numéro du pilote actuel donné par l'utilisateur

    int *semid = malloc(sizeof(int));
    sem_recup(semid, 2);

    P(*semid, PROTECTION);

    if (p_variable_partagee[num_pilote] < N && p_variable_partagee[num_pilote] >= 0)
    {
        p_variable_partagee[num_pilote] += rand() % 5 + 1;
    }
    else
    {
        p_variable_partagee[num_pilote] = 0;
    }

    V(*semid, PROTECTION);

    P(*semid, A_JOUE);

    printf("Le pilote : %d\n", num_pilote);
    fflush(stdout);
}