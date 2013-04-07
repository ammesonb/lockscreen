CC=gcc
CFLAGS=-c -std=c99 -g -I/usr/include/X11/ -lX11 -lXss -lXext
LDFLAGS=-g -lX11 -lXss -lXext

all:
	$(CC) $(CFLAGS) lockscreen.c -o lockscreen.o
	$(CC) $(CFLAGS) lockscreenloop.c -o lockscreenloop.o
	$(CC) lockscreen.o $(LDFLAGS) -o lockscreen
	$(CC) lockscreenloop.o $(LDFLAGS) -o lockscreenloop
	rm *.o

clean:
	rm *.o
	rm lockscreen
	rm lockscreenloop
