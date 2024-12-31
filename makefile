CC = gcc
CFLAGS = -Wall -g -std=c99 -I/usr/local/include/SDL2
LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
OBJS = game.o tilemap.o player.o base.o healthbar.o autoshoot-status-indicator.o window.o vector.o 

all: game

game: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
	rm -f $(OBJS)

game.o: src/main.c
	$(CC) -c $< $(CFLAGS) -o $@

tilemap.o: src/tilemap.c
	$(CC) -c $< $(CFLAGS) -o $@

player.o: src/components/player.c
	$(CC) -c $< $(CFLAGS) -o $@

base.o: src/base.c
	$(CC) -c $< $(CFLAGS) -o $@

healthbar.o: src/components/healthbar.c
	$(CC) -c $< $(CFLAGS) -o $@

autoshoot-status-indicator.o: src/components/autoshoot-status-indicator.c
	$(CC) -c $< $(CFLAGS) -o $@

window.o: src/window.c
	$(CC) -c $< $(CFLAGS) -o $@

vector.o: src/vector.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f main $(OBJS)
