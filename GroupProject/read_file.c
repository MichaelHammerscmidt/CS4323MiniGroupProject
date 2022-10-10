// Group Name: Group G
// Name: Chrisantus Eze
// Email: chrisantus.eze@okstate.edu
// Date: 09/25
// The logic to enable the server communicate with processes
// Haven't implemented the inter-process communication process yet. This is the
// next item to be done

#include "read_file.h"
#include "unique_record_struct.h"
#include "process_data_struct.h"

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>

#define MAXCHAR 1000

extern struct uniqueRecordStruct uniqueRecordArray;

// int main() {

//     char *filename = "bookInfo.txt";
//     char *column = "Stock";
//     char *uniqueValue = "In stock";

//     struct uniqueRecordStruct recordArray = readFile(filename);
// //     struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, column, uniqueValue);
  
    
// //     for (int i = 1; i < recordArray.rowSize; i++) {
// //       for (int j = 0; j < recordArray.colSize; j++) { 
// //           printf("%s ", recordArray.recordArray[i][j]);
// //       }
// //       printf("---------------------------------\n");
// //     }
  
//     // struct processDataStruct data;
    
//     // sendDataToParent(data);
  
//     return 0;

// }

struct uniqueRecordStruct readFile(char* filename) {
  int nRows = 710;
  int nCols = 6;
  int nValues = 300;

  if (strcmp(filename, "amazonBestsellers.txt") == 0) {
    nRows = 555;
    nCols = 6;
  }
  
  struct uniqueRecordStruct record;

  FILE *fp;
  char row[MAXCHAR];
  char *token;

  fp = fopen(filename,"r");


  int row_count = 0;
  int col_count = 0;
  while (feof(fp) != true)
  {
      fgets(row, MAXCHAR, fp);
//       printf("Row %d: %s", row_count, row);
    
      //This replaces any occurrence of ',' in between a double quotation with a ';'
      if (row[0] == '"') {
          for (int i = 1; i < strlen(row); i++) {
              if (row[i] == ',') {
                  row[i] = ';';
              }
              if (row[i] == '"') {
                  break;
              }
          }
      }

      token = strtok(row, ",");

      col_count = 0;
      while(token != NULL)
      {
//           printf("Token: %s\n", token);
          strcpy(record.recordArray[row_count][col_count], token);
//           printf("record[%d][%d]: %s\n", row_count, col_count, record.recordArray[row_count][col_count]);
        
          token = strtok(NULL, ",");
          col_count++;
      }

    row_count++;
  }
  
  record.colSize = col_count;
  record.rowSize = row_count;
  
//   for (int i = 1; i < record.rowSize; i++) {
//     for (int j = 0; j < record.colSize; j++) { 
//         printf("%s ", record.recordArray[i][j]);
//     }
//     printf("---------------------------------\n");
//   }

  return record;
}

char** getUniqueValues(struct uniqueRecordStruct records, char* column, int* returned_size) {
  int BUF_SIZE = 11;
  char **uniqueArray = (char **)malloc(BUF_SIZE * sizeof(char *));

  for (int i = 0; i < BUF_SIZE; i++) {
    uniqueArray[i] = (char *)malloc(100 * sizeof(char));
  }
  
  int colIndex = 0;
  for (int i = 0; i < records.colSize; i++) {
    if (strcmp(records.recordArray[0][i], column) == 0) {
      colIndex = i;
//       printf("colIndex: %d\n", colIndex);
      break;
    }
  }
  
  int countOfUnique = 0;
  for (int i = 1; i < records.rowSize; i++) {
    for (int j = 0; j < records.colSize; j++) {
      
      // Checks if the column is not the column we're interested in
      if (j != colIndex) {
        continue;
      }
      
      // Checks if the unique value has been added to the array already
      int added = 0;
      for (int k = 0; k < BUF_SIZE; k++) {
        if (strcmp(uniqueArray[k], records.recordArray[i][j]) == 0) {
          added = 1;
          break;
        }
      }

      if (added == 0) {
        uniqueArray[countOfUnique] = records.recordArray[i][j];
//         printf("uniqueArray[%d] = %s\n", countOfUnique, uniqueArray[countOfUnique]);
        countOfUnique++;
        break;
      }
    }
  }
  
  *returned_size = countOfUnique;
  return uniqueArray;
}

struct uniqueRecordStruct getRecordsByUniqueValue(struct uniqueRecordStruct records, char* column, char* uniqueValue) {
  int colIndex = 0;
  for (int i = 0; i < records.colSize; i++) {
    if (strcmp(records.recordArray[0][i], column) == 0) {
      colIndex = i;
//       printf("colIndex: %d\n", colIndex);
      break;
    }
  }
  
  struct uniqueRecordStruct uniqueRecordArray;
  
  int row = 0;
  int col = 0;
  for (int i = 0; i < records.rowSize; i++) {
    if (i != 0 && strcmp(records.recordArray[i][colIndex], uniqueValue) != 0) {
//       printf("Row %d: %s\n", i, records.recordArray[i][4]);
      continue;
    }
    
    col = 0;
    for (int j = 0; j < records.colSize; j++) {
      if (j == colIndex) {
        continue;
      }
      
      strcpy(uniqueRecordArray.recordArray[row][col], records.recordArray[i][j]);
      
//       printf("recordArray[%d][%d]: %s\n", row, col, uniqueRecordArray.recordArray[row][col]);
      col++;
    }
    row++;
  }
  strcpy(uniqueRecordArray.uniqueValue, uniqueValue);
  uniqueRecordArray.rowSize = row;
  uniqueRecordArray.colSize = col;
  
  return uniqueRecordArray;
}
