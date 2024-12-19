#ifndef PROJET_PRE_ING2_FONCTION_H
#define PROJET_PRE_ING2_FONCTION_H

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


Arbre *creationArbre(int, long long);

int max(int, int);

int min(int , int);

Arbre * rotationgauche(Arbre *);

Arbre * rotationdroite(Arbre *);

Arbre* doublerotationgauche(Arbre *);

Arbre*doublerotationdroite(Arbre *);

Arbre* equilibrage(Arbre *);

Arbre *insertionArbre(Arbre *, int, long long, int* );

void ajoutconsommation(Arbre *, long long, int);

void recuperationconsommation(Arbre*, char *);

Arbre* recuperationfichier(char *, char *);

void sommeconsommation(Arbre*);

void ecriturefichier(Arbre * , FILE *)

void creationfichieranalyse(Arbre *, char *, char *);

void liberationArbre(Arbre*);

#endif //PROJET_PRE_ING2_FONCTION_H
