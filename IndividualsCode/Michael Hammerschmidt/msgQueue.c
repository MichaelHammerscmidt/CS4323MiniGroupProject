
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

struct msgQue{
    long int msgType;
    char msgText[50];
};

int main(){
    int numOfProcesses = 3;
    int currentProcess = 0;
    int msgID;
    int msgCheck;
    int running = 1;
    struct msgQue message;
    
    strcpy(message.msgText,"Sending failed:(");
    
    int t;

    while(currentProcess < numOfProcesses && running == 1){
        msgID = msgget((key_t)t, 0666 | IPC_CREAT);
        if(msgID == -1){
            printf("Error making message queue\n");
        }
        
        t = fork();
        printf("Fork value: %d\n",t);
        
            if(t < 0){
                printf("Error forking\n");
            }else if(t == 0){
                printf("Entering child process PID: %d\n", t);
                msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
                if(msgCheck == -1){
                    printf("Error receiving message\n");
                }
                printf("Recieved message: %s\n",message.msgText);
                if(strncmp(message.msgText, "Sending success!", 16) == 0){
                    printf("Success!\n");
                    running = 0;
                }
            }else{
                printf("Entering parent process PID: %d\n", t);
                message.msgType = 1;
                strcpy(message.msgText,"Sending success!");
                //printf("Size of msgText: %ld\n", sizeof(message.msgText));
                msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
                //printf("msgType = %ld\n", message.msgType);
                if(msgCheck == -1){
                    printf("Error sending message\n");
                }
                printf("\nMessage sent\n");
            }
        currentProcess++;
        printf("Process %d finished\n", t);
    }
    
    if(t == 0){
        int var = msgctl(msgID, IPC_RMID, NULL);
        printf("\nDestoryed %d\n", var);
    }
    return 0;
}




