#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct station{
    int identifiant;
    long long capacite;
    long long consommation;
    long long analyse;
    long long valeur_absolue;
}Station;


typedef struct Arbre{
    Station *station;
    struct Arbre* fg;
    struct Arbre* fd;
    int equilibre;
}Arbre;

//fonction qui va créer un nœud de l'arbre et l'initialisera
Arbre *creationArbre(int identifiant , long long capacite){
    Arbre *d=malloc(sizeof(Arbre));
    if(d==NULL){
        exit(2);
    }
    d->station->identifiant=identifiant;
    d->station->capacite=capacite;
    d->fg=NULL;
    d->fd=NULL;
    d->equilibre=0;
    d->station->consommation=0;
    d->station->analyse=0;
    d->station->valeur_absolue=0;
    return d;
}

//fonction qui retournera le maximum entre les 2 valeurs en paramètre
int max(int a, int b){
    return (a>b)?a:b;
}

//fonction qui retournera le minimum entre les 2 valeurs en paramètre
int min(int a, int b){
    return (a<b)?a:b;
}

//fonction qui va rééquilibrer l'arbre par unr rotation simple gauche si l'élément ajouté qui déséquilibre est tout à droite
Arbre * rotationgauche(Arbre *a){
    Arbre *pivot=a->fd;
    int eq_a=a->equilibre,eq_p=pivot->equilibre;
    a->fd=pivot->fg;
    pivot->fg=a;
    a->equilibre=eq_a-max(eq_p,0)-1;
    pivot->equilibre=min(min(eq_a-2,eq_p+eq_p-2),eq_p-1);
    a=pivot;
    return a;
}

//fonction qui va rééquilibrer l'arbre par unr rotation simple droite si l'élément ajouté qui déséquilibre est tout à gauche
Arbre * rotationdroite(Arbre *a){
    Arbre *pivot=a->fg;
    int eq_a=a->equilibre,eq_p=pivot->equilibre;
    a->fg=pivot->fd;
    pivot->fd=a;
    a->equilibre=eq_a-min(eq_p,0)+1;
    pivot->equilibre=max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    a=pivot;
    return a;
}

//fonction qui va rééquilibrer l'arbre si une rotation simple ne suffit pas car la hauteur est trop importante, elle va faire une rotation simple droite puis une simple gauche
Arbre* doublerotationgauche(Arbre *a){
    a->fd= rotationdroite(a->fd);
    return rotationgauche(a);
}

//fonction qui va rééquilibrer l'arbre si une rotation simple ne suffit pas car la hauteur est trop importante, elle va faire une rotation simple gauche puis une simple droite
Arbre*doublerotationdroite(Arbre*a){
    a->fg= rotationgauche(a->fg);
    return rotationdroite(a);
}

// fonction qui va rééquilibrer l'arbre si il ne l'est plus, en utilisant les différentes rotations en fonction des cas
Arbre* equilibrage(Arbre *a){
    if(a->equilibre>=2) {
        if (a->fd >= 0) {
            return rotationgauche(a);
        } else {
            return doublerotationgauche(a);
        }
    }
    if(a->equilibre<=2){
        if(a->fd<=0){
            return rotationdroite(a);
        }
        else{
            return doublerotationdroite(a);
        }
    }
    return a;
}

//fonction qui permet d'ajouter un noeud dans l'arbre AVL et qui rééquilibrera l'arbre après chaque ajout
Arbre *insertionArbre(Arbre *a, int id, long long capacite, int* h){
    if (a == NULL) {
        *h = 1;
        return creationArbre(id,capacite);
    } else if (id < a->station->identifiant) {
        a->fg = insertionArbre(a->fg,id,capacite,h);
        *h = -*h;
    } else if (id > a->station->identifiant) {
        a->fd = insertionArbre(a->fd,id,capacite,h);
    } else {
        *h = 0;
        return a;
    }
    if (*h != 0) {
        a->equilibre += *h;
        a = equilibrage(a);

        if (a->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }
    return a;
}

//procédure de recherche à partir des identifiants de la station pour ajouter la consommation du consommateur à celle totale
void ajoutconsommation(Arbre *a, long long k, int id){
    if(a==NULL){
        printf("l'arbre est vide");
        return;
    }
    if(id<a->station->identifiant){
        ajoutconsommation(a->fg,k,id);
    }
    if(id>a->station->identifiant){
        ajoutconsommation(a->fd,k,id);
    }
    if(id==a->station->identifiant){
        a->station->consommation+=k;
    }
}

//procédure qui lit les données filtrées depuis un fichier CSV en fonction d'un type donné et met à jour la consommation totale en y ajoutant les consommations associées aux consommateurs
void recuperationconsommation(Arbre*a, char *type){
    if(a==NULL){
        printf("L'arbre est vide");
        return;
    }
    FILE *fichier=fopen("donnees_filtrees.csv","r");
    if(fichier ==NULL){
        exit(2);
    }

    int id_temporaire = 0;
    long long k = 0;
    if(strcmp(type,"hvb")==0){
        while (fscanf(fichier,"%*[^;];%d;%*[^;];%*[^;];%*[^;];%*[^;];%*[^;];%lld",&id_temporaire,&k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
    if(strcmp(type,"hva")==0){
        while (fscanf(fichier,"%*[^;];%*[^;];%d;%*[^;];%*[^;];%*[^;];%*[^;];%lld",&id_temporaire,&k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
    if(strcmp(type,"lv")==0){
        while (fscanf(fichier,"%*[^;];%*[^;];%*[^;];%d;%*[^;];%*[^;];%*[^;];%lld",&id_temporaire, &k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
}

// Fonction qui permet de récupérer les stations dans le fichier pour ensuite les ajouter dans un arbre AVL
Arbre* recuperationfichier(char * nomfichier, char *type, char *consommateur){
    FILE *fichier=fopen(nomfichier,"r");
    if(fichier ==NULL){
        exit(2);
    }
    Arbre* n=NULL;
    int ident;
    int h=0;
    long long capa;
    while(fscanf(fichier,"%d;%*[^;];%*[^;];%*[^;];%*[^;];%*[^;];%lld",&ident,&capa)==2){
        n = insertionArbre(n, ident, capa, &h);
    }
    fclose(fichier);
    return n;
}

//Une procédure qui calcule la différence entre la capacité d'une station et la consommation des consommateurs avec un parcours infixe
void sommeconsommation(Arbre* a){

    if(a==NULL){
        printf("L'arbre est vide");
        return;
    }
    sommeconsommation(a->fg);
    a->station->analyse = (a->station->capacite - a->station->consommation);
    a->station->valeur_absolue=a->station->analyse;
    if(a->station->valeur_absolue<0){
        a->station->valeur_absolue=-a->station->valeur_absolue;
    }
    sommeconsommation(a->fd);
}

//procédure qui va créer les fichiers avec les données de surproduction ou non de chaque station en fonction de ce que l'utilisateur aura demandé
void creationfichieranalyse(Arbre *a, char *type, char * consommateur){
    if(a==NULL){
        printf("L'arbre est vide");
        return;
    }
    if(strcmp(type,"hvb")==0){
        FILE *fichier = fopen("hvb_comp.csv", "w");
        if (fichier == NULL) {
            exit(2);
        }
        fprintf(fichier,"station:capacité:consommation totale des consommateurs:analyse de la comsommation");
        fprintf(fichier,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
        if(a->fg!=NULL){
            creationfichieranalyse(a->fg, type,consommateur);
        }
        else if(a->fd!=NULL){
            creationfichieranalyse(a->fd, type,consommateur);
        }
        else{
            fclose(fichier);
            return;
        }
    }
    if(strcmp(type,"hva")==0){
        FILE *fichier = fopen("hva_comp.csv", "w");
        if (fichier == NULL) {
            exit(2);
        }
        fprintf(fichier,"station:capacité:consommation totale des consommateurs:analyse de la comsommation");
        fprintf(fichier, "%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
        if(a->fg!=NULL){
            creationfichieranalyse(a->fg, type,consommateur);
        }
        else if(a->fd!=NULL){
            creationfichieranalyse(a->fd, type,consommateur);
        }
        else{
            fclose(fichier);
            return;
        }
    }
    if(strcmp(type,"lv")==0){
        if(strcmp(consommateur,"comp")==0){
            FILE *fichier = fopen("lv_comp.csv", "w");
            if (fichier == NULL) {
                exit(2);
            }
            fprintf(fichier,"station:capacité:consommation totale des consommateurs:analyse de la comsommation");
            fprintf(fichier,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,consommateur);
            }
            else if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,consommateur);
            }
            else{
                fclose(fichier);
                return;
            }
        }
        if(strcmp(consommateur,"indiv")==0) {
            FILE *fichier = fopen("lv_indiv.csv", "w");
            if (fichier == NULL) {
                exit(2);
            }
            fprintf(fichier,"station:capacité:consommation totale des consommateurs:analyse de la comsommation");
            fprintf(fichier,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,consommateur);
            }
            else if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,consommateur);
            }
            else{
                fclose(fichier);
                return;
            }
        }
        if(strcmp(consommateur,"all")==0) {
            FILE *fichier = fopen("lv_all.csv", "w");
            FILE *valeur_absolue = fopen("lv_valeur_absolue.csv", "w");
            if (fichier == NULL) {
                exit(2);
            }
            fprintf(fichier,"station:capacité:consommation totale des consommateurs:analyse de la comsommation\n");
            fprintf(fichier, "%d:%lld:%lld:%lld\n",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            fprintf(valeu_absolue,"station:capacité:consommation totale des consommateurs:analyse de la comsommation en valeur absolue\n");
            fprintf(valeur_absolue, "%d:%lld:%lld:%lld\n",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->valeur_absolue);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,consommateur);
            }
            else if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,consommateur);
            }
            else{
                fclose(fichier);
                return;
            }
        }
    }
}

//procédure qui va libérer l'espace alloué par l'arbre en commençant par les feuilles
void liberationArbre(Arbre* a){
    if(a==NULL){
        return;
    }
    liberationArbre(a->fg);
    liberationArbre(a->fd);
    free(a);
}

int main(int arc, char **argv) {
    Arbre* a= recuperationfichier("fichier",argv[1],argv[2]);
    recuperationconsommation(a,argv[1]);
    creationfichieranalyse(a,argv[1],argv[2]);
    liberationArbre(a);
    return 0;
}
