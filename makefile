# Nom de l'exécutable
TARGET = main

# Compilateur
CC = gcc

# Fichier source
SRCS = main.c

# Fichiers objets (dérivés des sources)
OBJS = $(SRCS:.c=.o)

# Commandes par défaut
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Règle pour générer les fichiers objets
%.o: %.c
	$(CC) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -f $(OBJS) $(TARGET)
