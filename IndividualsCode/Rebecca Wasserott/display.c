//Group G
//Rebecca Wasserott
//rebecca.wasserott@okstate.edu
//09/25
//The logic to enable the server to display the contents of the struct array on the client
//Not finished. This is for the progress report.
 
#include "process.c"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main() {
 
//    //struct definition
//    typedef struct{
//        //dynamic array of strings
//        char **data;
//    }
//    DataColumn;
 
//    typedef struct{
//        //dynamic array of strings for the column headers
//        char** colHeaders;
//        //dynamic array of datacolumns
//        DataColumn *cloumns;
 
//        int numRows;
//        int numCols;
//    }
//    DataTable;
 
//    char *filename = "bookInfo1.txt";
//    char *column = "Stock";
//    char *uniqueValue = "Out of stock";
 
//    int row = 0;
//    int col = 0;
 
//    char*** recordsArray = getRecordsByUniqueValue(filename, column, uniqueValue, &row, &col);
 
//    for (int i = 1; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            printf("%d. %s\n", (j + 1), recordsArray[0][j]);
          
//            printf("%s\n\n", recordsArray[i][j]);
//        }
//        printf("---------------------------------\n");
//    }
 
// }

void display(char column[20], char uniqueValue[20], int socket) {
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    for (int i = 1; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) { 
            // printf("%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);
            send(socket, uniqueRecord.recordArray[0][j], strlen(uniqueRecord.recordArray[0][j]), 0);

            
            // printf("%s\n\n", uniqueRecord.recordArray[i][j]);

            send(socket, uniqueRecord.recordArray[i][j], strlen(uniqueRecord.recordArray[i][j]), 0);

        }
        // printf("---------------------------------\n");
    }
}

