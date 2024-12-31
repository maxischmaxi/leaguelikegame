CC = gcc
CFLAGS = -Wall -g -std=c99 -I/usr/local/include/SDL2
LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image
OBJS = game.o tilemap.o player.o keyboard.o base.o

all: game

game: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

game.o: src/main.c
	$(CC) -c $< $(CFLAGS) -o $@

tilemap.o: src/tilemap.c
	$(CC) -c $< $(CFLAGS) -o $@

player.o: src/player.c
	$(CC) -c $< $(CFLAGS) -o $@

keyboard.o: src/keyboard.c
	$(CC) -c $< $(CFLAGS) -o $@

base.o: src/base.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f main $(OBJS)
