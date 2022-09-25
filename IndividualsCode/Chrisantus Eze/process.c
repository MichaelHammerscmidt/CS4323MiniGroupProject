#include "record_struct.h"
#include "process.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct uniqueRecordStruct {
    int sizeOfVal;
    char* uniqueValue;
    char** field;
    char*** values;
};

extern struct uniqueRecordStruct* uniqueRecordArray;

int row = 0;
int col = 0;

// int main() {

//     char *filename = "bookInfo1.txt";
//     char *column = "Stock";
//     char *uniqueValue = "In stock";

//     char ***recordArray = readFileByUniqueValue(filename, column, uniqueValue, &row, &col);

//     int count = 0;
//     // char*** uniqueRecordArray_ = getByUniqueValue(recordArray, column, "Out of stock", row, col, &count);


//     // char uniqueValues[2][20] = {"In stock", "Out of stock"};
//     // createProcesses(2, uniqueValues, recordArray, column, row, col);
// }

char ***readFileByUniqueValue(char* filename, char* column, char* uniqueValue, int* row_, int* col_) {

    int nRows = 705;
    int nCols = 5;
    int nValues = 300;

    if (strcmp(filename, "amazonBestsellers.txt") == 0) {
        nRows = 555;
        nCols = 6;
    }

    char*** recordArray = (char***)malloc(nRows * sizeof(char**));

    for (int i = 0; i < nRows; i++) {
        recordArray[i] = (char**)malloc(nCols * sizeof(char*));

        for (int j = 0; j < nCols; j++) {
            recordArray[i][j] = (char*)malloc(nValues * sizeof(char));
        }
    }


    FILE *inFile = fopen(filename, "r");
    if (!inFile) {
        perror("fopen for reading");
        exit(EXIT_FAILURE);
    }

    char *line = (char*)malloc(sizeof(char));
    int row = 0;
    int col = 0;

    int columnIndex = 0;

    while (fscanf(inFile, "%[^\n] ", line) != EOF) {

        char *pt = strtok(line, ",");

        col = 0;

        int i = 0;
        while (pt != NULL) {

            if (row == 0 && strcmp(pt, column) == 0) {
                columnIndex = i;
            }


            if (columnIndex != 0 && columnIndex == i && strcmp(pt, uniqueValue) != 0 && strcmp(pt, column) != 0) {
                row--;
                // printf("row = %d, col = %d\n", row, col);
                break;
            }

            if (columnIndex == 0 || columnIndex != i) {

                for (int k = 0; k < strlen(pt); k++) {
                    recordArray[row][col][k] = pt[k];
                }

//                 printf("recordArray[%d][%d] = %s\n", row, col, recordArray[row][col]);
//                 printf("recordArray[%d][%d] = %s\n", row, col, pt);

                col++;
            }


            pt = strtok (NULL, ",");
            i++;
        }

        row++;
    }
    
//     for (int i = 0; i < row; i++) {
//         for (int j = 0; j < nCols; j++) {
//             printf("recordArray[%d][%d] = %s\n", i, j, recordArray[i][j]);
//         }
//         printf("\n");
//     }

//     fclose(inFile);

    *row_ = row;
    *col_ = nCols;
    return recordArray;
}