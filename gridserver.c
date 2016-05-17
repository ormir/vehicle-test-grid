#include "queue.h"
#include <pthread.h>

int msgid = -1;
int running = 1;
char * field;
int x, y;

typedef struct {
    char name;
    int x, y;
    
} car_t;

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
    srand(time(NULL));  
    // Buffer for Message
    message_t msg;
    
    int c;
    int carCount = 0;

    car_t cars[26];
    
<<<<<<< HEAD
    char dir = ' '; 
    
    // Argument Handling
    if(argc != 5) {
        fprintf(stderr, "Usage: %s <Message>\n", argv[0]);
=======
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
>>>>>>> master
        return EXIT_FAILURE;
        // TODO printHelp();
    }

    
<<<<<<< HEAD
    program = argv[0];
    channel = *argv[1];
     
    // Get message queue
    if((msgid = msgget(KEY, PERM)) == -1) {
        // error handling
        fprintf(stderr, "%s: Can't access message queue\n", argv[0]);
        return EXIT_FAILURE;
=======
    // for (int k = 0; k < x; k++) {
    //  field[k] = '#';
    //  field[y*x+k] = '#';

    //  field[((y-1)*x+1+k)]

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
>>>>>>> master
    }
    
    printField(field);

    // Create Message Queue
    if((msgid = msgget(KEY, PERM | IPC_CREAT | IPC_EXCL )) == -1) {
        // error handling 
        fprintf(stderr, "%s: Error creating message queue\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get message
    while (1) {

        if (msgrcv(msgid, &msg, sizeof(msg)-sizeof(long), 1, 0) == -1) {
            // error handling
            if(running == 1)
                fprintf(stderr, "%s: Can't receive from message queue\n", argv[0]);
            return EXIT_FAILURE;
        }
        // decode arguments in message
        if(msg.mText[1] == 'c') {                       
            int posX = 0;
            int posY = 0;

            // get start pos
            while(field[posY*x + posX] != ' ' && carCount < 26) {
                posX = (rand() % x-2) + 1;
                posY = (rand() % x-2) + 1;
            }
            if(posX == 0 || posY == 0){
                // send message didnt work
                continue;
            }

            char c = msg.mText[3];
            // fill car with data
            car_t car;          
            car.name = c;
            car.x = posX;
            car.y = posY;

            int i = 'A' - c;
            cars[i] = car;          
            
            field[posY*x + posX] = c;

            printField(field);

            message_t msg_r;
            msg_r.mType = car.name;
            
            sprintf(msg_r.mText, "Registration OK. Start position: %d,%d.", posX, posY);
            msgsnd(msgid, &msg_r, sizeof(msg_r)-sizeof(long), 0);
        }

        printf("Message received: %s\n", msg.mText);
        // cars []
    }
}
