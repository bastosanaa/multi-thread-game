CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
INCLUDES = -Iinclude
SRC = src
OBJ = obj

SRCS = $(SRC)/main.c \
       $(SRC)/jogo.c \
       $(SRC)/helicoptero.c \
       $(SRC)/bateria.c \
       $(SRC)/foguete.c \
       $(SRC)/interface.c \
       $(SRC)/sincronizacao.c

OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)

TARGET = game_exec

.PHONY: all clean

all: $(TARGET)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -rf $(OBJ) $(TARGET)
