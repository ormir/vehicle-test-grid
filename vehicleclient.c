#include "queue.h"

int main(int argc, char const *argv[]) {
	message_t msg;
	int msgid = -1;

	// Argument Handling
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <Message>\n", argv[0]);
		return EXIT_FAILURE;
	}	

	// Get message queue
	if((msgid = msgget(KEY, PERM)) == -1) {
		// error handling 
		fprintf(stderr, "%s: Can't access message queue\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Send message
	msg.mType = 1;
	char *c = malloc(sizeof(char) * 4);	
	strcat(c, "-c ");
	char tmp = *argv[1];
	if(tmp < 'A' || tmp > 'Z') return EXIT_FAILURE;	
	
	c[3] = tmp;	
	// strcat(c, "-c ");
	// strcat(c, &tmp);

	strncpy(msg.mText, c, MAX_DATA);
	if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), CREATE) == -1){
		// error handling 
		fprintf(stderr, "%s: Can't send message\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("Name sent: %s\n", msg.mText);

	// Get message
	while (1) {
		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), 0, *argv[1]) == -1) {
			// error handling
			sleep(1);
			fprintf(stderr, "%s: Can't receive from message queue\n", argv[0]);			
			return EXIT_FAILURE;
		}		
		printf("Message received: %s\n", msg.mText);
	}

	return EXIT_SUCCESS;
}