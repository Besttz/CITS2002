#include <string.h>
// #include <unistd.h>
#include <stdio.h>  // Printf()
#include <stdlib.h> // Me
// #include <stdlib.h>

/*
Consider a 2-dimensional spreadsheet − a rectangular grid of cells, 
each of which holds an arbitrary string of characters, 
or an empty string if the value of the cell has not yet been defined. 

Each cell is identified by 2 non-negative integer values, 
identifying a specific row and column. 

Define a C99 user-defined data type to represent an instance 
of the 2-dimensional spreadsheet. Next, define and implement three C99 functions:
one to return a pointer to allocated memory suitable for storing and managing the spreadsheet, 
one to swap the contents of two indicated columns in a spreadsheet, and 
one to completely deallocate all memory allocated to a spreadsheet.
*/

#define MAX_SIZE 1000

typedef struct
{
    char ***data;
    int row;
    int column;
} SPREADSHEET;

char ***make(int row, int column)
{
    char ***data = calloc(row * column, sizeof(char *));
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            data[i][j] = calloc(1, MAX_SIZE);
        }
    }
    printf("111 \n");

    return (char ***)data;
}

int swap(SPREADSHEET sheet, int column1, int column2)
{
    char temp[MAX_SIZE];
    for (int i = 0; i < sheet.row; i++)
    {
        strcpy(temp, sheet.data[i][column1]);
        strcpy(sheet.data[i][column1], sheet.data[i][column2]);
        strcpy(sheet.data[i][column2], temp);
    }

    return 0;
}

int freeAll(SPREADSHEET sheet)
{
    for (int i = 0; i < sheet.row; i++)
    {
        for (int j = 0; j < sheet.column; j++)
        {
            if (sheet.data[i][j] != NULL)
                free(sheet.data[i][j]);
        }
    }
    free(sheet.data);
    return 0;
}

int main(int argc, char *argv[])
{
    SPREADSHEET aaa = {make(1, 1), 1, 1};
    printf("111 \n");
    char *new = "Hello";
    printf("%s \n", new);
    printf("111 \n");
    printf("%s \n", **(aaa.data));
    strcpy(**(aaa.data), new);
    printf("%s\n", aaa.data[0][0]);
}

//  以上由于未知原因 Segmentation fault: 11 无法执行