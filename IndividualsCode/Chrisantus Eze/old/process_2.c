#include "record_struct.h"
#include "process.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



#include <stdio.h> 	// Baisc standard I/O like printf
#include <unistd.h> 	// defines standard symbolic constants and types
#include <sys/types.h> 	// basic derived types like pid_t
#include <sys/stat.h> 	//required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h> 	//required for open (i.e. file control options)

struct uniqueRecordStruct {
    int sizeOfVal;
    char* uniqueValue;
    char** field;
    char*** values;
};

extern struct uniqueRecordStruct* uniqueRecordArray;

int row = 0;
int col = 0;

int main() {

    char *filename = "bookInfo1.txt";
    char *column = "Stock";

    char ***recordArray = readFile(filename, column, &row, &col);

    int count = 0;
    // char*** uniqueRecordArray_ = getByUniqueValue(recordArray, column, "Out of stock", row, col, &count);


    // char uniqueValues[2][20] = {"In stock", "Out of stock"};
    // createProcesses(2, uniqueValues, recordArray, column, row, col);
}

char ***readFile(char* filename, char* column, int* row_, int* col_) {

    int nRows = 705;
    int nCols = 6;
    int nValues = 300;

    if (strcmp(filename, "amazonBestsellers.txt") == 0) {
        nRows = 555;
        nCols = 7;
    }

    // Each record has a uniqueValue
    // struct recordStruct *recordArray = (struct recordStruct*)malloc(nRows * sizeof(struct recordStruct));
    // for (int i = 0; i < nRows; i++) {
    //     recordArray[i].uniqueValue = (char*)malloc(nValues * sizeof(char));

    //     recordArray[i].values = (char**)malloc(nCols * sizeof(char*));
    //     for (int j = 0; j < nCols; j++) {
    //         recordArray[i].values[j] = (char*)malloc(nValues * sizeof(char));
    //     }
    // }

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
        while (pt != NULL) {

            for (int k = 0; k < strlen(pt); k++) {
                recordArray[row][col][k] = pt[k];
                // printf("recordArray[%d].values[%d][%d] = %c\n", row, col, k, recordArray[row][col][k]);
            }

            // recordArray[row][col] = pt;
            printf("recordArray[%d].values[%d] = %s\n", row, col, recordArray[row][col]);

            // recordArray[row].values[col] = pt;

            // if (row == 0) {
            //     if (strcmp(pt, column) == 0) {
            //         columnIndex = col;
            //     }

            // } else {
            //     if (col == columnIndex) {
            //         for (int k = 0; k < strlen(pt); k++) {
            //             recordArray[row].uniqueValue[k] = pt[k];
            //         }
            //     }

            //     // printf("recordArray[%d].values[%d] = %s\n", row, col, recordArray[row].values[col]);
            // }
            
            pt = strtok (NULL, ",");
            col++;
        }

        row++;
    }

    // fclose(inFile);

    *row_ = row;
    *col_ = col;
    return recordArray;

}

/*

char*** getByUniqueValue(struct recordStruct* recordArray, char* column, char* uniqueValue, int row, int col, int *count) {

    char*** uniqueRecordArray_ = (char***)malloc(row * sizeof(char**));
    for (int i = 0; i < row; i++) {
        uniqueRecordArray_[i] = (char**)malloc(col * sizeof(char*));

        for (int j = 0; j < col; j++) {
            uniqueRecordArray_[i][j] = (char*)malloc(300 * sizeof(char));
        }
    }

    int colCount = 0;
    int colIndex = 0;
    for (int j = 0; j < col; j++) {
        if (strcmp(column, recordArray[0].values[j]) != 0) {
            uniqueRecordArray_[0][colCount] = recordArray[0].values[j];
            colCount++;
        } else {
            colIndex = j;
        }
    }

    int rowCount = 1; 
    for (int i = 1; i < row; i++) {
        colCount = 0;
        if (strcmp(recordArray[i].uniqueValue, uniqueValue) == 0) {
            for (int j = 0; j < col; j++) {
                if (colIndex != j) {
                    uniqueRecordArray_[rowCount][colCount] = recordArray[i].values[j];
                    // printf("uniqueRecordArray[%d][%d] = %s\n", rowCount, colCount, uniqueRecordArray_[rowCount][colCount]); 
                    colCount++;
                } 
            }
            rowCount++;
        }
    }

    *count = rowCount;
    return uniqueRecordArray_;
}

char*** getRecordsByUniqueValue(char* filename, char* column, char* uniqueValue, int *row_, int *col_) {
    int count = 0;
    if (recordArray == NULL) {
        recordArray = readFile(filename, column, &row, &col);
    }

    char*** recordsByUniqueValue = getByUniqueValue(recordArray, column, uniqueValue, row, col, &count);
    *row_ = count;
    *col_ = col-1;

    return recordsByUniqueValue;
}

*/