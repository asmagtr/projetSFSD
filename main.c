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
typedef struct FICHIER {
    int nbrBloc; /// l'entete de fichier oú on vas garder le nuombres de blocs utilisés
    bloc blocs[max];
}FICHIER;
//

int main(){
    printf("hello wolrd!");
    
    return 0;
}