# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Iinclude

# Source files and executable
SRCS = src/main.c src/fonction.c
OBJS = $(SRCS:.c=.o)
TARGET = projet_exe

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
