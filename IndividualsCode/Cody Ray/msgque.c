#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

struct msgQue{
    long msgType;
    char msgText[1028];
}message;

int main(){
    int numOfProcesses = 3;
    int currentProcess = 0;
    int childID;
    int msgID;
    int msgCheck;

    //while(currentProcess < numOfProcesses){
        childID = fork();
        msgID = msgget(childID, 0666 | IPC_CREAT);

        printf("CURRENT PROCESS: %d\n",currentProcess);

        if(childID < 0){
            printf("Error forking");
        }else if(childID == 0){
            printf("Entering child process PID: %d\n", childID);
            msgCheck = msgrcv(msgID, &message, sizeof(message),1,IPC_NOWAIT);
            sleep(20);
            if(msgCheck == -1){
                printf("Error receiving message\n");
            }
            printf("%s\n",message.msgText);
            msgctl(msgID, IPC_RMID, NULL);
        }else{
            printf("Entering parent process PID: %d\n", childID);
            strcpy(message.msgText,"hello");
            sleep(5);
            msgCheck = msgsnd(msgID, &message, sizeof(message), 0);
            if(msgCheck == -1){
                printf("Error sending message\n");
            }
        }
        //currentProcess++;
    //}

    msgctl(msgID, IPC_RMID, NULL);
    return 0;
}
