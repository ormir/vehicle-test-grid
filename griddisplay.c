#include <stdio.h>
#define MAX 256

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

