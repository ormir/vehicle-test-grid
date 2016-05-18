#include "queue.h"
#include <pthread.h>

int msgid = -1;
message_t msg;
int channel;
char *program;
int threadRunning = 1;
pthread_mutex_t lock;

void *listenMsg(void *args) {
	// Get message
	while (1) {
		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), channel, 0) == -1) {
			// error handling
			sleep(1);			
			// fprintf(stderr, "%s: Can't receive from message queue\n", program);
		}

		// Terminate signal
		if(msg.mText[1] == 't'){
			msg.mText[0] = '\0';
			msg.mText[1] = '\0';
			exit(0);
		}
		
		printf("%s\n", msg.mText);
	}
    pthread_exit(0);
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

	// Get message queue
	if((msgid = msgget(KEY, PERM)) == -1) {
		// error handling 
		fprintf(stderr, "%s: Can't access message queue\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Send message
	msg.mType = 1;

	sprintf(msg.mText, "-c %c", *argv[1]);

	if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
		// error handling 
		fprintf(stderr, "%d: Can't send message\n", channel);
		return EXIT_FAILURE;
	}

	pthread_t thread;
	long t = 0;
	// int rc = pthread_create(&thread, NULL, listenMsg, &t);
    pthread_create(&thread, NULL, listenMsg, &t);
    
	while(threadRunning) {
		scanf("%c", dir);
        char tmp = dir[0];
		msg.mType = 1;
		sprintf(msg.mText, "-m -n %c %c ", channel, tmp);
		if(threadRunning && msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
			// error handling 
			fprintf(stderr, "%d: Can't send message\n", channel);
		}
	}

	return EXIT_SUCCESS;
}