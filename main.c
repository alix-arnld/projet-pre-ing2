#include "fonction.h"

int main(int arc, char **argv){
    Arbre* a= recuperationfichier("fichier",argv[1],argv[2]);
    recuperationconsommation(a,argv[1]);
    creationfichieranalyse(a,argv[1],argv[2]);
    liberationArbre(a);
    return 0;
}
