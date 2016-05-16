#include "queue.h"

int msgid = -1;
int running = 1;
char * field;
int x, y;

void signal_handler(int sig) {
	printf("Recieved %d\n", sig);
	running = 0;
   	msgctl (msgid, IPC_RMID, NULL);
   	free(field);
}

void printField(char * field){
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++) {
			printf("%c", field[i*x+j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]) {
	// Buffer for Message
	message_t msg;
	
	int c;

	// Signal handlers
	if(signal(SIGHUP, signal_handler) == SIG_ERR) printf("\ncan't catch SIGHUP\n");
	if(signal(SIGINT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGINT\n");
	if(signal(SIGQUIT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGQUIT\n");

	
	// Argument Handling
	if(argc == 5) {
		while((c = getopt(argc, argv, "x:y:")) != EOF) {
	      	switch(c) {
				case 'x':
					x = atoi(optarg) + 2;
					break;
				case 'y':
					y = atoi(optarg) + 2;
					break;
				case '?':
					// unguelgtiges Argument
					// TODO printHelp();
					break;
				default:
					printf("Unknown argument");
	      	}
	   }
	} else {
		printf("Not enough arguments");
		return EXIT_FAILURE;
		// TODO printHelp();
	}

	
	// for (int k = 0; k < x; k++) {
	// 	field[k] = '#';
	// 	field[y*x+k] = '#';

	// 	field[((y-1)*x+1+k)]

	// }
	
	// Create field
	field = malloc(sizeof(char)*x*y);
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++) {
			if(i == 0 || j == 0 || i == y-1 || j == x-1){
				field[i*x+j] = '#';
			} else {
				field[i*x+j] = ' ';
			}
		}
	}
	
	printField(field);

	// Create Message Queue
	if((msgid = msgget(KEY, PERM | IPC_CREAT | IPC_EXCL )) == -1) {
		// error handling 
		fprintf(stderr, "%s: Error creating message queue\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Get message
	while (running) {

		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), 0, 0) == -1) {
			// error handling
			if(running == 1)
				fprintf(stderr, "%s: Can't receive from message queue\n", argv[0]);
			return EXIT_FAILURE;
		}
		printf("Message received: %s\n", msg.mText);
	}

	free(field);

	msgctl (msgid, IPC_RMID, NULL);
	return EXIT_SUCCESS;
}


