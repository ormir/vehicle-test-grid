#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>

#define KEY 420257999
#define PERM 0660

#define MAX_DATA 255

typedef struct {
	long mType;
	char mText[MAX_DATA];
} message_t;

typedef struct {
    char name;
    int x, y;
} car_t;

// TODO 
void printHelp(int f) {
	if(f == 1) {
		printf("Server");
	}
}

