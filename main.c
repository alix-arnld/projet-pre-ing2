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
