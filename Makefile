TARGET = game.exe
SRC = main.c player.c  map.c
CC = gcc

INCLUDE_DIR = include
LIB_DIR = lib


CFLAGS = -Wall -std=c99 -I./include
LDFLAGS = -L./lib -lraylib -lm -lopengl32 -lgdi32 -lwinmm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)


run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET).exe
