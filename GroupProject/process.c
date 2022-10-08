#include "process.h"
#include "read_file.c"
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

int main(){
  
    char* column = "Stock";
    manageIPC("bookInfo.txt", column);
    
    return 0;
}

void manageIPC(char* filename, char* column) {
  int numOfProcesses = 1;
  int currentProcess = 0;
  int msgID;
  int msgCheck;
  int running = 1;

  struct uniqueRecordStruct recordArray = readFile("bookInfo.txt");

  int numUniqueValues = 0;
  char** uniqueValueArray = getUniques(recordArray, column, &numUniqueValues);
  int count = 0;

//   strcpy(message.msgText,"Sending failed:(");

  int t;

  while(currentProcess < numOfProcesses && running == 1){
      msgID = msgget((key_t)t, 0666 | IPC_CREAT);
      //printf("msgID =  %d\n", msgID);
      if(msgID == -1){
          printf("Error making message queue\n");
      }

      t = fork();
      printf("Fork value: %d\n",t);

          if(t < 0){
              printf("Error forking\n");
          }else if(t == 0){
           
            running = handleChildProcess(msgID, recordArray, column, t);

          }else{
              char* uniqueValue = uniqueValueArray[count];
              handleParentProcess(msgID, uniqueValue, t);

              count++;
          }
      currentProcess++;
      printf("Process %d finished\n", t);
  }


  if(t == 0){
      int var = msgctl(msgID, IPC_RMID, NULL);
      printf("\nDestoryed %d\n", var);
  }
  wait(NULL);
}

int handleChildProcess(int msgID, struct uniqueRecordStruct recordArray, char* column, int t) {
  int running = 1;
  struct msgQue message;
  
  printf("Entering child process PID: %d\n", t);
  int msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
  if(msgCheck == -1){
      printf("Error receiving message\n");
  }
  printf("Recieved message: %s\n",message.msgText);

  struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(recordArray, column, message.msgText);

  printf("The unique value is: %s\n", uniqueRecord.uniqueValue);

  if(msgCheck != -1){
      printf("Success!\n");
      running = 0;
      //break;
  }
  
  return running;
}

void handleParentProcess(int msgID, char* uniqueValue, int t) {
  struct msgQue message;
  
  printf("Entering parent process PID: %d\n", t);
  message.msgType = 1;

  strcpy(message.msgText, uniqueValue);

  int msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
  if(msgCheck == -1){
      printf("Error sending message\n");
  }
  printf("\nMessage sent\n");
}
