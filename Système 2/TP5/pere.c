#include "mes_semaphores.h"
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>

// Procédure qui s'occupe de remplir le tableau.
void remplissage_tableau(int taille, int *tab)
{
    /* Initialisation du générateur */
    srand(getpid());
    /* génération d'un nombre */

    for (int i = 0; i < taille; ++i)
    {
        tab[i] = rand() % 100; /*ici entre 0 et 100 (exclus)*/
    }
}

// Procédure qui affiche le tableau.
void afficher_tab(int taille, int *tab)
{
    printf("(");
    for (int i = 0; i < taille - 1; ++i)
    {
        printf("%d, ", tab[i]);
    }
    printf("%d)\n", tab[taille - 1]);
    fflush(stdout);
}

// Fonction qui trouve le minimum dans un intervalle de valeurs données.
int minimum(int borne_inf, int borne_sup, int *tab)
{
    int minimum = borne_inf;
    for (int i = borne_inf; i <= borne_sup; ++i)
    {
        if (tab[i] < tab[minimum])
        {
            minimum = i; // On renvoie la position du minimum dans le tableau
        }
    }

    return minimum;
}

// Fonction qui trouve le maximum dans un intervalle de valeurs données.
int maximum(int borne_inf, int borne_sup, int *tab)
{
    int maximum = borne_inf; // Indice de la valeur maximum du tableau
    for (int i = borne_inf; i <= borne_sup; ++i)
    {
        if (tab[i] > tab[maximum])
        {
            maximum = i;
        }
    }

    return maximum;
}

// Procédure qui échange les 2 valeurs du tableau.
void echange_valeurs(int indice, int *tab)
{
    int temp = tab[indice];
    tab[indice] = tab[0];
    tab[0] = temp;
}

// Variable qui vaut 1 lorsque le fils à fini. (pour indiquer au père que le fils à fini).
int boolean_le_fils_a_fini = 0;

/* handler pour SIGCHLD */
void handler_pour_signal(int sig)
{
    boolean_le_fils_a_fini = 1;
}

pid_t fork();
int main(int argc, char *argv[])
{
    int *p_variable_partagee; // Pointera sur la case mémoire partagée
    int key = IPC_PRIVATE;

    /* Préparation de la structure */
    struct sigaction sa;
    sa.sa_handler = handler_pour_signal;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    /* Installation effective du handler */
    sigaction(SIGCHLD, &sa, NULL);
    /* à partir de maintenant si un signal arrive, il fera déclencher le nouvel handler*/

    /* Création d'un segment de memoire partagée privé */
    int mem_id = shmget(key, 1 * sizeof(int), IPC_CREAT | 0660);
    if (mem_id == -1)
    {
        perror("Erreur création segment mémoire partagé");
        exit(1);
    }
    /* Attachement du segment pour pouvoir l'utiliser */
    p_variable_partagee = (int *)shmat(mem_id, NULL, 0); // shmat return un void*, il suffit de le cast
    if (p_variable_partagee == NULL)
    {
        perror("Erreur d'attachement au segment mémoire partagé");
        exit(2);
    }

    /*Utilisation classique*/
    int n;

    // Si l'utilisateur ne rentre pas d'arguments (argc = 2 : il met un argument, argc = 1 : il n'en donne pas), je met une taille de tableau par défaut.
    if (argc != 2)
    {
        printf("Vous n'avez pas rentré d'argument, la valeur par défaut est de 10.\n");
        n = 10; //On initialise la valeur par défaut à 10.
    }
    else
    {
        // On initialise la taille du tableau à 2 fois la valeur que l'utilisateur rentre.
        n = atoi(argv[1]) * 2;
    }

    remplissage_tableau(n, p_variable_partagee);

    printf("Tableau d'origine : ");
    fflush(stdout);
    afficher_tab(n, p_variable_partagee);

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
        // Tant que le tableau n'est pas trié : le min de droite est plus petit que le maximum de gauche
        // On stop quand le min de droite est >= au max de gauche.
        printf("Je suis en C et je n'ai pas le droit d'avoir une déclaration en première ligne de code dans un case.\n");
        fflush(stdout);

        int booleen_echange_fini = 0; // 1 = vrai, le reste est faux
        while (booleen_echange_fini != 1)
        {
            P(*semid, num_semaphore); // Pour pas que le père et le fils écrivent sur la variable partagée en même temps.

            int min = minimum(n / 2, n - 1, p_variable_partagee);

            // Si le min de droite est >= au max de gauche on stop l'échange.
            if (p_variable_partagee[min] >= p_variable_partagee[0])
            {
                booleen_echange_fini = 1;
            }
            else
            {
                // Le fils attend 5 secondes avant de rendre la sémaphore.
                // sleep(5);
                // On échange les valeurs : le min va dans la dernière du tableau.

                printf("Tableau avant échange : ");
                afficher_tab(n, p_variable_partagee);

                echange_valeurs(min, p_variable_partagee);

                printf("Tableau après échange : ");
                fflush(stdout);
                afficher_tab(n, p_variable_partagee);
            }

            // printf("min = %d\n", min);
            // fflush(stdout);

            V(*semid, num_semaphore);
        }
        break;
    default: // Bloc du père

        while (boolean_le_fils_a_fini != 1)
        {
            P(*semid, num_semaphore);

            int max = maximum(0, (n / 2) - 1, p_variable_partagee);
            // printf("max = %d\n", max);
            // fflush(stdout);

            // Le père attend 2 secondes avant de rendre la sémaphore.
            // sleep(2);

            // On échange les valeurs : le max va dans la première case du tableau.
            echange_valeurs(max, p_variable_partagee);

            // afficher_tab(n, p_variable_partagee);

            V(*semid, num_semaphore);
        }

        // Le père attend le fils
        // waitpid(ret, NULL, 0);

        printf("Tableau final : ");
        fflush(stdout);
        afficher_tab(n, p_variable_partagee);

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