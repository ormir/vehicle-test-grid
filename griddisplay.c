#include <stdio.h>
#define MAX 256
int x, y;

int main(int argc, char* argv[]) {
   FILE *fp;
   char puffer[MAX];

   // get initial x, y
   if ((fp = fopen("display", "r")) != NULL) {
      if (fgets(puffer, MAX, fp) != NULL) {
        sscanf(puffer, "%d %d", &x, &y);
      }

      while (fgets(puffer, x+1, fp) != NULL) {
         printf("%s\n", puffer);
      }

   }

   while (1) {
	   	if ((fp = fopen("display", "r")) != NULL) {
	      while (fgets(puffer, x+1, fp) != NULL) {
	         printf("%s\n", puffer);
	      }

	   }
   }

   pclose (fp); 
   return 0;
} 

