#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct station{
    int identifiant;
    int idcentrale;
    int capacite;
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

Arbre *creationArbre(station *e){
    Arbre *d=malloc(sizeof(Arbre));
    if(d==NULL){
        exit(ERREUR);
    }
    d->c->identifiant=e->identifiant;
    d->c->capacite=e->capacite;
    d->c->somme=0;
    d->fg=NULL;
    d->fd=NULL;
    d->equilinbre=0;
    return d;
}

int max(int a, int b){
    return (a>b)?a:b;
}
int min(int a, int b){
    return (a<b)?a:b;
}

Arbre * rotationgauche(Arbre **a){
    Arbre**pivot=a->fd;
    int eq_a=a->equilibre,eq_p=pivot->equilibre;
    a->fd=pivot->fg;
    pivot->fg=a;
    a->equilibre=eq_a-max(eq_p,0)-1;
    pivot->equilibre=min(min(eq_a-2,eq_p+eq_p-2),eq_p-1);
    a=pivot;
    return a;
}

Arbre * rotationdroite(Arbre **a){
    Arbre**pivot=a->fg;
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

Arbre *insertionArbre(Arbre **a, Station*e) {
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
        exit(FICHIERINEXISTANT);
    }
    Arbre *f;
    if(f==NULL){
        fclose(fichier);
        exit(ERREUR);
    }
    char ligne[200];
    while(fgets(ligne,199,fichier)){
        if(sscanf(ligne, "%d;%d;%d",f->c->idcentrale,f->c->identifiant,f->c->capacite)){
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
