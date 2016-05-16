#include "queue.h"
#include <pthread.h>

int msgid = -1;
message_t msg;
int channel;
char *program;

void *listenMsg(void *args) {
	// Get message
	while (1) {
		printf("%d\n", channel);
		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), channel, 0) == -1) {
			// error handling
			sleep(1);			
			fprintf(stderr, "%s: Can't receive from message queue\n", program);
		}		
		printf("%s\n", msg.mText);
	}
}

void *printhello(void *arg){
	long tid;
	tid = (long) arg;
	printf("hello\n");
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

	char dir = ' ';

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
	
	msg.mType = 1;
	if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
		// error handling 
		fprintf(stderr, "%s: Can't send message\n", channel);
		return EXIT_FAILURE;
	}

	pthread_t thread;
	long t = 0;
	int rc = pthread_create(&thread, NULL, listenMsg, &t);
	
	while(1) {
		scanf("%c",&dir);		
	}

	// listenMsg(argv[1],argv[1]);

	return EXIT_SUCCESS;
}