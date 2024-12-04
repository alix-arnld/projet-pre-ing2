#!/bin/bash

# Fonction d'aide
function afficher_aide() {
    echo "Usage : $0"
    echo
    echo "Ce script collecte des informations sur un fichier CSV, un type de station,"
    echo "un type de consommateur et, éventuellement, un identifiant de centrale."
    echo
    echo "Options :"
    echo "  --help, -h       Affiche cette aide et quitte le script."
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
    echo "    Entrez le chemin du fichier CSV : /home/user/projet"
    echo "    Entrez le type de station (hvb, hva ou lv) : hva"
    echo "    Entrez le type de consommateur (comp, indiv ou all) : comp"
    echo "    Entrez l'identifiant de la centrale (optional) : 123"
    echo
    echo "Note : Si une combinaison invalide est détectée, le script vous demandera de réessayer."
    exit 0
}

if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    afficher_aide
    exit 0
fi

# Boucle pour vérifier que le chemin d'acces saisi mene vers le fichier source ***.csv
while true; do
    read -p "Entrez le chemin du fichier CSV : " chemin_fichier
    if [[ -f "$chemin_fichier"/c-wire_v00.csv ]]; then
        break
    else
        echo "Pas de fichier c-wire_v00 via ce chemin. Veuillez réessayer."
    fi
done

# Boucle pour vérifier le type de station saisi est valide
while true; do
    read -p "Entrez le type de station (hvb, hva ou lv) : " type_station
    if [[ "$type_station" =~ ^(hvb|hva|lv)$ ]]; then
        break
    else
        echo "Type de station invalide. Veuillez réessayer."
    fi
done

# Boucle pour vérifier le type de consommateur saisi est valide
while true; do
    read -p "Entrez le type de consommateur (comp, indiv ou all) : " type_consommateur
    if [[ "$type_consommateur" =~ ^(comp|indiv|all)$ ]]; then   
    # Vérification des combinaisons interdites
        if [[ "$type_station" =~ ^(hvb|hva)$ ]] && [[ "$type_consommateur" =~ ^(all|indiv)$ ]]; then
            echo "Erreur : Les combinaisons 'hvb all' 'hvb all' 'hva all' et 'hva indiv' sont toutes interdites. Veuillez réessayer"
        else 
            break
        fi
    else
        echo "Type de consommateur invalide. Veuillez réessayer."
    fi
done

# Demande l'identifiant d'une centrale
read -p "Entrez l'identiant de la centrale (optional): " identifiant_centrale

# construcion du fichier de capacites en fonction des parametres
if [[ -n "$identifiant_centrale" ]]; then
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 == "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 != "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
        else
            if [[ "$type_station" == "lv" ]]; then
                awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $4 != "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
            fi
        fi
    fi
else
     if [[ "$type_station" == "hvb" ]]; then
        awk -F';' '$2 != "-" && $3 == "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' '$2 != "-" && $3 != "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
        else
            if [[ "$type_station" == "lv" ]]; then
                awk -F';' '$4 != "-" && $7 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_capacites.csv
            fi
        fi
    fi
fi

# Ajouter l'en-tête au fichier de sortie
echo "Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load" > temp.csv
cat donnees_capacites.csv >> temp.csv
mv temp.csv donnees_capacites.csv


# construction du fichier source filtre en fonction des parametres

if [[ -n "$identifiant_centrale" ]]; then
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 != "-" && $3 == "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
        else
            if [[ "$type_station" == "lv" && "$type_consommateur" == "indiv" ]]; then
                awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 == "-" && $5 == "-" && $6 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
            else
                awk -F';' -v var1="$identifiant_centrale" '$1 == var1 && $2 == "-" && $3 == "-" && $4 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
            fi
        fi
    fi
else
    if [[ "$type_station" == "hvb" ]]; then
        awk -F';' '$2 != "-" && $3 == "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
    else 
        if [[ "$type_station" == "hva" ]]; then
            awk -F';' '$2 == "-" && $3 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
        else
            if [[ "$type_station" == "lv" && "$type_consommateur" == "indiv" ]]; then
                awk -F';' '$2 == "-" && $3 == "-" && $4 != "-" && $6 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
            else
                awk -F';' '$2 == "-" && $3 == "-" && $4 != "-" && $8 != "-"' "$chemin_fichier"/c-wire_v00.csv > donnees_filtrees.csv
            fi
        fi
    fi
fi

# Ajouter l'en-tête au fichier de sortie
echo "Power plant;HV-B Station;HV-A Station;LV Station;Company;Individual;Capacity;Load" > temp.csv
cat donnees_filtrees.csv >> temp.csv
mv temp.csv donnees_filtrees.csv

# RESTE A FAIRE
# on verifie si l executable existe sinon on le complile via un makefile
# on passe le fichier de donnnes filtres et le fichier de capacite au programme c
# on recupere les resultats du programme c
# on genere le resultat dans le fichier fichier_resultat.csv

# construction du nom du fichier de resultats en fonction des parametres
if [[ -n "$identifiant_centrale" ]]; then
    # Si l'identifiant de centrale est renseigné, on construit un nom de fichier complet
    fichier_resultat="${type_station}_${type_consommateur}_${identifiant_centrale}.csv"
else
    # Sinon, on construit un nom de fichier sans l'identifiant de centrale
    fichier_resultat="${type_station}_${type_consommateur}.csv"
fi
