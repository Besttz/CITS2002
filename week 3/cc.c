#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc <= 2)
        {
                fprintf(stderr, "%s: program expected 2 arguments, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            int s1 = 0;
            int s2 = 0;
            for (int i = 0; i <argc-1 ; i++)
            {
                if (i%2==0)
                {
                    s1 += atoi(argv[argc-1-i]);
                    //When it's odd
                } else {
                    //When it's even
                    s2 += atoi(argv[argc-1-i]);
                }
                
            }
            if ((s1+s2)%10==0)
            {
                printf("Vaild Credit Card Number\n");
                exit(EXIT_SUCCESS);
            } else {
                printf("Non-Vaild Credit Card Number\n");
                exit(EXIT_SUCCESS);
            }
        }
        return 0;
}
