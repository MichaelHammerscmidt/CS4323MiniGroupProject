#include "process.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *filename = "bookInfo1.txt";
    char *column = "Stock";
    char *uniqueValue = "In stock";

    int row = 0;
    int col = 0;

    char ***recordArray = readFileByUniqueValue(filename, column, uniqueValue, &row, &col);


    for (int i = 1; i < row; i++) {
        for (int j = 0; j < col; j++) { 
            printf("%d. %s\n", (j + 1), recordArray[0][j]);
            
            printf("%s\n\n", recordArray[i][j]);
        }
        printf("---------------------------------\n");
    }

}