#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    FILE *f;
}TOF;

//les fonctions 

//fonction pour recuperer les donnees de bloc d'entete
int ENTETE(TOF *t,int i)
{
    if (i==1) return t->tete.nbrBlocs;
    if (i==2) return t->tete.nbrEl;
    if (i==3) return t->tete.nbrElSupp;
    
}

//fonction pour affecter des valeurs a l'entete
void AFF_ENTETE(TOF *t,int i,int val)
{
    if (i==1) t->tete.nbrBlocs=val;
    if (i==2) t->tete.nbrEl=val;
    if (i==3) t->tete.nbrElSupp=val;
}

void LireDir (TOF *t,int i,buffer *buf)
{
  if (i<=ENTETE(t,1))
    {
      fseek(t->f,sizeof(entete)+(i-1)*sizeof(bloc),0);
      fread(buf,sizeof(bloc),1,t->f);
      
    }
}

void EcrireDir (TOF *t,int i,buffer buf)
{
      fseek(t->f,sizeof(entete)+(i-1)*sizeof(bloc),0);
      fwrite(&buf,sizeof(bloc),1,t->f);
     
}

void Recherche_DEC_TOF(TOF *t, char *cle, int *i, int *j, bool *Trouv) {
    bloc Buf;
    int deb = 1, fin = ENTETE(t, 1), inf, sup, Arret = 0;
    *Trouv = false;
    *j = 0;

    while ((!(*Trouv)) && (deb <= fin) && (!Arret)) {
        *i = (deb + fin) / 2;
        LireDir(t, *i, &Buf);

        if (Buf.nb > 0) {
            if ((strcmp(cle, Buf.tab[0].nom) >= 0) && (strcmp(cle, Buf.tab[Buf.nb - 1].nom) <= 0)) {
                inf = 0;
                sup = Buf.nb - 1;

                while ((!(*Trouv)) && (inf <= sup)) {
                    *j = (inf + sup) / 2;
                    int compareResult = strcmp(cle, Buf.tab[*j].nom);

                    if (compareResult == 0) {
                        *Trouv = true;
                    } else {
                        if (compareResult < 0) {
                            sup = *j - 1;
                        } else {
                            inf = *j + 1;
                        }
                    }
                }

                if (!(*Trouv)) {
                    *j = inf;
                    Arret = true;
                }
            } else {
                if (strcmp(cle, Buf.tab[0].nom) < 0) {
                    fin = *i - 1;
                } else {
                    deb = *i + 1;
                }
            }
        } else {
            Arret = true;
        }
    }

    if (deb > fin) {
        *i = deb;
    }
}



int main(){

    return 0;
}