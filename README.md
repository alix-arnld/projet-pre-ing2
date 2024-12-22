Read me: projet d’informatique

Notre projet d’informatique est codé en deux parties: le langage c et le shell. Premièrement le code en langage c a été repartis en 3 modules: «main.c», «langage.c» et «fonction.h».
    • langage c
«fonction.c»

Bibliothèques Standard du C:
<stdio.h>: Utilisée pour les opérations d'entrée/sortie, comme la lecture et l'écriture dans des fichiers.
<stdlib.h>: Utilisée pour la gestion de la mémoire dynamique et le contrôle des erreurs.
<math.h>: Utilisée pour des calculs mathématiques.
<string.h>: Utilisée pour la manipulation de chaînes de caractères.

Gestion de l'arbre AVL
→ Arbre *creationArbre(int identifiant, long long capacite): Crée un nouveau nœud d’arbre AVL. Initialise les valeurs associées à une station. Retourne un pointeur vers le nœud créé.
→ int max(int a, int b): Retourne le maximum entre deux entiers.
→ int min(int a, int b): Retourne le minimum entre deux entiers.
→ Arbre *rotationgauche(Arbre *a): Effectue une rotation simple à gauche pour rééquilibrer l'arbre.
→ Arbre *rotationdroite(Arbre *a): Effectue une rotation simple à droite pour rééquilibrer l'arbre.
→ Arbre *doublerotationgauche(Arbre *a): Rééquilibre un arbre avec une double rotation gauche.
→ Arbre *doublerotationdroite(Arbre *a): Rééquilibre un arbre avec une double rotation droite.
→ Arbre *equilibrage(Arbre *a): Détermine et applique la rotation nécessaire pour rééquilibrer un arbre déséquilibré.
→ Arbre *insertionArbre(Arbre *a, int id, long long capacite, int *h): Insère un nœud dans l'arbre AVL en maintenant son équilibre. Met à jour la hauteur après insertion.
→ void liberationArbre(Arbre *a): Libère récursivement la mémoire allouée pour l'arbre AVL.

Gestion des données
→ void ajoutconsommation(Arbre *a, long long consommation, int id): Ajoute une consommation à une station spécifique identifiée par son ID.
→ void recuperationconsommation(Arbre *a, char *type): Lit les données filtrées dans le fichier donnees_filtrees.csv. Met à jour les consommations des stations dans l'arbre en fonction du type.
→ Arbre *recuperationfichier(char *nomfichier, char *type, char *consommateur):Récupère les stations depuis un fichier CSV d’entrée et construit un arbre AVL à partir de ces données.
→ void sommeconsommation(Arbre *a): Parcours l’arbre pour calculer la différence entre capacité et consommation. Met à jour les champs analyse et valeur_absolue de chaque station.
→ void ecriturefichier(Arbre *a, FILE *fichier): Parcourt l'arbre et écrit les informations de chaque station dans un fichier.
→ void creationfichieranalyse(Arbre *a, char *type, char *consommateur): Crée des fichiers CSV pour enregistrer les résultats de l’analyse selon les types de stations et de consommation. Génère des fichiers comme hvb_comp.csv, lv_indiv.csv, etc.

«fonction.h»
→ la structure «station» représente une station électrique avec ses caractéristiques principales qui sont: l’identifiant, la capacité, la consommation, l’analyse, la valeur absolue.
→ la structure «arbre» représente un nœud d’un arbre AVL avec comme attributs: la station, faisant appel à la structure station, fg et fd, les pointeurs des sous arbres droites et gauches, et l’équilibre.

«main.c»
Il commence par récupérer les données depuis un fichier CSV, puis traite les consommations associées. Ensuite, il calcule la somme des consommations, génère un fichier d'analyse basé sur ces données, et libère la mémoire utilisée pour l'arbre, assurant ainsi une gestion efficace des ressources. 

    • Shell
Le script commence par vérifier que le chemin et le fichier CSV saisis par l'utilisateur sont corrects. À travers une boucle, il demande à l'utilisateur de saisir le chemin complet du fichier CSV, vérifie que ce fichier existe, et s'assure qu'il possède l'extension .csv. Si toutes ces conditions sont remplies, le script passe à l'étape suivante.
Ensuite, il demande à l'utilisateur de saisir le type de station (hvb, hva, ou lv) et valide cette entrée. Une autre boucle est utilisée pour demander le type de consommateur (comp, indiv, ou all), avec une vérification supplémentaire pour interdire certaines combinaisons spécifiques (par exemple, hvb all ou hva indiv). En cas de combinaison interdite, le script ajuste automatiquement les paramètres pour éviter toute erreur.
L'utilisateur peut également fournir un identifiant de centrale, bien que cette étape soit optionnelle. Si aucun identifiant n’est fourni, le script procède avec les paramètres de station et de consommateur uniquement.
Le script mesure ensuite le temps nécessaire pour traiter les données. Deux fichiers principaux sont générés à partir du fichier CSV d'origine : un fichier des capacités (donnees_capacites.csv), et un fichier source filtré (donnees_filtrees.csv).
Avant d'exécuter le programme C (projet_exe), le script vérifie si l'exécutable existe. Si ce n'est pas le cas, il compile automatiquement le programme en utilisant un Makefile. Une fois l'exécutable disponible, le script le lance avec les paramètres appropriés (type de station, type de consommateur, et identifiant de centrale, le cas échéant). La sortie du programme est redirigée vers un fichier de résultats. Si un identifiant de centrale est fourni, le fichier de résultats porte un nom spécifique l'incluant. Sinon, il est nommé en fonction des types de station et de consommateur.
Dans le cas particulier où le type de station est lv et le type de consommateur est all, le script effectue un traitement supplémentaire. Il extrait les 10 lignes avec les valeurs maximales et les 10 lignes avec les valeurs minimales de consommation, combine ces données dans un fichier (lv_all_minmax.csv), et les trie selon la différence entre la capacité et la consommation.
Enfin, le script mesure et affiche le temps total d'exécution, incluant à la fois le traitement des données avec awk et l'exécution du programme C. Cela permet de fournir une indication de performance pour l’ensemble du processus.
Dans le script, une étape clé consiste à analyser les données de consommation pour les cas spécifiques de type "lv_all". Cela implique d'extraire les 10 lignes avec les valeurs maximales et les 10 avec les valeurs minimales, puis de les combiner dans un seul fichier. Ces données sont ensuite triées par quantité d'énergie consommée en excès pour identifier les cas les plus significatifs. Une fois l'analyse terminée, un en-tête est ajouté au fichier final pour garantir la clarté et la structure des résultats. Les fichiers temporaires sont systématiquement supprimés pour optimiser l'organisation. Enfin, le script mesure et affiche le temps écoulé pour cette opération, offrant ainsi une vue d'ensemble du processus d'exécution.
