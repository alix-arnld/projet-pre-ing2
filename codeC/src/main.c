#include "fonction.h"

int main(int argc,char** argv) {
    Arbre* a= recuperationfichier("../tmp/donnees_capacites.csv", argv[1]);
    recuperationconsommation(a,argv[1]);
    sommeconsommation(a);
    creationfichieranalyse(a,argv[1],argv[2]);
    liberationArbre(a);
    return 0;
}
