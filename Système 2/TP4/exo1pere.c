#include "mes_semaphores.h"
#include <sys/wait.h>
#include <sys/shm.h>

pid_t fork();

int main(int argc, char *argv[])
{
    int *p_variable_partagee; // Pointera sur la case mémoire partagée
    int key = IPC_PRIVATE;
    int tmp1 = 0;
    int tmp2 = 0;

    /* Création d'un segment de memoire partagée privé */
    int mem_id = shmget(key, 1 * sizeof(int), IPC_CREAT | 0660);
    if (mem_id == -1)
    {
        perror("Erreur creation segment memoire partagé");
        exit(1);
    }
    /* Attachement du segment pour pouvoir l'utiliser */
    p_variable_partagee = (int *)shmat(mem_id, NULL, 0); // shmat return un void*, suffit de le cast
    if (p_variable_partagee == NULL)
    {
        perror("Erreur d'attachement au segment memoire partagé");
        exit(2);
    }

    /*Utilisation classique*/
    *p_variable_partagee = 0;

    // Création du sémaphore.
    int num_semaphore = 0; // On crée un ensemble de sémaphore, soit un tableau de sém et la première valeur d'un tableau est à l'indice 0
    int *semid = malloc(sizeof(int));

    if (sem_creation(semid, 1) == -1)
    {
        perror("Erreur lors de la création du semaphore");
        exit(1);
    }

    // Initialisation du sémaphore.
    if (sem_initialisation(*semid, num_semaphore, 1) == -1)
    {
        perror("Erreur lors de l'initialisation du sémaphore");
        exit(1);
    }

    pid_t ret;
    switch (ret = fork())
    {
    case (pid_t)-1:
        perror("creation impossible\n");
        exit(1);
    case (pid_t)0: // Bloc du fils
        for (int i = 0; i < 100000; ++i)
        {
            P(*semid, num_semaphore); // Pour pas que le père et le fils écrivent sur la variable partagée en même temps.

            tmp1 = (*p_variable_partagee);
            tmp1 = tmp1 + 5;
            (*p_variable_partagee) = tmp1;

            V(*semid, num_semaphore);
        }
        break;
    default: // Bloc du père
        for (int i = 0; i < 100000; ++i)
        {
            P(*semid, num_semaphore);

            tmp2 = (*p_variable_partagee);
            tmp2 = tmp2 + 3;
            (*p_variable_partagee) = tmp2;

            V(*semid, num_semaphore);
        }

        // Le père attend le fils
        waitpid(ret, NULL, 0);

        printf("Valeur du tmp : %d", *p_variable_partagee);
        fflush(stdout);

        /* Détachement*/
        shmdt(p_variable_partagee);
        /* Destruction*/
        shmctl(mem_id, IPC_RMID, 0);

        // Destruction des sémaphores
        sem_destruction(*semid);

        break;
    }
    exit(0);
}