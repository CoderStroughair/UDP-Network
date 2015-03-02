
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <time.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    srand(time(NULL));
    int i=0;
    int count=0;
    char letter;
    FILE *fp;
    //reads in file into buffer
    fp = fopen("Ascii.txt", "w");
    fclose(fp);
    
    fp = fopen("Ascii.txt", "a");
    while(count<1024)
    {
    	i=rand()%25;
    	i+=65;
    	if((rand()%2)==1)
    	{
    	     i+=32;
    	}
    	letter=i;
    	fprintf(fp,"%c",letter);
    	count++;
    }
    fclose(fp);
    
    
    return 0;
}
   
