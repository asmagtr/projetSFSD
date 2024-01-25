#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define max 10
///***************************on declare les types**********************/
/// notre enregistrement represente un etudiant
typedef struct enregistremnt{
    bool supprimer; // pour montrer si l'element est supprimé logiquement
    char matricule[20];// le matricule sera la clé
    char nom[20];
    char prenom[20];

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
    char nomFichier[20];
    int nbrBlocs;
    int nbrEl; //nombre d'element (enregistrement)
    int nbrElSupp; // nombre d'enregistrement effacé logiquement
}entete;

// puis on definit le fichier
typedef struct TOF{
    entete tete;
    bloc tab[max];
}TOF;
//les fonctions
///****************les algorithmes de manipulation des fichiers*********************//////
/// Ouvrir le fichier logique et l’associer au fichier physique en précisant si le fichier est nouveau ('N') ou ancien ('A').
TOF ouvrir(char fileName[30],char mode)
{    TOF fileTOF;
      if(mode=='a'|| mode=='A'){
     FILE *f=fopen(fileName,"rb+");// on ouvre le fichier existant en mode lecture/ecriture
     // on lit l'entete;
     fread(&(fileTOF.tete),sizeof(entete),1,f);
     int nb_block=fileTOF.tete.nbrBlocs;
     for(int i=0;i<nb_block;i++){
        fread(&(fileTOF.tab[i]),sizeof(bloc),1,f); // on lit le bloc  num i
     }
      fclose(f);
    }else if(mode=='n'|| mode=='N'){
     FILE *f=fopen(fileName,"wb+");// on cree le fichier
       (fileTOF.tete).nbrBlocs=(fileTOF.tete).nbrEl=(fileTOF.tete).nbrElSupp=0;/// on initialise le nbr de blocs et d'enregistrement effacées et non effacé á 0
       strcpy((fileTOF.tete).nomFichier,fileName); // on sauvegarde le nom de fichier dans l'entete
        fclose(f);
    }

    return fileTOF;
}
void fermer(TOF fileTOF){

    FILE *f=fopen(fileTOF.tete.nomFichier,"wb+");// on ouvre le fichier deja existant
    // on recopie la tete;
    fwrite(&(fileTOF.tete),sizeof(entete),1,f);// on update l'entete dans le fichier logique (pour savoir avec quoi nous travaillons plus tard)
    int nb=fileTOF.tete.nbrBlocs;
    for(int i=0;i<nb;i++){
         fwrite(&(fileTOF.tab[i]),sizeof(bloc),1,f);
    }
    fclose(f);
    f=NULL;
}
//fonction pour affecter des valeurs a l'entete
void AFF_ENTETE(TOF *t,int i,int val)
{
    if (i==1)  t->tete.nbrBlocs=val;
    if (i==2)  t->tete.nbrEl=val;
    if (i==3)  t->tete.nbrElSupp=val;
}
//fonction pour recuperer les donnees de bloc d'entete
int ENTETE(TOF t,int i)
{
    if (i==1) return t.tete.nbrBlocs;
    if (i==2) return t.tete.nbrEl;
    if (i==3) return t.tete.nbrElSupp;

}
// une fonvtion qui copie le contenue d'un enregistrement dans un autre
void copyEnregistrement(enregistrement* destination, const enregistrement source)
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



// la fonction de suppression logique
void supprimerLogique(TOF *fichier,char* matricule){
int i,j;
bool trouv;
rechercheDichotomique(*fichier,matricule,&i,&j,&trouv);
if(trouv){
    // on vas mettre le bloc dans le buffer
    buffer buf;
    LireDir(*fichier,i,&buf);
    buf.tab[j].supprimer=true;
    AFF_ENTETE(fichier,3,ENTETE(*fichier,3)+1);// om incremente le nombre d'element supprimés
    EcrireDir(fichier,i,buf);// on recopie le contenue de buffer dans le bloc de fichier
}
}
// fonction qui cree un fichier
TOF creer(char* nomFichier , int n){
    TOF fichier=ouvrir(nomFichier,'n');
    int nbbloc;

    int i=0,j,k,l;
    bool trouv,suite=true;
    enregistrement etudiant;
    while(i<n && suite){
        do{
        printf("inserez les information du %d eme etudiant :\n",i+1);
        lireEnregistrement(&etudiant);
    
        rechercheDichotomique(fichier,etudiant.matricule,&j,&k,&trouv);
        
        if(trouv){
            printf("cet element existe deja, voulez vous inserer un autre (1 pour oui 0 pour non ):");
            scanf("%d",&suite);
            if(!suite){
                      
                break;
            }

        }

    }while(trouv && suite );
   
      i++;

    inserer(&fichier,etudiant.matricule,etudiant.nom,etudiant.prenom);
  
    printf("voulez vous inserer un autre (1 pour oui 0 pour non ):");
            scanf("%d",&suite);
}
   if(i%max==0){
        nbbloc=i/max;
    }else{
        nbbloc=(i/max)+1;
    }
    AFF_ENTETE(&fichier,1,nbbloc);
    AFF_ENTETE(&fichier,2,i);
    fermer(fichier);
}
void lireEnregistrement(enregistrement *etudiant){

    printf("\n entrez le matricule : ");
    scanf("%s",etudiant->matricule);
     printf("\n entrez le nom : ");
    scanf("%s",etudiant->nom);
     printf("\n entrez le prenom: ");
    scanf("%s",etudiant->prenom);
    etudiant->supprimer=false;

}
// une fonction qui affiche les données d'un etudiant 
void afficherEtudiant(enregistrement e){
    printf("le nom :%s / le prenom : %s / le matricule : %d \n",e.nom,e.prenom,e.matricule);
}


int main(){


    int choix,i,j;
    bool trouv;
    char nomFichier[30];
    char matricule[20];
    char nom[20];
    char prenom[20];
    TOF fichier;
    printf("vous voulez ?: \n 1/ creer un fichier \n 2/ inserer un etusiant \n 3/ chercher un etudiant \n 4/ supprimer un etudiant \n selectionnez un chiffre entre 1 et 4 :");
    scanf("%d",&choix);
        switch (choix) {
        case 1:
            printf("\n entrez le nom de fichier vous souhaitez creer :");
            scanf("%s",nomFichier);
            printf("\n combien d'etudiant vous voulez inserer");
            scanf("%d",&i);
            creer(nomFichier,i);
            break;

        case 2:
             fichier=ouvrir("listeEtudiants", 'a');// on ouvre le fichier existant deja
            printf("You selected Option 2.\n");
            printf("\n entrez le matricule de l'etudiant :");

            scanf("%s",matricule);
            printf("\n entrez le nom de l'etudiant :");

            scanf("%s",nom);
            printf("\n entrez le prenom de l'etudiant :");

            scanf("%s",prenom);
            inserer(&fichier,matricule,nom,prenom);

            fermer(fichier);
            break;

        case 3:
            printf("You selected Option 3.\n");
            fichier=ouvrir("listeEtudiants",'a');// on ouvre le fichier existant deja
            printf("\n entrez le matricule de l'etudiant :");

            scanf("%s",matricule);
            rechercheDichotomique(fichier,matricule,&i,&j,&trouv);
            if(trouv){
                printf("l'etudiant avec le matricule %s est dans le bloc num %d á la case avec l'indice %d",matricule,i,j);
            }
            else{
                 printf("l'etudiant avec le matricule %s n'existe pas",matricule);
            }
            fermer(fichier);
            break;
        case 4:
             printf("You selected Option 4.\n");
            fichier=ouvrir("listeEtudiants",'a');// on ouvre le fichier existant deja
            printf("\n entrez le matricule de l'etudiant :");
            scanf("%s",matricule);
            rechercheDichotomique(fichier,matricule,&i,&j,&trouv);
            if(trouv){
                printf("l'element avec le matricule %s est supprimé",matricule);
            }else{
                 printf("l'element avec le matricule %s n'existe pas",matricule);
            }
            fermer(fichier);
            break;

        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
    }

 return 0;
}
