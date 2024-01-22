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
///****************les algorithmes de manipulation des fichiers*********************//////
/// Ouvrir le fichier logique et l’associer au fichier physique en précisant si le fichier est nouveau ('N') ou ancien ('A').
TOF* ouvrir(char fileName[30],char mode)
{
    TOF* fileTOF=malloc(sizeof(TOF));
        if( mode=='a'|| mode=='A'){
            fileTOF->f=fopen(fileName,"rb+");
            fread(&(fileTOF->tete),sizeof(entete),1,fileTOF->f);
        }
        else if ( mode=='n'|| mode=='N'){
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
// une fonction qui ecrit le contenue d'un bloc du fichier dans un buffer
void LireDir (TOF *t,int i,buffer *buf)
{
  if (i<=ENTETE(t,1))
    {
      fseek(t->f,sizeof(entete)+(i-1)*sizeof(bloc),0); // on cherche la position du i eme bloc
      fread(buf,sizeof(bloc),1,t->f); // on copie son contenue dans le buffer 
      
    }
}
// une fonction qui ecrit le contenue du buffer dans un fichier
void EcrireDir (TOF *t,int i,buffer buf) 
{   // d'abord on verifie que le numero de bloc ne depasse pas la limite
    if(i< max){ 
      fseek(t->f,sizeof(entete)+(i-1)*sizeof(bloc),0); // on cherche la position du i eme bloc (ou  oû il doit se positionner)
      fwrite(&buf,sizeof(bloc),1,t->f); // on copie le contenue du buffer dans le fichier 
    } 
}
void copierEtudiant(enregistrement *destination, enregistrement source){
   strcpy(destination->nom, source.nom);
    strcpy(destination->prenom, source.prenom);
    destination->matricule = source.matricule;
    destination->supprimer = source.supprimer;
}
void Recherche_DEC_TOF(TOF *t, char *cle, int *i, int *j, bool *Trouv) { // i le numero de bloc et j l'indice ou se retrouve cet element 
    bloc Buf;
    int deb = 1, fin = ENTETE(t, 1), inf, sup, Arret = 0;
    *Trouv = false;
    *j = 0;
     int k=0;

    while ((!(*Trouv)) && (deb <= fin) && (!Arret))
    {  
        *i = (deb + fin) / 2; // on lit le numero du  bloc au milieu
        LireDir(t, *i, &Buf); // on copie son contenue dans le buffer

        if (Buf.nb > 0) // si il ya des element dans ce bloc
        { 
            if ((strcmp(cle, Buf.tab[0].nom) >= 0) && (strcmp(cle, Buf.tab[Buf.nb - 1].nom) <= 0)) // si la valeur á chercher est comprise entre le 1er et dernier element de ce bloc
             {
                inf = 0; /// on initialise l debut des element 
                sup = Buf.nb - 1;

                while ((!(*Trouv)) && (inf <= sup)) // 
                {
                    *j = (inf + sup) / 2; // on cherche l'element au milieu
                    int compareResult = strcmp(cle, Buf.tab[*j].nom); // on compare cet element avec la valeur à chercher

                    if (compareResult == 0 )  // si ils sont egaux 
                    {
                        *Trouv = true; //l'element n'est pas effacé logiquement trouv recoit vrai sinon 
                        if(Buf.tab[*j].supprimer){
                            k=1; // si l'element est effacé logiquement 
                        } 
                    } else // si c'est pas egale
                    {
                        if (compareResult < 0)  // la valeur a chercher doit se placer avant cet element de milieu
                        {
                            sup = *j - 1; // on chnge la borne sup pour chercher dans la premiere partie 
                        } else    //  // la valeur a chercher doit se placer apres cet element de milieu
                        {
                            inf = *j + 1;  // on chnge la borne inf pour chercher dans la deuxieme partie
                        }
                    }
                } // on retourne a la boucle while pour chercher jusqu'a la borne inf > la borne sup

                if (!(*Trouv)) // si on a pas trouvé l'element et l'element doit etre placé dans ce bloc 
                {
                    *j = inf;  // si on veut l'inserer il doit ce positionner dans la case j
                    Arret = true; // on arrete la recherche on est sure que l'element n'existe pas
                }
            } else {  //si la valeur á chercher n'est pas comprise entre le 1er et dernier element de ce bloc on change de bloc 
                if (strcmp(cle, Buf.tab[0].nom) < 0) {// si l'element a chercher doit se placer dans un des bloc avant 
                    fin = *i - 1; // on cherche dans la premiere partie des blocs
                } else { //si l'element a chercher doit se placer dans un des bloc apres
                    deb = *i + 1;  // on cherche dans la deuxieme  partie des bloc 
                }
            }
        } else { // si on a plus d'element dans les blocs
            Arret = true; // on arrete la rechere
        }
    }

    if (deb > fin) { // si le bloc de debut est plus grand que bloc de fin apres sortir de la boucle while
        *i = deb; // l'element doit se placer dans le bloc du debut 
    }
    if (Trouv && k==1){ // si l'element est trouvé mais supprimé logiquement donc il n'est pas dans le fichier trouv est faux
        Trouv=false; // on a ajouté k pour sauvegardé l'indice d'element 
        *j=j+1; // si on veut l'inserer il vas se placer apres l'element 
    }

}
void inserer(TOF *fichier, char *nom, char *prenom, int matricule){
    bool stop=false,trouv;
    int i,j,blocDebut,blocFin,indiceDebut,indiceFin;
    
    Recherche_DEC_TOF(fichier,nom,&i,&j,&trouv);
   
    // si l'element n'existe pas alors Recherche_DEC_TOF retourne i et j le numero de bloc et l'indice ou cet element doit se placer
    if(!trouv && ENTETE(fichier,2)!= max*max){// si l'element n'existe pas et on a de place dans le fichier alors
    buffer buf;
    enregistrement tmpDernier,tmpPremier;
    tmpPremier.matricule=matricule;
    tmpPremier.supprimer=false;
    strcpy(tmpPremier.prenom,prenom);
    strcpy(tmpPremier.nom,nom);

     while(!stop && i<=ENTETE(fichier,1)) // tant que on termine la recherche et i (le numero de bloc ou on travail) est inferieur ou egale a numero de bloc du fichier
    { 
         LireDir(fichier,i,&buf); // on lit le bloc numero i et on le copie dans le buffer
        indiceFin=buf.nb-1;    // on recupere l'indice de dernier element de bloc
        copierEtudiant(&tmpDernier,buf.tab[indiceFin]);// on sauvegarde l'element du dernier indice car on va l'ecrasser
        while(j<indiceFin){
            copierEtudiant(&buf.tab[indiceFin],buf.tab[indiceFin-1]);// on copie l'element dans l'element avant lui
            indiceFin--;
        }

        copierEtudiant(&buf.tab[j],tmpPremier);// on affecte l'element á inserer dasn l'indice j
        EcrireDir(fichier,i,buf);// on copie le contenue du buffer dans le fichier apres avoir decaler les element et inserer le nouveau etudiant
        
        if(buf.nb<max){ // si on peut inserer un autre lement dans le buffer
            copierEtudiant(&buf.tab[buf.nb],tmpDernier);// on copie le dernier element dasn la derniere case du buffer;
            buf.nb++;// on incremente le nombre d'element dans le buffer
             EcrireDir(fichier,i,buf); // on copie ce qui dans le buffer dans le fichier
             stop=true; // on arrete le decalage des element;
        } 
        else{// on passe au prochain bloc
            EcrireDir(fichier,i,buf); // on copie ce qui dans le buffer dans le fichier
            i++;// on incremente le num de bloc
            j=0;// prochainement on va faire le decalage a partit de la case 0
            copierEtudiant(&tmpPremier,tmpDernier);// le prochain premier element dans le bloc suivant est actuellement le dernier dans ce bloc 
        }
        }
        
    }
}
void supprimerLogique(TOF *fichier,char *key){
    int i,j;
    bool trouv;
    buffer buf;
    Recherche_DEC_TOF(fichier,key,&i,&j,&trouv);
    if(trouv){
        LireDir(fichier,i,&buf);// on lit le bloc oú se retrouve l'element
        buf.tab[j].supprimer=true; // on change le champs "supprimer" á vrai
        AFF_ENTETE(fichier,3,fichier->tete.nbrElSupp-1) ;// on incremente le nombre d'element supprimé
        EcrireDir(fichier,i,buf);// on recopie le contenue 
    }
}

TOF *creerFichier(char *nomFile,int nbEnregistrements){
    TOF *fichier=ouvrir(nomFile,'n');
    if(nbEnregistrements<= max*max){// si le nombres d'element l'utilisateur souhaite saisir est inferieur au nombrre d'enregistrement max que le fichier peut avoir
        // on cree le fichier


        char *nom,*prenom;
        int matricule;
        for(int cpt=0;cpt<nbEnregistrements;cpt++){
            printf("\n entrez les information du %d eme element:", cpt+1);
            printf("\n le nom :");
            scanf("%s",nom);
             printf("\n le prennom :");
            scanf("%s",prenom);
             printf("\n le matricule :");
            scanf("%d",matricule);
            inserer(fichier,*nom,*prenom,matricule);
        }
        fermer(fichier);// on ferme le fichier pour sauvegarder l'entete;
    }
    return fichier;
}

int main(){
    printf("hello world");
   

    return 0;
}
