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
    int nbrElSupp; // nombre d'enregistrement effacé logiquement
}entete;
// puis on definit le fichier
typedef struct TOF{
    entete tete;
    FILE *f
}TOF;
///****************les algorithmes de manipulation des fichiers*********************//////
/// Ouvrir le fichier logique et l’associer au fichier physique en précisant si le fichier est nouveau ('N') ou ancien ('A').
TOF* ouvrir(char fileName[30],char mode){
    TOF* fileTOF=malloc(sizeof(TOF));
        if( mode=='a'|| mode=='A'){
            fileTOF->f=fopen(fileName,"rb+");
            fread(&(fileTOF->tete),sizeof(entete),1,fileTOF->f);
        }
        else{
            fileTOF->f=fopen(fileName,"wb+");
            (fileTOF->tete).nbrBlocs=(fileTOF->tete).nbrEl=(fileTOF->tete).nbrElSupp=0;/// on initialise le nbr de blocs et d'enregistrement effacées et non effacé á 0 
            fwrite(&(fileTOF->tete),sizeof(entete),1,fileTOF->f); /// on copie l'entete dans notre fichier
        }
    return fileTOF;
}
// Fermer le fichier.
void fermer(TOF *F){
    rewind(F->f);// on se met dans le debut du fichier
    fwrite(&(F->tete),sizeof(F->tete),1,F->f);// on update l'entete dans le fichier logique (pour savoir avec quoi nous travaillons plus tard)
    fclose(F->f);
    F=NULL;
}

// une fonction qui affiche les données d'un etudiant 
void afficherEtudiant(enregistrement e){
    printf("le nom :%s / le prenom : %s / le matricule : %d \n",e.nom,e.prenom,e.matricule);
}


int main(){

    
    return 0;
}