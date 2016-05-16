#include "queue.h"

int main(int argc, char const *argv[]) {
	message_t msg;
	msg.mText = "Hallo";
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
	msg.mType=1;
	strncpy(msg.mText,argv[1],MAX_DATA);
	if(msgsnd(msgid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
		// error handling 
		fprintf(stderr, "%s: Can't send message\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("Message sent: %s\n", msg.mText);
	return EXIT_SUCCESS;

	// Get message
	while (1) {
		if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), 0, 0) == -1) {
			// error handling
			fprintf(stderr, "%s: Can't receive from message queue\n", argv[0]);
			sleep(1);
			return EXIT_FAILURE;
		}
		printf("Message received: %s\n", msg.mText);
	}

	return 0;
}