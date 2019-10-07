#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void money(int total, int *dollars, int *cents){
    *dollars = total/100;
    *cents = total%100;

}

int main(int argcount, char *argvalue[])
{
    int dollar = 0;
    int cents = 0;

    money(524,&dollar,&cents);
    printf("%i dollar(s) and %i cent(s). \n",dollar,cents);
    return 0;
}
