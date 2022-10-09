// #include "read_file.c"
#include "unique_record_struct.h"
#include "pipe.h"
#include <stdio.h>  // Baisc standard I/O like printf
#include <unistd.h>  // defines standard symbolic constants and types
#include <sys/types.h>  // basic derived types like pid_t
#include <sys/stat.h>  //required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h>  //required for open (i.e. file control options)
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define MAX 900000 // 9 billion times

#define MAX_RECORD_STRING 80000

char myPipe[] = "./NamedPipe";  // FIFO file path 

// int main() {
    
// //     int pid = fork();
// //     if (pid < 0) {
// //         printf("There was an error creating the process\n");
// //         return 0;
// //     }

// //     if (pid == 0) {
// //         sendDataViaPipe(myPipe);
// //     } else {
// //         receiveDataViaPipe(myPipe);
// //     }
  
//     struct uniqueRecordStruct recordArray = readFile("bookInfo.txt");

//     struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, "Stock", "In stock");

//     char* recordString = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
// //     printf("The array:%sC\n", recordString);
  
//     struct uniqueRecordStruct newRecord = unwrap(recordString);

//     return 0;
// }

void sendDataViaPipe(char* myPipe, struct uniqueRecordStruct uniqueRecord) {
    int fd; // file descriptor
    long long int result = 0;
    long long int i;

    for (i = 1; i <= MAX/2; i++){
        result = result + i;
    }

    printf("Computation done...\n");
    mkfifo(myPipe, 0666); // Creating the named file(FIFO)

    if ((fd = open(myPipe, O_WRONLY | O_CREAT)) < 0) {  // Open FIFO for write only
        printf("1: Error opening..");
    }

//     for (int i = 0; i < 10; i++) {
//       char* recordString = concatenate(uniqueRecord.recordArray[i], uniqueRecord.colSize);
// //       printf("%s\n", recordString);
//       if (write(fd, &recordString, sizeof((char*)900000)) < 0) {// Write on the FIFO
//           printf("1: Error writing..");
//       }
//     }
  
    char* myArray = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
  
//     strcat(recordString, "\n");
//     char* temp = strtok(recordString, "\n");
  
    char temp[MAX_RECORD_STRING];

  
    strcpy(temp, myArray);
//     printf("%s", temp);
    if (write(fd, &temp, sizeof(char[900000])) < 0) {// Write on the FIFO
        printf("1: Error writing..");
    }
    close(fd);  // Close the pipe
}

struct uniqueRecordStruct receiveDataViaPipe(char* myPipe) {
    int fd; // file descriptor
    long long int result = 0; // computing results for addition
    long long int finalResult; // computing final results for addition
    long long int i; // loop counter

    // Perform addition
    for (i = MAX/2+1; i <= MAX; i++){
        result = result + i;
    }

    mkfifo(myPipe, 0666); // Creating the named file(FIFO) 
    if ((fd = open(myPipe, O_RDONLY)) < 0) { // Open FIFO for Read only
        printf("2: Error opening..");
    }
  
    char recordString[MAX_RECORD_STRING];
  
    if (read(fd, &recordString, sizeof(char[900000])) < 0) { // Read from FIFO 
      printf("2: Error reading..");
    }
  
    struct uniqueRecordStruct record = unwrap(recordString);
    printf("The received message is: %d\n", record.rowSize);
    close(fd);  // Close the pipe
  
    return record;
}

// char* concatenate(char recordRow[7][300], int col) {  
//   char* recordString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));

//   for (int i = 0; i < col; i++) {
//     if (recordRow[i][0] == '"') {
//         for (int k = 0; k <= strlen(recordRow[i]); k++) {
//             if (recordRow[i][k] == '"') {
//                 recordRow[i][k] = ' ';
// //               printf("Here: %s\n", recordArray[i][j]);
//             }
//         }
//       }
//     strcat(recordString, recordRow[i]);
//     if (i < col - 1) {
//       strcat(recordString, "|");
//     }
//     recordString[ strcspn( recordString, "\n" ) ] = ' ';
//   }
// //   printf("The slave string is %s\n", recordString);
//   return recordString;
// }


char* concatenate(char recordArray[705][7][300], int row, int col) {  
  char* masterString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));

  char* slaveString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));
  
  for (int i = 0; i < row; i++) {
    strcpy(slaveString, "");
    for (int j = 0; j < col; j++) {
      
      // This replaces all occurrences of '"' with a whitespace
      if (recordArray[i][j][0] == '"') {
        for (int k = 0; k <= strlen(recordArray[i][j]); k++) {
            if (recordArray[i][j][k] == '"') {
                recordArray[i][j][k] = ' ';
//               printf("Here: %s\n", recordArray[i][j]);
            }
        }
      }
      
      strcat(slaveString, recordArray[i][j]);
      if (j < col - 1) {
        strcat(slaveString, "|");
      }
//       printf("The slave string is %s\n", slaveString);
    }
    strcat(masterString, slaveString);
    masterString[ strcspn( masterString, "\n" ) ] = '@';
  }
  return masterString;
}

struct uniqueRecordStruct unwrap(char* arrayString) {
  
    char rowArray[700][300];

    // Tokenize the character array by '@' and store the tokens in a 2D array
    char* row = strtok(arrayString, "@");
    int row_count = 0;
    while(row != NULL) {
//       printf("Length: %ld, Row: %s\n", strlen(row), row);

      strcpy(rowArray[row_count], row);
      row = strtok(NULL, "@");
      row_count++;
    }

    struct uniqueRecordStruct record;

    // Loop through the 2D array, tokenize each row by '|' and store the tokens in the struct's 3D array
    int col_count = 0;
    for (int i = 0; i < row_count; i++) {
      char* col = strtok(rowArray[i], "|");
      col_count = 0;
      
      while(col != NULL) {
//         printf("Length: %ld, Col: %s\n", strlen(col), col);

        strcpy(record.recordArray[i][col_count], col);
//         printf("record[%d][%d]: %s\n", i, col_count, record.recordArray[i][col_count]);
        
        col = strtok(NULL, "|");
        col_count++;
      }
    }
  
    record.colSize = col_count;
    record.rowSize = row_count;
  
    return record;
}



