#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct station{
    int identifiant;
    int capacite;
    int somme;
    struct station* fg;
    struct station* fd;
    int equilibre;
}Station;

//fonction d'quilibrage

int main() {
    printf("Hello, World!\n");
    return 0;
}
