#include "queue.h"
#include <pthread.h>

int msgid = -1;
message_t msg;
int channel;
char const *program;
pthread_mutex_t lock;

void *listenMsg(void *args) {
	// Get message
	while (1) {
		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), channel, 0) == -1) {
			sleep(1);
		}
	}
    pthread_exit(0);
}

void signal_handler(int sig) {
    printf("Vechicle has been eleminated\n");
    exit(0);
}

int main(int argc, char const *argv[]) {
	char dir[2];

	// Argument Handling
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <Message>\n", argv[0]);
		return EXIT_FAILURE;		
	}	

	program = argv[0];
	channel = *argv[1];

	// Signal Handling
	if(signal(SIGTERM, signal_handler) == SIG_ERR) printf("\ncan't catch SIGTERM\n");

	// Get message queue
	if((msgid = msgget(KEY, PERM)) == -1) {
		// error handling 
		fprintf(stderr, "%s: Can't access message queue\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Send message
	msg.mType = 1;

	sprintf(msg.mText, "-c %c %d", *argv[1], getpid());

	if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
		// error handling 
		fprintf(stderr, "%d: Can't send message\n", channel);
		return EXIT_FAILURE;
	}

	pthread_t thread;
	long t = 0;
	// int rc = pthread_create(&thread, NULL, listenMsg, &t);
    pthread_create(&thread, NULL, listenMsg, &t);
    
	while(1) {
		scanf("%s", dir);
        char tmp = dir[0];
		msg.mType = 1;
		
		// process terminates
		if(dir[0] == 'T'){
			sprintf(msg.mText, "-T %c", channel);
			msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0);
			return EXIT_SUCCESS;			
		}

		sprintf(msg.mText, "-m -n %c %c ", channel, tmp);
		if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
			// error handling 
			fprintf(stderr, "%d: Can't send message\n", channel);
		}
	}

	return EXIT_SUCCESS;
}