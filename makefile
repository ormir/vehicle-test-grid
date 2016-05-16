CC = gcc
CFLAGS = -Wall -std=c99

all: gridserver vehicleclient griddisplay

gridserver: gridserver.o queue.h
		$(CC) $(CFLAGS) gridserver.o -o gridserver

gridserver.o: gridserver.c
		$(CC) $(CFLAGS) -c gridserver.c

vehicleclient: vehicleclient.o queue.h
		$(CC) $(CFLAGS) vehicleclient.o -o vehicleclient

vehicleclient.o: vehicleclient.c
		$(CC) $(CFLAGS) -c vehicleclient.c

griddisplay: griddisplay.o queue.h
		$(CC) $(CFLAGS) griddisplay.o -o griddisplay

griddisplay.o: griddisplay.c
		$(CC) $(CFLAGS) -c griddisplay.c
		
clean:
	rm *.o gridserver vehicleclient griddisplay