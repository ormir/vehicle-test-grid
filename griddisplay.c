#include <stdio.h>
#define MAX 256
int x, y;


void printField(char * field){
    for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++) {
            printf("%c", field[i*x+j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
   FILE *fp;
   char puffer[MAX];

   if ((fp = fopen("display", "r")) != NULL) {
      while (fgets(puffer, MAX, fp) != NULL) 
      {
         printf("%s",puffer);
      }
   }
   pclose (fp); 
   return 0;
} 

