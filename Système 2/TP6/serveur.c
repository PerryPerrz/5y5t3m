#include "serveur.h"
#include "mes_semaphores.h"

// Procédure qui initialise le circuit. (taille du circuit = 2 fois nb pilotes)
void init_circuit(int *tab)
{
    for (int i = 0; i <= (NBPILOTES * 2) - 1; ++i)
    {
        tab[i] = 0;
    }
}

// Procédure qui affiche le circuit.
void afficher_circuit(int *tab)
{
    for (int i = 0; i <= NBPILOTES - 1; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (j == tab[i])
            {
                printf("%d  ", i + 1);
            }
            else
            {
                printf("0  ");
            }
        }
        printf("\n");
    }
    printf("\n");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    int *p_variable_partagee; // pointera sur la case mémoire partagée

    // Récupération de la clé
    key_t key = ftok(PATH, ID);

    // Création d'un segment de memoire partagée privé
    fflush(stdout);
    int mem_id = shmget(key, 1 * sizeof(int *), IPC_CREAT | 0660);
    if (mem_id == -1)
    {
        perror("Erreur création segment mémoire partagé");
        exit(1);
    }

    // Attachement du segment pour pouvoir l'utiliser
    p_variable_partagee = (int *)shmat(mem_id, NULL, 0);
    if (p_variable_partagee == NULL)
    {
        perror("Erreur d'attachement au segment mémoire partagé");
        exit(2);
    }

    // Initialisation du circuit
    init_circuit(p_variable_partagee);

    // On attend 3 secondes.
    // sleep(3);

    // Création et initialisation des sémaphores.
    int *semid = malloc(sizeof(int));

    if (sem_creation(semid, 2) == -1)
    {
        perror("Erreur lors de la création du sémaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_initialisation(*semid, PROTECTION, 1) == -1)
    {
        perror("Erreur lors de l'initialisation du sémaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_initialisation(*semid, A_JOUE, 0) == -1)
    {
        perror("Erreur lors de l'initialisation du sémaphore");
        exit(EXIT_FAILURE);
    }

    P(*semid, PROTECTION);

    afficher_circuit(p_variable_partagee);

    V(*semid, PROTECTION);

    int nb = 0;
    while (nb < NBPILOTES)
    {
        if (V(*semid, A_JOUE))
        {
            printf("serveur : %d\n", nb - 1);
            nb += 1;
            fflush(stdout);
        }
    }

    P(*semid, PROTECTION);

    afficher_circuit(p_variable_partagee);

    V(*semid, PROTECTION);

    // Détachement
    shmdt(p_variable_partagee);
    // Destruction
    shmctl(mem_id, IPC_RMID, 0);
    // Destruction des semaphores
    sem_destruction(*semid);
}