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
    destination->supprimer = source.supprimer;
    strcpy(destination->nom, source.nom);
    strcpy(destination->prenom, source.prenom);
    strcpy(destination->matricule, source.matricule);
}
// une fonvtion qui copie le contenue d'un element dans un autre
void copyBloc(bloc* destinationBloc, bloc sourceBloc) {
    destinationBloc->nb = sourceBloc.nb;

    for (int i = 0; i < sourceBloc.nb; ++i) {
        copyEnregistrement(&(destinationBloc->tab[i]), sourceBloc.tab[i]);
    }
}

// une fonction qui ecrit le contenue d'un bloc du fichier dans un buffer
void LireDir(TOF t, int i, buffer *buf) {

        copyBloc(buf, t.tab[i-1]);

}
// une fonction qui ecrit le contenue du buffer dans un fichier
void EcrireDir (TOF *t,int i,buffer buf) {


        copyBloc(&t->tab[i-1], buf);

}
// une fonction qui affiche les données d'un etudiant
void afficherEtudiant(enregistrement e){
    printf("le nom :%s / le prenom : %s / le matricule : %s \n",e.nom,e.prenom,e.matricule);
}
void afficherBloc(bloc b){
for(int i=0;i<b.nb;i++){
    if(!b.tab[i].supprimer)
     afficherEtudiant(b.tab[i]);
}


}
void afficherFichier(TOF fichier)
{

    int nbBloc=fichier.tete.nbrBlocs;
    int nbEnre=fichier.tete.nbrEl;
    // afficher l'entete;
    printf("\n           l'entete             \n");
    printf("\n le nom du fichier %s",fichier.tete.nomFichier);
    for(int i=1;i<4;i++){
        printf("\n la val num %d est: %d",i,ENTETE(fichier,i));
    }
    for(int i=0;i<nbBloc;i++){
            printf("\n \\\\\\\\ le blloc %d \\\\\\\\ \n",i+1);
    afficherBloc(fichier.tab[i]);
    }
}

// la fonction d'insertion
void inserer(TOF *fichier,char* matricule,char* nom, char* prenom){
   bool stop=false,trouv;
    int i,j,blocDebut,blocFin,indiceDebut,indiceFin;


    rechercheDichotomique(*fichier,matricule,&i,&j,&trouv);
   // si l'element n'existe pas alors RechercheDichotomique retourne le numero de bloc et l'indice ou cet element doit se placer


    if(!trouv && ENTETE(*fichier,2)!= max*max){// si l'element n'existe pas et on a de place dans le fichier alors
    buffer buf;
    enregistrement tmpDernier,tmpPremier;
    tmpPremier.supprimer=false;
    strcpy(tmpPremier.matricule,matricule);
    strcpy(tmpPremier.prenom,prenom);
    strcpy(tmpPremier.nom,nom);

       while(!stop && i<=ENTETE(*fichier,1)) // tant que on termine la recherche et i (le numero de bloc ou on travail) est inferieur ou egale a numero de bloc du fichier
    {
         LireDir(*fichier,i,&buf); // on lit le bloc numero i et on le copie dans le buffer
        indiceFin=buf.nb-1;    // on recupere l'indice de dernier element de bloc
        copyEnregistrement(&tmpDernier,buf.tab[indiceFin]);// on sauvegarde l'element du dernier indice car on va l'ecrasser

        while(j<indiceFin){
            copyEnregistrement(&buf.tab[indiceFin],buf.tab[indiceFin-1]);// on copie l'element dans l'element apres lui
            indiceFin--;
        }

        copyEnregistrement(&buf.tab[j],tmpPremier);// on affecte l'element á inserer dasn l'indice j
        EcrireDir(fichier,i,buf);// on copie le contenue du buffer dans le fichier apres avoir decaler les element et inserer le nouveau etudiant

        if(buf.nb<max ){ // si on peut inserer un autre lement dans le buffer
            if(indiceFin!=buf.nb-1) {copyEnregistrement(&buf.tab[buf.nb],tmpDernier);}// on copie le dernier element dasn la derniere case du buffer;
            buf.nb++;// on incremente le nombre d'element dans le buffer
             EcrireDir(fichier,i,buf); // on copie ce qui dans le buffer dans le fichier
             stop=true; // on arrete le decalage des element;
        }
        else{// on passe au prochain bloc
            EcrireDir(fichier,i,buf); // on copie ce qui dans le buffer dans le fichier
            i++;// on incremente le num de bloc
            j=0;// prochainement on va faire le decalage a partit de la case 0
            printf("\n \n \n hohohohoh l'element d'indice fin est  \n \n \n");
            afficherEtudiant(tmpDernier);
            copyEnregistrement(&tmpPremier,tmpDernier);// le prochain premier element dans le bloc suivant est actuellement le dernier dans ce bloc
        }
    }
     // si on a besoin de creer un nouveau bloc
        if(i>ENTETE(*fichier,1)){
            buf.nb=1;
            copyEnregistrement(&buf.tab[0],tmpPremier); // on copie cet element dans la premeiere cas de buffer
            AFF_ENTETE(fichier,1,i);// on incremente le nombre de bloc dans l'entete;
            EcrireDir(fichier,i,buf); // on copie le contenue de buffer dans le bloc q'on a ajoute
        }

         AFF_ENTETE(fichier,2,ENTETE(*fichier,2)+1);// on incremente le nombre d'element


    }
}

void fichierBinaireVersTxt(char *nomFile){
 FILE *fBinaire=fopen(nomFile,"rb+");
char newname[50];
strcpy(newname, nomFile);
strcat(newname, ".txt");

 FILE *fTxt=fopen(newname,"w+");
 buffer buf;
 entete tete;

 fread(&tete,sizeof(entete),1,fBinaire);// on lit l'entete;

 for(int i=1;i<=tete.nbrBlocs;i++){
    fseek(fBinaire,(sizeof(entete)+sizeof(bloc)*(i-1)),SEEK_SET);// Positionne le curseur dans le bloc i
    fread(&buf,sizeof(buffer),1,fBinaire);//on lit le bloc num i
    for(int j=0;j<buf.nb;j++){
    if(!buf.tab[i].supprimer){
    fprintf(fTxt,"%s \t %s\t %s \n",buf.tab[j].matricule, buf.tab[j].nom, buf.tab[j].prenom);
    }
    }
    }
    fclose(fBinaire);
    fclose(fTxt);

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

// Fonction de recherche dichotomique à l'intérieur d'un bloc
void rechercheDichotomiqueBloc(bloc blocRecherche,char* matricule, int *indice, bool *trouv) {
    *trouv = false;

    int debut = 0;
    int fin = blocRecherche.nb - 1;

    while (debut <= fin) {
        int milieu = (debut + fin) / 2;

        // Comparaison des matricules à l'intérieur du bloc
        int comparaison = strcmp(blocRecherche.tab[milieu].matricule, matricule);

        if (comparaison == 0) {
            // Matricule trouvé dans ce bloc
            *indice = milieu;
            *trouv = !blocRecherche.tab[milieu].supprimer;
            return;
        } else if (comparaison < 0) {
            // Le matricule recherché est dans la partie droite du bloc
            debut = milieu + 1;
        } else {
            // Le matricule recherché est dans la partie gauche du bloc
            fin = milieu - 1;
        }
    }

    // Matricule non trouvé dans le bloc
    *indice = debut;
    *trouv = false;
    return;
}

// Fonction de recherche dichotomique sur un fichier
void rechercheDichotomique(TOF fichier,char* matricule, int *numeroBloc, int *indice, bool *trouv) {

    *trouv = false;
    buffer buf;
    bool arret=false;

    int debut = 0;
    int fin = ENTETE(fichier,1)- 1; /// on recupere l'indice du dernier bloc

    while (debut <= fin && !arret) {
        int milieu = (debut + fin) / 2; /// on recupere l'indice du bloc milieu
        LireDir(fichier,milieu+1,&buf); /// on le met dans le buffer
        int comparaisonDebut = strcmp(matricule,buf.tab[0].matricule);
        int comparaisonFin = strcmp(matricule,buf.tab[buf.nb - 1].matricule);
        /***strcmp(A,B)
           <0  A doit venir avant B
           =0  A egale á B
           >0   A viens apres B
        ****/
        if (comparaisonDebut >= 0 && comparaisonFin <= 0) {
            int i;
            bool existe;
            rechercheDichotomiqueBloc(buf,matricule,&i,&existe);// on cherche le matricule dans le le buffer qui contient le bloc milieu
            *indice=i;
            *trouv=existe;
            *numeroBloc=milieu+1; // la valeur se retrouve ou doit s'inserer dans le bloc qu'on travail
            arret=true ;// on arrete la recherche
            return;

        }else if(comparaisonDebut < 0){/// matricule se positione dans la partie haute
            fin=milieu-1; // on change le bloc de fin
        }
        else{ /// matricule se positionne dans la partie bas

            debut= milieu+1; // on change le bloc de debut
        }

    }
     if (debut > fin) {/// verifiée lorsque l'element doit se retrouver avant le premier element on sort de la boucle avec fin=-1 ou doit se retrouver apres le dernieres element on sort de la boucle avec debut=fin+1
        *numeroBloc = debut+1;
        *indice=0;
    }
    if(fin== ENTETE(fichier,1)-1 && buf.nb<max){// si l'element doit se retrouver apres le dernieres element et on a de place dans le dernier bloc
        *numeroBloc = fin+1;
        *indice=buf.nb;
    }


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
    fichierBinaireVersTxt("listeEtudiants");

 return 0;
}


