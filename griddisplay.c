#include <stdio.h>
#define MAX 256
int x;

int main(int argc, char* argv[]) {
   FILE *fp;
   char puffer[MAX];

   // get initial x, y
   if ((fp = fopen("display", "r")) != NULL) {
    	if (fgets(puffer, MAX, fp) != NULL) {
    	    sscanf(puffer, "%d", &x);
      	}
   }
  
   // Read mesages from server
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

