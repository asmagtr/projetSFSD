#include <stdio.h>
#include <stdlib.h>
#define max 10
/// on declare les types 
/// notre enregistrement represente un etudiant
typedef struct enregistremnt{
    char nom[20]; // le nom sera la clé
    char prenom[20];
    int matricule;
}enregistrement;

/// le type de bloc
typedef struct bloc{
    int nb;
    enregistrement tab[max];
}bloc;

//le type fichier
//d'abord on definit l'entete de fichier
typedef struct entete{
    int nbrBlocs;
}entete;
// puis on definit le fichier
typedef struct FICHIER{
    entete tete;
    bloc blocs[max];
}FICHIER;


int main(){


    return 0;
}