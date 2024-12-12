#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct arbre{
    int identifiant;
    int idcentrale;
    long long capacite;
    long long consommation;
    struct arbre* fg;
    struct arbre* fd;
    int somme;
}Station;


typedef struct Arbre{
    Station c;
    struct station* fg;
    struct station* fd;
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
    d->identifiant=identifiant;
    d->capacite=capacite;
    d->fg=NULL;
    d->fd=NULL;
    d->equilibre=0;
    d->consommation=0;
    d->idcentrale=0;
    return d;
}

int max(int a, int b){
    return (a>b)?a:b;
}
int min(int a, int b){
    return (a<b)?a:b;
}

Arbre * rotationgauche(Arbre *a){
    Arbre*pivot=a->fd;
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
    Arbre*pivot=a->fg;
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
    } else if (id < a->identifiant) {
        a->fg = insertionArbre(a->fg,id,capacite,h);
        *h = -*h;
    } else if (id > a->identifiant) {
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

void ajoutconsommation(Arbre *a, long long k, int id){
    if(a==NULL){
        return;
    }
    if(id<a->identifiant){
        ajoutconsommation(a->fg,k,id);
    }
    if(id>a->identifiant){
        ajoutconsommation(a->fd,k,id);
    }
    if(id==a->identifiant){
        a->consommation+=k;
    }
}

void recuperationconsommation(Arbre*a, char *type){//type est la sattion hvb ou hva ou lv, on la rentre dans le main
    FILE *fichier=fopen(donnees_filtrees.csv,"r");
    if(fichier ==NULL){
        exit(2);
    }
    int id_temporaire = 0;
    long long k = 0;
    if(strcmp(type,"hvb")==0){
        while (fscanf(fichier,"%[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^;];%lld",&id_temporaire,&k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
    if(strcmp(type,"hva")==0){
        while (fscanf(fichier,"%[^;];%[^;];%d;%[^;];%[^;];%[^;];%[^;];%lld",&id_temporaire,&k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
    if(strcmp(type,"lv")==0){
        while (fscanf(fichier,"%[^;];%[^;];%[^;];%d;%[^;];%[^;];%[^;];%lld",&id_temporaire,&k) == 2){
            ajoutconsommation(a,k,id_temporaire);
        }
    }
}

Arbre* recuperationfichier( char * nomfichier, char *type, char *consommateur){
    FILE *fichier=fopen(nomfichier,"r");
    if(fichier ==NULL){
        exit(2);
    }
    Arbre *f;

    if(f==NULL){
        fclose(fichier);
        exit(2);
    }
    char ligne[200];
    while(fscanf(file,"%d;%[^;];%[^;];%[^;];%[^;];%[^;];%lld",f-> )==2){
        if(fscanf(ligne, ";%d;%d;%d",f->c->id,f->c->identifiant,f->c->capacite)) {
        }
    }

    while(fgets(ligne,199,fichier)){
        if(sscanf(ligne, "%d;%d;%d;%d",f->c->idcentrale,f->c->identifiant,f->c->capacite)){
            insertionArbre(f,f->c);
        }
    }
    fclose(fichier);
    return f;
}

//Une fonction qui additionne la consonsommation des consommateurs en faisant un parcour infixe
int sommeconsommation(Arbre* a){
    int k=0;
    if(a==NULL){
        exit(2);
    }
    sommeconsommation(a->fg);
    k=k+a->consommation;
    sommeconsommation(a->fd);
    return k;
}

int main(char **argv) {
    printf("Hello, World!\n");
    return 0;
}
