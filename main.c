#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 10
///***************************on declare les types**********************/
/// notre enregistrement represente un etudiant
typedef struct enregistremnt{
    bool supprimer; // pour montrer si l'element est supprimé logiquement 
    char nom[20]; // le nom sera la clé
    char prenom[20];
    int matricule;
}enregistrement;

/// le type de bloc
typedef struct bloc{
    int nb;
    enregistrement tab[max];
}bloc;
typedef struct bloc buffer;// le buffer est de meme type que le bloc

//le type fichier
//d'abord on definit l'entete de fichier
typedef struct entete{
    int nbrBlocs;
    int nbrEl; //nombre d'element (enregistrement)
}entete;
// puis on definit le fichier
typedef struct TOF{
    entete tete;
    FILE *f
}TOF;
///****************les algorithmes de manipulation des fichiers*********************//////



int main(){

    
    return 0;
}