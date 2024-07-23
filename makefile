CC = gcc
CFLAGS = -D_REENTRANT -I/usr/include/SDL2
LIBS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f main

