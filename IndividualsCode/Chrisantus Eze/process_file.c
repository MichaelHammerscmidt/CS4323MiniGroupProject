// Group Name: Group G
// Name: Chrisantus Eze
// Email: chrisantus.eze@okstate.edu
// Date: 09/25
// The logic to enable the server communicate with processes
// Haven't implemented the inter-process communication process yet. This is the
// next item to be done

#include "process_file.h"
#include "unique_record_struct.h"
#include "process_data_struct.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <fcntl.h>

extern struct uniqueRecordStruct uniqueRecordArray;

// int main() {

//     char *filename = "bookInfo1.txt";
//     char *column = "Stock";
//     char *uniqueValue = "In stock";

//     int row = 0;
//     int col = 0;

// //     struct uniqueRecordStruct uniqueRecord = readFileByUniqueValue(filename,
// //     column, uniqueValue);
  
//     struct processDataStruct data;
    
//     sendDataToParent(data);
  
//     return 0;

// }

struct uniqueRecordStruct readFileByUniqueValue(char *filename, char *column,
                                                char *uniqueValue) {
  int nRows = 705;
  int nCols = 5;
  int nValues = 300;

  if (strcmp(filename, "amazonBestsellers.txt") == 0) {
    nRows = 555;
    nCols = 6;
  }

  char ***recordArray = (char ***)malloc(nRows * sizeof(char **));

  for (int i = 0; i < nRows; i++) {
    recordArray[i] = (char **)malloc(nCols * sizeof(char *));

    for (int j = 0; j < nCols; j++) {
      recordArray[i][j] = (char *)malloc(nValues * sizeof(char));
    }
  }

  FILE *inFile = fopen(filename, "r");
  if (!inFile) {
    perror("fopen for reading");
    exit(EXIT_FAILURE);
  }

  char *line = (char *)malloc(sizeof(char));
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

      if (columnIndex != 0 && columnIndex == i &&
          strcmp(pt, uniqueValue) != 0 && strcmp(pt, column) != 0) {
        row--;
        break;
      }

      if (columnIndex == 0 || columnIndex != i) {
        for (int k = 0; k < strlen(pt); k++) {
          recordArray[row][col][k] = pt[k];
        }

        col++;
      }

      pt = strtok(NULL, ",");
      i++;
    }

    row++;
  }

  free(inFile);
//   fclose(inFile);

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

void msgsnd_(int msgid, struct processDataStruct data, int size, int pms) {
    snd(data);
}

struct processDataStruct msgrcv_(int msgid, char uniqueValue[20], int size, int pms, int detr) {
    struct processDataStruct data = rcv(uniqueValue);    

    printf("Data received: %s\n", data.uniqueValue);
    return data;
}

void snd(struct processDataStruct data) {
    char filename[30];

    strcpy(filename, data.uniqueValue);
    strcat(filename, ".txt");

    FILE *outFile = fopen(filename, "w");
    printf("snd: filename -> %s\n", filename);

    if (outFile == NULL || !outFile) {
      printf("There was an error opening the file for writing\n");
      perror("fopen for writing");
      exit(EXIT_FAILURE);
    }

    fprintf(outFile, "%s\n", data.uniqueValue);
    fprintf(outFile, "%s\n", data.column);
    fprintf(outFile, "%s\n", data.filename);
    fclose(outFile);
}

struct processDataStruct rcv(char uniqueValue[20]) {
    char filename[20];
    strcpy(filename, uniqueValue);
    strcat(filename, ".txt");

    printf("rcv: filename -> %s\n", filename);
    FILE *inFile = fopen(filename, "r");
    if (!inFile) {
        perror("fopen for reading");
        exit(EXIT_FAILURE);
    }

    char *line = (char *)malloc(sizeof(char));
    int row = 0;
    int col = 0;

    char processData[3][20];
    while (fscanf(inFile, "%[^\n] ", line) != EOF) {
        strcpy(processData[row], line);
        // printf("processData[%d]: %s\n", row, processData[row]);

        row++;
    }

    struct processDataStruct data;

    strcpy(data.uniqueValue, processData[0]);
    strcpy(data.column, processData[1]);
    strcpy(data.filename, processData[2]);
    return data;
}

void sendDataToParent(struct processDataStruct data) {
    printf("sendDataToParent: uniqueValue -> %s\n", data.uniqueValue);
    snd(data);
}

struct uniqueRecordStruct receiveDataFromProcess(char filename[20],
                                                 char column[20],
                                                 char uniqueValue[20]) {
    
    struct processDataStruct data;
    strcpy(data.uniqueValue, uniqueValue);
    strcpy(data.column, column);

    rcv(uniqueValue);

    struct uniqueRecordStruct uniqueRecord = readFileByUniqueValue(data.filename, data.column, data.uniqueValue);

    return uniqueRecord;
}