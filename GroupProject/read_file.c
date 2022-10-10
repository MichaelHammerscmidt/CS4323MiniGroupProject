// Group Name: Group G
// Name: Chrisantus Eze
// Email: chrisantus.eze@okstate.edu
// Date: 10/10
// The logic to that handles file retrieval and processing

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

//     char *filename = "amazonBestsellers.txt";
//     char *column = "Genre";
//     char *uniqueValue = "Fiction";

//     struct uniqueRecordStruct recordArray = readFile(filename);
  
//     int numberOfUniques = 0;
//     char** uniqueArray = getUniqueValues(recordArray, column, &numberOfUniques);
//     struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, column, uniqueValue);
    
    
// //     for (int i = 0; i < uniqueRecord.rowSize; i++) {
// //       for (int j = 0; j < uniqueRecord.colSize; j++) { 
// //           printf("%s ", uniqueRecord.recordArray[i][j]);
// //       }
// //       printf("---------------------------------\n");
// //     }
//     return 0;

// }


/**
 * @brief This opens, reads and saves the content of the file in a struct
 * 
 * @param filename name of the file
 * @return struct uniqueRecordStruct the returned struct containing all contents of the file.
 */
struct uniqueRecordStruct readFile(char* filename) {
  int nRows = 710;
  int nCols = 6;
  int nValues = 300;

  if (strcmp(filename, "amazonBestsellers.txt") == 0) {
    nRows = 552;
    nCols = 7;
  }
  
  struct uniqueRecordStruct record;

  FILE *fp;
  char row[MAXCHAR];
  char *token;

  fp = fopen(filename, "r");


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

      // Tokenizes the row
      token = strtok(row, ",");

      col_count = 0;
      while(token != NULL)
      {
//           printf("Token: %s\n", token);

          // Saves each column in the array
          strcpy(record.recordArray[row_count][col_count], token);
//           printf("record[%d][%d]: %s\n", row_count, col_count, record.recordArray[row_count][col_count]);
        
          token = strtok(NULL, ",");
          col_count++;
      }
    
      // This helped fix a bug in the amazonBestsellers file where for some reason an extra col was getting retrieved even 
      // when it is not contained in the file
      if (strcmp(filename, "amazonBestsellers.txt") == 0 && row_count >= 549) {
        break;
      }
      
//     printf("row_count %d\n", row_count);

      row_count++;
  }
  
  record.colSize = col_count;
  record.rowSize = row_count;
  
//   for (int i = 0; i < record.rowSize; i++) {
//     for (int j = 0; j < record.colSize; j++) { 
//         printf("%s ", record.recordArray[i][j]);
//     }
//     printf("---------------------------------\n");
//   }

  return record;
}

/**
 * @brief Get the Unique Values in an array
 * 
 * @param records the record retrieved in readFile function
 * @param column the column we're interested in
 * @param returned_size this allows us send back the size of the unique values array
 * @return char** this is the unique values array
 */
char** getUniqueValues(struct uniqueRecordStruct records, char* column, int* returned_size) {
  int BUF_SIZE = 11;
  char **uniqueArray = (char **)malloc(BUF_SIZE * sizeof(char *));

  for (int i = 0; i < BUF_SIZE; i++) {
    uniqueArray[i] = (char *)malloc(100 * sizeof(char));
  }
  
  // We get the index of the column we're interested in
  int colIndex = 0;
  for (int i = 0; i < records.colSize; i++) {
    if (strncmp(records.recordArray[0][i], column, strlen(column)) == 0) {
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

/**
 * @brief Get the Records By Unique Value
 * 
 * @param records the record retrieved in readFile function
 * @param column the column we're interested in
 * @param uniqueValue 
 * @return struct uniqueRecordStruct 
 */
struct uniqueRecordStruct getRecordsByUniqueValue(struct uniqueRecordStruct records, char* column, char* uniqueValue) {
  
  // We get the index of the column we're interested in
  int colIndex = 0;
  for (int i = 0; i < records.colSize; i++) {
    if (strncmp(records.recordArray[0][i], column, strlen(column)) == 0) {
      colIndex = i;
//       printf("colIndex: %d\n", colIndex);
      break;
    }
  }
  
  struct uniqueRecordStruct uniqueRecordArray;
  
  int row = 0;
  int col = 0;
  for (int i = 0; i < records.rowSize; i++) {

    // If the unique value does not match the one we're interested in, skip that row
    if (i != 0 && strncmp(records.recordArray[i][colIndex], uniqueValue, strlen(uniqueValue)) != 0) {
//       printf("Row %d: %s\n", i, records.recordArray[i][6]);
      continue;
    }
    
    col = 0;
    for (int j = 0; j < records.colSize; j++) {

      // Skip the column that our unique value is contained in
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