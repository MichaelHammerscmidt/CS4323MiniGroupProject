#include "read_file.c"
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

#define MAX 90//9000000000 // 9 billion times

#define MAX_RECORD_STRING 80000

char myPipe[] = "./NamedPipe";  // FIFO file path 
void sendDataViaPipe(char* myPipe, struct uniqueRecordStruct uniqueRecord);
void receiveDataViaPipe(char* myPipe);
// char* concatenate(char recordRow[7][300], int col);
char* concatenate(char recordArray[700][7][300], int row, int col);

int main() {
    
//     int pid = fork();
//     if (pid < 0) {
//         printf("There was an error creating the process\n");
//         return 0;
//     }

//     if (pid == 0) {
//         sendDataViaPipe(myPipe);
//     } else {
//         receiveDataViaPipe(myPipe);
//     }
  
    struct uniqueRecordStruct recordArray = readFile("bookInfo.txt");

    struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, "Stock", "In stock");

    char* recordString = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
    printf("The master string is %s\n", recordString);
    return 0;
}

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

//     for (int i = 0; i < 4; i++) {
//       char* recordString = concatenate(uniqueRecord.recordArray[], uniqueRecord.rowSize, uniqueRecord.colSize);
//       if (write(fd, &recordString, sizeof((char*)900000) < 0) {// Write on the FIFO
//           printf("1: Error writing..");
//       }
//     }
  
    char* recordString = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
//     char* temp = (char*)malloc(800000 * sizeof(char));
  
    char temp[800000];
  
    strcpy(temp, recordString);
//     printf("The master string is-%s\n", temp);
    if (write(fd, &temp, sizeof((char*)900000)) < 0) {// Write on the FIFO
        printf("1: Error writing..");
    }
    close(fd);  // Close the pipe
}

void receiveDataViaPipe(char* myPipe) {
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
  
//     char* recordString = (char*)malloc(800000 * sizeof(char));
  
       char recordString[800000];

//     for (int i = 0; i < 4; i++) {
//       if (read(fd, &recordString, sizeof(char*)) < 0) { // Read from FIFO 
//         printf("2: Error reading..");
//       }
//       printf("The received message is: %s\n", recordString);
//     }
  
    if (read(fd, &recordString, sizeof((char*)900000)) < 0) { // Read from FIFO 
      printf("2: Error reading..");
    }
    printf("The received message is: %s\n", recordString);
    close(fd);  // Close the pipe
}

// char* concatenate(char recordRow[7][300], int col) {  
//   char* recordString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));
  
// //   printf("row %d, col %d\n", row, col);

//   for (int i = 0; i < col; i++) {
//     strcat(recordString, recordRow[i]);
//     if (j < col - 1) {
//       strcat(slaveString, "|");
//     }
//   }
//   printf("The slave string is %s\n", recordString);
//   return recordString;
// }


char* concatenate(char recordArray[700][7][300], int row, int col) {  
  char* masterString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));

  char* slaveString = (char*)malloc(MAX_RECORD_STRING * sizeof(char));
  
  for (int i = 0; i < row; i++) {
    strcpy(slaveString, "");
    for (int j = 0; j < col; j++) {
      
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