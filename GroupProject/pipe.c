// #include "read_file.c"
#include "pipe.h"
#include "unique_record_struct.h"
#include <stdio.h>  // Baisc standard I/O like printf
#include <unistd.h>  // defines standard symbolic constants and types
#include <sys/types.h>  // basic derived types like pid_t
#include <sys/stat.h>  //required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h>  //required for open (i.e. file control options)
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define MAX 9000000 // 9 billion times

#define MAX_RECORD_STRING 80000

char myPipe[] = "./NamedPipe";  // FIFO file path
// int main() {
  
//     struct uniqueRecordStruct recordArray = readFile("bookInfo.txt");

//     struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, "Stock", "In stock");
  
//     int pid = fork();
//     if (pid < 0) {
//         printf("There was an error creating the process\n");
//         return 0;
//     }

//     if (pid == 0) {
//         sendDataViaPipe(myPipe, uniqueRecord);
//     } else {
//         receiveDataViaPipe(myPipe);
//     }

// //     char* recordString = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
// //     printf("The array:%sC\n", recordString);
  
// //     struct uniqueRecordStruct newRecord = unwrap(recordString);

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
    int resp = mkfifo(myPipe, 0666); // Creating the named file(FIFO)
    if (resp < 0) {
      perror("Could not create the named file.");
    }
    while (resp < 0) {
        if (remove(myPipe) == 0) {
            printf("Deleted successfully\n");

        } else {
            printf("Unable to delete the file\n");
        }
      
        resp = mkfifo(myPipe, 0666); // Creating the named file(FIFO)
        if (resp == 0) {
          printf("Named file created successfully.\n");
        }
    }

    printf("I am in the send function\n");
   
    if ((fd = open(myPipe, O_WRONLY)) < 0) {  // Open FIFO for write only
      perror("Could not open the named file.");
//       printf("1: Error opening..\n");
      
      remove(myPipe);
      close(fd);
      return;
    }
  
    printf("Opened the named file at the receiver.\n");
  
    printf("Now here!\n");

    char* myArray = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
  
//     strcat(recordString, "\n");
//     char* temp = strtok(recordString, "\n");
  
    char temp[MAX_RECORD_STRING];

  
    strcpy(temp, myArray);
//     printf("%s", temp);
    if (write(fd, &temp, sizeof(char[900000])) < 0) {// Write on the FIFO
        perror("Could not write to the named file.");
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

    int resp = mkfifo(myPipe, 0666); // Creating the named file(FIFO)
    if (resp < 0) {
      perror("Could not create the named file.");
    }
    while (resp < 0) {
        if (remove(myPipe) == 0) {
            printf("Deleted successfully\n");

        } else {
            printf("Unable to delete the file\n");
        }
      
        resp = mkfifo(myPipe, 0666); // Creating the named file(FIFO)
        if (resp == 0) {
          printf("Named file created successfully.\n");
        }
    }
  
    printf("I am in the receive function\n");
    if ((fd = open(myPipe, O_RDONLY)) < 0) { // Open FIFO for Read only
        perror("Could not open the named file.");
    }
  
    printf("Opened the named file at the receiver.\n");
    char recordString[MAX_RECORD_STRING];
  
    if (read(fd, &recordString, sizeof(char[900000])) < 0) { // Read from FIFO 
      perror("Could not read from the named file.");
    }
  
    struct uniqueRecordStruct record = unwrap(recordString);
    printf("The received message is: %d\n", record.rowSize);
    close(fd);  // Close the pipe
  
  return record;
}

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