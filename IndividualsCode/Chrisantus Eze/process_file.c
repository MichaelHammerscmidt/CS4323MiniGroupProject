//Group Name: Group G
//Name: Chrisantus Eze
//Email: chrisantus.eze@okstate.edu
//Date: 09/25
//The logic to enable the server communicate with processes
//Haven't implemented the inter-process communication process yet. This is the next item to be done

#include "process_file.h"
#include "unique_record_struct.h"
#include "process_data_struct.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

extern struct uniqueRecordStruct uniqueRecordArray;

// int main() {

//     char *filename = "txtfiles/bookInfo1.txt";
//     char *column = "Stock";
//     char *uniqueValue = "In stock";

//     int row = 0;
//     int col = 0;

//     struct uniqueRecordStruct uniqueRecord = readFileByUniqueValue(filename, column, uniqueValue, &row, &col);


// }


struct uniqueRecordStruct readFileByUniqueValue(char* filename, char* column, char* uniqueValue) {

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
                break;
            }

            if (columnIndex == 0 || columnIndex != i) {

                for (int k = 0; k < strlen(pt); k++) {
                    recordArray[row][col][k] = pt[k];
                }

                col++;
            }


            pt = strtok (NULL, ",");
            i++;
        }

        row++;
    }

//     fclose(inFile);

    struct uniqueRecordStruct uniqueRecordArray;
    strcpy(uniqueRecordArray.uniqueValue, uniqueValue);
    uniqueRecordArray.colSize = nCols;
    uniqueRecordArray.rowSize = row;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            strcpy(uniqueRecordArray.recordArray[i][j], recordArray[i][j]);
        }
    }

    return uniqueRecordArray;
}

void sendDataToParent(struct processDataStruct data, struct uniqueRecordStruct uniqueRecord) {
    strcat(data.uniqueValue, ".txt");
    FILE *outFile = fopen(data.uniqueValue, "w");
    if (!outFile) {
        perror("fopen for writing");
        exit(EXIT_FAILURE);
    }

    printf("File has been opened\n");
    for (int i = 0; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) {
            fprintf(outFile, "%s", uniqueRecord.recordArray[i][j]);

            if ((j + 1) < uniqueRecord.colSize) {
                fputc(',', outFile);
            }
        }        
        fputc('\n', outFile);
    }

    fclose(outFile);
}

struct uniqueRecordStruct receiveDataFromProcess(char filename[20], char column[20], char uniqueValue[20]) {
    FILE *inFile = fopen(filename, "r");
    if (!inFile) {
        perror("fopen for reading");
        exit(EXIT_FAILURE);
    }

    char *line = (char*)malloc(sizeof(char));
    int row = 0;
    int col = 0;

    char recordArray[705][7][300];
    while (fscanf(inFile, "%[^\n] ", line) != EOF) {

        char *pt = strtok(line, ",");

        col = 0;
        while (pt != NULL) {
            strcpy(recordArray[row][col], pt);
            // printf("recordArray[%d][%d]: %s\n", row, col, recordArray[row][col]);

            col++;
            pt = strtok (NULL, ",");
        }

        row++;
    }

    struct uniqueRecordStruct uniqueRecordArray;
    strcpy(uniqueRecordArray.uniqueValue, uniqueValue);
    uniqueRecordArray.colSize = col;
    uniqueRecordArray.rowSize = row;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            strcpy(uniqueRecordArray.recordArray[i][j], recordArray[i][j]);
            printf("recordArray[%d][%d]: %s\n", row, col, recordArray[i][j]);
        }
    }

    return uniqueRecordArray;
}