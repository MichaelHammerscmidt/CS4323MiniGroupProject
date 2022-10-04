//Group G
//Rebecca Wasserott
//rebecca.wasserott@okstate.edu
//09/25
//The logic to enable the processes save the read files into a file saved in the local storage.
//The save function is included in the server.c file submitted 

//#include "process.c"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main() {

void save(char column[20], char uniqueValue[20], int socket) {
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    FILE *outFile = fopen(filename, "w");
    printf("snd: filename -> %s\n", filename);

    if (outFile == NULL || !outFile) {
      printf("There was an error opening the file for writing\n");
      perror("fopen for writing");
      exit(EXIT_FAILURE);
    }

    for (int i = 1; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) { 
            fprintf(outFile, "%s\n", uniqueRecord.recordArray[0][j]);

            fprintf(outFile, "%s\n", uniqueRecord.recordArray[i][j]);
        }
        printf("---------------------------------\n");
    }
    fclose(outFile);
}

}
