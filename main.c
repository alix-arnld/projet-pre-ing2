#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct station{
    int identifiant;
    long long capacite;
    long long consommation;
    long long analyse;
}Station;


typedef struct Arbre{
    Station *station;
    struct Arbre* fg;
    struct Arbre* fd;
    int equilibre;
}Arbre;

//fonction d'equilibrage, creation arbre, max, min,
// rotation(gauche,droite,double);insertion,exoste fils droit, recherche
//fonction somme pour calculer tout les noeud de l'arbre'
// fonction pour recuperer info sur fichoer

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
    return d;
}

int max(int a, int b){
    return (a>b)?a:b;
}
int min(int a, int b){
    return (a<b)?a:b;
}

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

Arbre* doublerotationgauche(Arbre *a){
    a->fd= rotationdroite(a->fd);
    return rotationgauche(a);
}

Arbre*doublerotationdroite(Arbre*a){
    a->fg= rotationgauche(a->fg);
    return rotationdroite(a);
}

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

//inserer les capacitées et les identifiants dans l'arbre

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

//
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
        exit(2);
    }
    sommeconsommation(a->fg);
    a->station->analyse = (a->station->capacite - a->station->consommation);
    sommeconsommation(a->fd);
}

//procédure qui va créer les fichiers avec les données de surproduction ou non de chaque station en fonction de ce que l'utilisateur aura demandé
void creationfichieranalyse(Arbre *a, char *type, char * pareil){
    if(strcmp(type,"hvb")==0){
        FILE *hvb = fopen("hvb_comp.csv", "w");
        if (hvb == NULL) {
            exit(1);
        }
        printf("station:capacité:consommation totale des consommateurs:analyse de la comsommation");
        fprintf(hvb,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
        if(a->fg!=NULL){
            creationfichieranalyse(a->fg, type,pareil);
        }
        if(a->fd!=NULL){
            creationfichieranalyse(a->fd, type,pareil);
        }
    }
    if(strcmp(type,"hva")==0){
        FILE *hva = fopen("hva_comp.csv", "w");
        if (hva == NULL) {
            exit(1);
        }
        printf("tation:capacité:consommation totale des consommateurs:analyse de la comsommation");
        fprintf(hva, "%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
        if(a->fg!=NULL){
            creationfichieranalyse(a->fg, type,pareil);
        }
        if(a->fd!=NULL){
            creationfichieranalyse(a->fd, type,pareil);
        }
    }
    if(strcmp(type,"lv")==0){
        if(strcmp(pareil,"comp")==0){
            FILE *lv = fopen("lv_comp.csv", "w");
            if (lv == NULL) {
                exit(1);
            }
            printf("tation:capacité:consommation totale des consommateurs:analyse de la comsommation");
            fprintf(lv,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,pareil);
            }
            if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,pareil);
            }
        }
        if(strcmp(pareil,"indiv")==0) {
            FILE *lv = fopen("lv_indiv.csv", "w");
            if (lv == NULL) {
                exit(1);
            }
            printf("tation:capacité:consommation totale des consommateurs:analyse de la comsommation");
            fprintf(lv,"%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,pareil);
            }
            if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,pareil);
            }
        }
        if(strcmp(pareil,"all")==0) {
            FILE *lv = fopen("lv_all.csv", "w");
            if (lv == NULL) {
                exit(1);
            }
            printf("tation:capacité:consommation totale des consommateurs:analyse de la comsommation");
            fprintf(lv, "%d:%lld:%lld:%lld",a->station->identifiant,a->station->capacite,a->station->consommation,a->station->analyse);
            if(a->fg!=NULL){
                creationfichieranalyse(a->fg, type,pareil);
            }
            if(a->fd!=NULL){
                creationfichieranalyse(a->fd, type,pareil);
            }
        }
    }
}

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
    creationfichieranalyse(a,argv[1],luciejetelaissemodifier);
    liberationArbre(a);
    return 0;
}

/*

 message.txt
9 Ko

*/