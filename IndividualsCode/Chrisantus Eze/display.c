#include "process_comm.c"
#include "unique_record_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern struct uniqueRecordStruct uniqueRecord;

int main() {
    char *filename = "bookInfo1.txt";
    char *column = "Stock";
    char *uniqueValue = "Out of stock";

    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);


    for (int i = 1; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) { 
            printf("%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);
            
            printf("%s\n\n", uniqueRecord.recordArray[i][j]);
        }
        printf("---------------------------------\n");
    }

}