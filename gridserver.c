#include "queue.h"
#include <pthread.h>

int msgid = -1;
int running = 1;
char * field;
int x, y;
char msg_send[MAX_DATA];

typedef struct {
    char name;
    int x, y;
    
} car_t;

void signal_handler(int sig) {
    // printf("Recieved %d\n", sig);
    
    // Quit all car processes

    // Delete message queue
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

void sendMessage(char* msg, char destination) {
     printf("Sending message: %s\n", msg);
    // Construct message
    message_t msg_r;
    msg_r.mType = destination;
    sprintf(msg_r.mText, msg);

    msgsnd(msgid, &msg_r, sizeof(msg_r)-sizeof(long), 0);

    // Clean message
    for (int i = 0; i < MAX_DATA; ++i)
        msg_send[i] = '\0';
}



int main(int argc, char* argv[]) {
    srand(time(NULL));  
    // Buffer for Message
    message_t msg;
    
    int carCount = 0;

    car_t cars[26];
    for(int i = 0; i < 26; i++){
        cars[i].name = '#';
    }
    
    // Signal handlers
    if(signal(SIGHUP, signal_handler) == SIG_ERR) printf("\ncan't catch SIGHUP\n");
    if(signal(SIGINT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGINT\n");
    if(signal(SIGQUIT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGQUIT\n");

    
    // Argument Handling
    if(argc == 5) {
        int c;
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

            // TODO send message didnt work
            if(posX == 0 || posY == 0){
                continue;
            }

            char c = msg.mText[3];
            // fill car with data
            car_t car;          
            car.name = c;
            car.x = posX;
            car.y = posY;

            int i = c - 'A';
            cars[i] = car;          
            
            field[posY*x + posX] = c;
            printField(field);

            // Send OK message
            sprintf(msg_send, "Registration OK. Start position: %d,%d.", posX, posY);
            sendMessage(msg_send, car.name);
        } else if(msg.mText[1] == 'm') { // if message is "-m ..." make move
            // mtext[6] contains car letter - A to get index in car array
            char cletter = msg.mText[6];
            int car = cletter - 'A';
            char dir = msg.mText[8];
            
            int cx = cars[car].x;
            int cy = cars[car].y;

            field[cy*x + cx] = ' ';

            switch(dir){
                case 'N': 
                    cy--;
                    break;
                case 'E':
                    cx++;
                    break;
                case 'S':
                    cy++;
                    break;
                case 'W':
                    cx--;
                    break;
                default:
                    printf("Not recognised direction: %d\n", dir);            
            }

            // Check if car is in border 
            if(field[cy*x + cx] != ' '){
                // collision with wall
                if(field[cy*x + cx] == '#'){
                    cars[car].name = '#';
                    // TODO send kill
                    sprintf(msg_send, "-t");
                    sendMessage(msg_send, cletter);
                } else {
                    cars[car].name = '#';
                    
                    // delete moving car
                    sprintf(msg_send, "-t");
                    sendMessage(msg_send, cletter);

                    // get standing car
                    car = field[cy*x + cx] - 'A';
                    cars[car].name = '#';
                    field[cy*x + cx] = ' ';
                    
                    // delete standing car
                    sprintf(msg_send, "-t");
                    sendMessage(msg_send, car + 'A');
                }                
            } else {
                field[cy*x + cx] = cletter;
                cars[car].x = cx;
                cars[car].y = cy;
            }

            printField(field);
            printf("car: %d, direction: %c \n", car, dir);
        } 

        printf("Message received: %s\n", msg.mText);
    }
}
