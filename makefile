CC = gcc
CFLAGS = -Wall -std=c99

all: gridserver vehicleclient

gridserver: gridserver.o queue.h
		$(CC) $(CFLAGS) gridserver.o -o gridserver

gridserver.o: gridserver.c
		$(CC) $(CFLAGS) -c gridserver.c

vehicleclient: vehicleclient.o queue.h
		$(CC) $(CFLAGS) vehicleclient.o -o vehicleclient

vehicleclient.o: vehicleclient.c
		$(CC) $(CFLAGS) -c vehicleclient.c