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
    struct station* fg;
    struct station* fd;
    int equilibre;
}Arbre;

//fonction d'equilibrage, creation arbre, max, min,
// rotation(gauche,droite,double);insertion,exoste fils droit, recherche
//fonction somme pour calculer tout les noeud de l'arbre'
// fonction pour recuperer info sur fichoer

Arbre *creationArbre(int identifiant, int idcentrale, long long capacité,long long consommation ){
    Arbre *d=malloc(sizeof(Arbre));
    if(d==NULL){
        exit(3);
    }
    d->identifiant=identifiant;
    d->idcentrale=idcentrale;
    d->capacite=capacité;
    d->consommation=consommation;
    d->fg=NULL;
    d->fd=NULL;
    d->equilibre=0;
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

Arbre *insertionArbre(Arbre *a, Station*e) {
    int *h;
    if (a == NULL) {
        *h = 1;
        return creationArbre(e);
    } else if (e->capacite < a->c.capacite) {
        a->fg = insertionArbre(a->fg, e);
        *h = -*h;
    } else if (e->capacite > a->c.capacite) {
        a->fd = insertionArbre(a->fd, e);
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

Arbre* recuperationfichier( char * nomfichier){
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
    while(fgets(ligne,199,fichier)){
        if(sscanf(ligne, "%d;%d;%d;%d",f->c->idcentrale,f->c->identifiant,f->c->capacite)){
            insertionArbre(f,f->c);
        }
    }
    fclose(fichier);
    return f;
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
