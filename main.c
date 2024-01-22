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
int Alloc_Block(TOF *5)
{
    AFF_ENTETE(S,1,ENTETE(S,1)+1);
    return ENTETE(S,1);
}




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

void supprimer(TOF *s,enregistrement key)
    {
       int i,j,Trouv;
       bloc Buf;
       Recherche_DEC_TOF(S,key,&i,&j,&Trouv);
       if (Trouv==1)
       {
        LireDir(S,i,&Buf);
        if (Buf.nb!=1)
          {
            while(j<Buf.nb)
            {
            
                Buf.tab[j]=Buf.tab[j+1];
                j++;
            }
          }
          Buf.nb--;
          EcrireDir(S,i,Buf);
          AFF_ENTETE(S,i,ENTETE(S,2)-1);

       }

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


void Insertion_TOF(TOF *S,int key)
 {   
    Tbloc Buf;
    int i,j,Trouv=faux,Temp,k,Arret=faux;
     Recherche_DEC_TOF(S,key,&i,&j,&Trouv);
     if(!Trouv){
        while((!Arret)&&(i<=ENTETE(S,1)))
         {
             LireDir(S,i,&Buf); k=Buf.nb-1;
             Temp=Buf.Tab[k];
             while(k>j)
             {
                Buf.Tab[k]=Buf.Tab[k-1];
                 k--;

             }
             Buf.Tab[j]=key;
             if (Buf.nb<max)
             {
                Buf.nb=Buf.nb+1;Buf.Tab[Buf.nb-1]=Temp;
                 EcrireDir(S,i,Buf);Arret=vrai;
             }
             else{EcrireDir(S,i,Buf);i++;j=0;key=Temp;}
         }
         if(i>ENTETE(S,1))
         {
             Buf.Tab[0]=key;Buf.nb=1;
             EcrireDir(S,i,Buf);
             Aff_ENTETE(S,1,i);
         }
     Aff_ENTETE(S,2,ENTETE(S,2)+1);
     }
     }



// une fonction qui affiche les données d'un etudiant 
void afficherEtudiant(enregistrement e){
    printf("le nom :%s / le prenom : %s / le matricule : %d \n",e.nom,e.prenom,e.matricule);
}


int main(){

    return 0;
}