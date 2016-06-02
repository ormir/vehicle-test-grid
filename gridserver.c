/*
    Gruppenarbeit Auerbeck, Gjurgjej
*/


#include "queue.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

int msgid = -1;
int running = 1;
char * field;
int x, y;
char msg_send[MAX_DATA];
car_t cars[26];
FILE *fp;

void printField(char * field){
    if ((fp = fopen("display", "w")) != NULL) {
        fprintf(fp, "%s", field);
        fclose(fp);
    }
}

void sendMessage(char* msg, char destination) {
    // Construct message
    message_t msg_r;
    msg_r.mType = destination;
    sprintf(msg_r.mText, "%s",msg);

    msgsnd(msgid, &msg_r, sizeof(msg_r)-sizeof(long), 0);

    // Clean message
    for (int i = 0; i < MAX_DATA; ++i)
        msg_send[i] = '\0';
}

void signal_handler(int sig) {
    // printf("Recieved %d\n", sig);
    
    // Quit all car processes
    for(int i = 0; i < 26; i++){
        if(cars[i].name != '#') {
            kill(cars[i].pid, SIGTERM);
        }
    }

    // Delete message queue
    running = 0;
    msgctl (msgid, IPC_RMID, NULL);
    system("rm display");
    free(field);
    exit(0);
}



int main(int argc, char* argv[]) {
    srand(time(NULL));  
    // Buffer for Message
    message_t msg;
    
    int carCount = 0;

    // car_t cars[26];
    for(int i = 0; i < 26; i++){
        cars[i].name = '#';
    }
    
    // Signal handlers
    if(signal(SIGHUP, signal_handler) == SIG_ERR) printf("\ncan't catch SIGHUP\n");
    if(signal(SIGINT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGINT\n");
    if(signal(SIGQUIT, signal_handler) == SIG_ERR) printf("\ncan't catch SIGQUIT\n");
    if(signal(SIGTERM, signal_handler) == SIG_ERR) printf("\ncan't catch SIGTERM\n");

    
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
                default:
                    printf("Unknown argument");
            }
       }
    } else {
        printf("Not enough arguments");
        return EXIT_FAILURE;
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

    // Create named pipe
    if (mkfifo("display", 0660) == -1) {
        fprintf(stderr,"myfifo: Error creating display\n");
        return EXIT_FAILURE;
    }

    if ((fp = fopen("display", "w")) != NULL) {
        fprintf(fp,"%d\n", x);
        fclose(fp);
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
            while(field[posY*x + posX] != ' ' && carCount < (x - 2)*(y - 2)) {
                posX = (rand() % x-2) + 1;
                posY = (rand() % x-2) + 1;
            }

            // fill car with data
            car_t car;
            sscanf(msg.mText, "-c %c %d", &car.name, &car.pid);
            car.x = posX;
            car.y = posY;
            // if field is full
            if(posX == 0 || posY == 0){
                kill(car.pid, SIGTERM);
                continue;
            } else {
                carCount++;
            }
            cars[car.name - 'A'] = car;       

            field[posY*x + posX] = car.name;
            printField(field);

            // Send OK message
            sprintf(msg_send, "Registration OK. Start position: %d,%d.", posX, posY);
            sendMessage(msg_send, car.name);
        } else if(msg.mText[1] == 'm') { // if message is "-m ..." make move
            // mtext[6] contains car letter -'A' to get index in car array
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

            // Check if car is in border or other car
            if(field[cy*x + cx] != ' '){
                // collision with wall
                if(field[cy*x + cx] == '#'){
                    cars[car].name = '#';
                    carCount--;
                    kill(cars[car].pid, SIGTERM);
                } else {
                    cars[car].name = '#';
                    
                    // delete moving car
                    kill(cars[car].pid, SIGTERM);

                    // get standing car
                    car = field[cy*x + cx] - 'A';
                    cars[car].name = '#';
                    field[cy*x + cx] = ' ';
                    
                    // delete standing car
                    kill(cars[car].pid, SIGTERM);

                    carCount -= 2;
                }                
            } else {
                field[cy*x + cx] = cletter;
                cars[car].x = cx;
                cars[car].y = cy;
            }
            printField(field);
        } else if(msg.mText[1] == 'T'){
            int ind = msg.mText[3] - 'A';
            int cx = cars[ind].x;
            int cy = cars[ind].y;

            field[cy*x + cx] = ' ';
            printField(field);
        }
    }
}
