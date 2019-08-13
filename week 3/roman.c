#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bigRoma(int num){

}

int main(int argc, char *argv[])
{
    if(argc != 2)
        {
                fprintf(stderr, "%s: program expected 2 arguments, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            int count = atoi(argv[1]);
            for (int i = 1; i <= count; i++)
            {
                //Check if it's greatter than 1000
                if(i>4999){
                    bigRoma(i/1000);
                    roma()
                }

            }
            
        }
        
        
}
