# Nom de l'exécutable
TARGET = mon_programme

# Compilateur
CC = gcc

# Fichier source
SRCS = mon_programme.c

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
