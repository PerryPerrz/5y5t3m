//Open permet d'ouvrir un ficher, open retourne le descripteur du fichier
//WriteOnly, Create et la disjonction des 2

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//Les valeurs 1 et 512 sont incluent dans fcntl.h définie dans notre version de C
//0_WRONLY : J'ouvre un fichier en écriture uniquement, pas en lecture
//Desci lien entre fichier et proc

int main(){
    printf("%d | %d = %d\n",0_WRONLY, 0_CREAT, 0_WRONLY | 0_CREAT);
    int desc = open("truc.txt",0_WRONLY | 0_CREAT);
    printf("%d\n",desc); //Descripteur dépend du proc et du fichier (0,1,2 occup par entrée sortie erreur) donc ici ce devrait être 3
    printf("%d\n",close(desc)):
    close(desc); //Quand on ouvre un fichier, on le ferme
    return 0;
}
//Quand je l'ouvre, il va rajouter un entier dans la table des desc (3 ici), je ferme le fichier en donnant close via son desc