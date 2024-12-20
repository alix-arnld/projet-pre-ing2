#!/bin/bash

# Fonction d'aide
function afficher_aide() {
    echo "Usage : $0"
    echo
    echo "Ce script collecte des informations sur un fichier CSV, un type de station,"
    echo "un type de consommateur et, éventuellement, un identifiant de centrale."
    echo
    echo "Options :"
    echo "  -h       Affiche cette aide et quitte le script."
    echo
    echo "Étapes du script :"
    echo "  1. Saisissez le chemin où le fichier source ***.csv existe."
    echo "  2. Indiquez le type de station parmi :"
    echo "       - hvb : Haute tension, station type B."
    echo "       - hva : Haute tension, station type A."
    echo "       - lv  : Basse tension."
    echo "  3. Indiquez le type de consommateur parmi :"
    echo "       - comp : Société"
    echo "       - indiv : individuel."
    echo "       - all  : Tous types de consommateurs."
    echo "     Remarque : Certaines combinaisons station/consommateur sont interdites :"
    echo "       - hvb all, hvb indiv, hva all, hva indiv."
    echo "  4. (Optionnel) Saisissez l'identifiant d'une centrale."
    echo
    echo "Exemple :"
    echo "  $0"
    echo "    Entrez le chemin du fichier et le nom du fichier csv : /home/user/projet/cs-wire.csv"
    echo "    Entrez le type de station (hvb, hva ou lv) : hva"
    echo "    Entrez le type de consommateur (comp, indiv ou all) : comp"
    echo "    Entrez l'identifiant de la centrale (optional) : 2"
    echo
    echo "Note : Si une combinaison invalide est détectée, le script vous demandera de réessayer."
    exit 0
}

for arg in "@"; do
    if [ "$arg" == '-h' ]; then
        argh=1
    fi
done     

if [[ "$1" == "-h" ]]; then
    afficher_aide
    exit 0
fi

# Boucle pour vérifier que le chemin d'acces saisi mene vers le fichier source csv
while true; do
    read -p "Entrez le chemin complet du fichier CSV suivi du nom du fichier : " chemin_fichier

    # Vérification de l'existence du fichier
    if [[ -e "$chemin_fichier" ]]; then
        # Vérification de l'extension .csv
        if [ -f $chemin_fichier ]; then
            echo "Le fichier $chemin_fichier a été trouvé."
            break  # Sortir de la boucle si le fichier est correct
        else
            echo "Le fichier n'est pas au format CSV. Veuillez réessayer."
            afficher_aide
            exit 1
        fi
    else
        echo "Le fichier $chemin_fichier n'existe pas. Veuillez réessayer."
    fi
done

# Boucle pour vérifier le type de station saisi est valide
while true; do
    read -p "Entrez le type de station (hvb, hva ou lv) : " type_station
    if [[ "$type_station" =~ ^(hvb|hva|lv)$ ]]; then
        break
    else
        echo "Type de station invalide. Veuillez réessayer."
        afficher_aide
        exit 2
    fi
done

# Boucle pour vérifier le type de consommateur saisi est valide
while true; do
    read -p "Entrez le type de consommateur (comp, indiv ou all) : " type_consommateur
    if [[ "$type_consommateur" =~ ^(comp|indiv|all)$ ]]; then   
    # Vérification des combinaisons interdites
        if [[ "$type_station" =~ ^(hvb|hva)$ ]] && [[ "$type_consommateur" =~ ^(all|indiv)$ ]]; then
            echo "Erreur : Les combinaisons 'hvb all' 'hvb all' 'hva all' et 'hva indiv' sont toutes interdites."
            echo "On va traiter les compagnies uniquement"
            type_consommateur="comp"
            break
        else 
            break
        fi
    else
        echo "Type de consommateur invalide. Veuillez réessayer."
    fi
done

# Demande l'identifiant d'une centrale
read -p "Entrez l'identiant de la centrale (optional): " identifiant_centrale

# Stocker le temps de début
debut1=$(date +%s)

# construction du fichier de capacites en fonction des parametres

entete=1

if [ ! -d "tmp" ]; then
    mkdir "tmp"
fi

if [ ! -d "graphs" ]; then
    mkdir "graphs"
fi

if [ -d "tmp" ]; then
    rm -rf "tmp"
    mkdir "tmp"
fi

if [[ -n "$identifiant_centrale" ]]; then
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 == "-" && $7 != "-"' "$chemin_fichier"> tmp/donnees_capacites.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 != "-" && $7 != "-"' "$chemin_fichier" > tmp/donnees_capacites.csv
        else
            if [[ "$type_station" == "lv" ]]; then
                awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $4 != "-" && $7 != "-"' "$chemin_fichier" > tmp/donnees_capacites.csv
            fi
        fi
    fi
else
     if [[ "$type_station" == "hvb" ]]; then
        awk -F';' '$2 != "-" && $3 == "-" && $7 != "-"' "$chemin_fichier" > tmp/donnees_capacites.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' '$2 != "-" && $3 != "-" && $7 != "-"' "$chemin_fichier" > tmp/donnees_capacites.csv
            entete=0
        else
            if [[ "$type_station" == "lv" ]]; then
                awk -F';' '$4 != "-" && $7 != "-"' "$chemin_fichier" > tmp/donnees_capacites.csv
                entete=0
            fi
        fi
    fi
fi

# Ajouter l'en-tête au fichier de sortie si besoin
if [[ "$entete" == "1" ]]; then
    echo "Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load" > tmp/temp.csv
    cat tmp/donnees_capacites.csv >> tmp/temp.csv
    mv tmp/temp.csv tmp/donnees_capacites.csv
fi

# construction du fichier source filtre en fonction des parametres

if [[ -n "$identifiant_centrale" ]]; then
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 == "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
        else
            if [[ "$type_station" == "lv" && "$type_consommateur" == "indiv" ]]; then
                awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 == "-" && $5 == "-" && $6 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
            else
                if [[ "$type_station" == "lv" && "$type_consommateur" == "comp" ]]; then
                    awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 == "-" && $5 != "-" && $6 == "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
                else
                    awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 == "-" && $4 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
                fi
            fi
        fi
    fi
else
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' '$2 != "-" && $3 == "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' '$2 == "-" && $3 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
        else
            if [[ "$type_station" == "lv" && "$type_consommateur" == "indiv" ]]; then
                awk -F';' '$2 == "-" && $3 == "-" && $4 != "-" && $5 == "-" && $6 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
            else    
                if [[ "$type_station" == "lv" && "$type_consommateur" == "comp" ]]; then
                    awk -F';' '$2 == "-" && $3 == "-" && $4 != "-" && $5 != "-" && $6 == "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
                else
                    awk -F';' '$2 == "-" && $3 == "-" && $4 != "-" && $8 != "-"' "$chemin_fichier" > tmp/donnees_filtrees.csv
                fi
            fi
        fi
    fi
fi

# Ajouter l'en-tête au fichier de sortie
echo "Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load" > tmp/temp.csv
cat tmp/donnees_filtrees.csv >> tmp/temp.csv
mv tmp/temp.csv tmp/donnees_filtrees.csv

# Stocker le temps de fin
fin1=$(date +%s)

# Calculer le temps écoulé
temps1=$(( fin1 - debut1 ))


# on verifie si l executable existe sinon on le complile via un makefile

# Nom de l'exécutable à vérifier
executable="projet_exe"

# Vérifier si l'exécutable existe sinon on le genere
if [[ ! -f "$executable" ]]; then
    echo "L'exécutable '$executable' n'existe pas ou n'est pas exécutable."
    echo "Lancement de la compilation..."
    (cd codeC && make clean && make)
fi

# Stocker le temps de début dans une variable
debut2=$(date +%s)

# Lancer l'exécutable et rediriger la sortie vers un fichier

if [[ -n "$identifiant_centrale" ]]; then
    # Si l'identifiant de centrale est renseigné, on construit un nom de fichier complet
    (cd codeC && ./"$executable" $type_station $type_consommateur $identifiant_centrale)
else
    # Sinon, on construit un nom de fichier sans l'identifiant de centrale
    (cd codeC && ./"$executable" $type_station $type_consommateur)
fi

# on copie les resultats du programme c dans le fichier final
if [[ -n "$identifiant_centrale" ]]; then
    # Si l'identifiant de centrale est renseigné, on construit un nom de fichier complet
    fichier_resultat="codeC/${type_station}_${type_consommateur}_${identifiant_centrale}.csv"
else
    # Sinon, on construit un nom de fichier sans l'identifiant de centrale
    fichier_resultat="codeC/${type_station}_${type_consommateur}.csv"
fi


# On extrait les 10 min et les 10 max dans les cas lv_all par consommation
if [[ "$type_station" == "lv" && "$type_consommateur" == "all" ]]; then
    # Extraire les 10 valeurs maximales et leurs lignes associées
    sort -nr -t':' -k3 "$fichier_resultat" | head -n 10 > lv_all_max.csv

    # Extraire les 10 valeurs minimales et leurs lignes associées
    sort -n -t':' -k3 "$fichier_resultat" | head -n 11 > lv_all_min.csv

    # Combiner les deux fichiers en un seul
    cat lv_all_min.csv lv_all_max.csv > lv_all_minmax.csv

    # Supprimer le fichier temporaire
    rm lv_all_max.csv lv_all_min.csv

    # Trier par quantité d'énergie consommée en trop des 20 lignes
    sort -n -t':' -k4 lv_all_minmax.csv | head -n 20 > tmp/tmp.csv

    # Ajouter l'en-tête et déplacer le fichier final
    echo "Station:Capacite:Consommation:Difference" > lv_all_minmax.csv
    cat tmp/tmp.csv >> lv_all_minmax.csv

    # Supprimer le fichier temporaire
    rm tmp/tmp.csv
fi

# Stocker le temps de fin
fin2=$(date +%s)

# Calculer le temps écoulé
temps2=$(( fin2 - debut2 ))

temps_total=$(( temps1 + temps2 ))

# Afficher le résultat avec un formatage plus clair
echo "L'exécution du script a duré $temps_total secondes."
