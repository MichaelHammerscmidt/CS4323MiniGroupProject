#include "process.h"
#include "read_file.c"
#include "pipe.c"
#include "unique_record_struct.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#define MAX_TEXT 50

extern struct uniqueRecordStruct uniqueRecordArray;

struct msgQue{
    long int msgType;
    char msgText[MAX_TEXT];
};

struct uniqueRecordStruct uniqueRecord;

struct uniqueRecordStruct uniqueRecords[5];

int main(){
  
    char* column = "Stock";
    manageIPC("bookInfo.txt", column);
  
    wait(NULL);
//     struct uniqueRecordStruct uniqueRecord_ = uniqueRecords[0];
//     for (int i = 0; i < uniqueRecord_.rowSize; i++) {
//       for (int j = 0; j < uniqueRecord_.colSize; j++) {
//         printf("Col: %s ", uniqueRecord_.recordArray[i][j]);
//       }
//       printf("\n");
//     }
    
    return 0;
}

void manageIPC(char* filename, char* column) {
  int numOfProcesses = 2;
  int currentProcess = 0;
  int msgID;
  int msgCheck;
  int running = 1;

  struct uniqueRecordStruct recordArray = readFile(filename);

  int numUniqueValues = 0;
  char** uniqueValueArray = getUniqueValues(recordArray, column, &numUniqueValues);

  int count = 0;

//   strcpy(message.msgText,"Sending failed:(");

  int t;

  while(currentProcess < numOfProcesses && running == 1){
      msgID = msgget((key_t)t, 0666 | IPC_CREAT);
      //printf("msgID =  %d\n", msgID);
      if(msgID == -1){
          printf("Error making message queue\n");
      }
    
      int fd[2];
      pipe(fd);

      t = fork();
      printf("Fork value: %d\n",t);

      if(t < 0){
          printf("Error forking\n");
      }else if(t == 0){

        char* uniqueValue = handleChildProcess(msgID, recordArray, column, t, &running);
        
        close(fd[0]); 

        uniqueRecord = getRecordsByUniqueValue(recordArray, column, uniqueValue);

        printf("The unique value is: %s\n", uniqueRecord.uniqueValue);
        
        char* myArray = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
  
        char temp[MAX_RECORD_STRING];


        strcpy(temp, myArray);
//         printf("%s", temp);
        
        write(fd[1], &temp, sizeof(char[MAX_RECORD_STRING]));
        close(fd[1]);

      }else{
          char* uniqueValue = uniqueValueArray[count];
          handleParentProcess(msgID, uniqueValue, t);
        
          close(fd[1]);
        
//           wait(NULL);
          /* Send "text" through the output side of pipe */
        
          char recordString[MAX_RECORD_STRING];
        
          read(fd[0], &recordString, sizeof(char[MAX_RECORD_STRING]));
        
//           printf("In parent-%s", recordString);
        
          struct uniqueRecordStruct record = unwrap(recordString);
          printf("The received message is: %s\n", record.recordArray[12][1]);
                
//           struct uniqueRecordStruct uniqueRecord_ = uniqueRecords[0];
//           for (int i = 0; i < record.rowSize; i++) {
//             for (int j = 0; j < record.colSize; j++) {
//               printf("Col: %s ", record.recordArray[i][j]);
//             }
//             printf("\n");
//           }
        
          uniqueRecords[count] = record;
        
          close(fd[0]);

          count++;
      }
      currentProcess++;
//       printf("Process %d finished\n", t);
  }


  if(t == 0){
      int var = msgctl(msgID, IPC_RMID, NULL);
      printf("\nDestroyed %d\n", var);

  } else {
//         struct uniqueRecordStruct record = receiveDataViaPipe("In stock");
//     strcpy(record.uniqueValue, uniqueValue);
  }
  wait(NULL);
}

char* handleChildProcess(int msgID, struct uniqueRecordStruct recordArray, char* column, int t, int* running) {
  *running = 1;
  struct msgQue message;
  
  printf("Entering child process PID: %d\n", t);
  int msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
  if(msgCheck == -1){
      perror("Error receiving message");
  }
  printf("Recieved message: %s\n",message.msgText);

//   uniqueRecord = getRecordsByUniqueValue(recordArray, column, message.msgText);

//   printf("The unique value is: %s\n", uniqueRecord.uniqueValue);

  printf("Success!\n");

//   sendDataViaPipe(uniqueRecord.uniqueValue, uniqueRecord);
  
  *running = 0;
  
  char* uniqueValue = message.msgText;

  return uniqueValue;
}

void handleParentProcess(int msgID, char* uniqueValue, int t) {
  struct msgQue message;
  
//   printf("Entering parent process PID: %d\n", t);
  message.msgType = 1;

  strcpy(message.msgText, uniqueValue);

  int msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
  if(msgCheck == -1){
      perror("Error sending message.");
  }
  printf("\nMessage sent\n");
//   struct uniqueRecordStruct record = receiveDataViaPipe(uniqueValue);

}