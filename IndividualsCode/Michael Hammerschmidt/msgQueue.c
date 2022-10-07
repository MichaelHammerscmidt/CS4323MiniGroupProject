
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
    //int numOfProcesses = 2;
    int currentProcess = 0;
    int msgID;
    int msgCheck;
    int running = 1;
    struct msgQue message;
    
    strcpy(message.msgText,"Sending failed:(");
    
    int t;

    //while(currentProcess < numOfProcesses && running == 1){
        t = fork();
        
        msgID = msgget((key_t)t, 0666 | IPC_CREAT);
        printf("CURRENT PROCESS: %d\n",currentProcess);
        
        while(1){ // While loop to continue sending and recieving until success
            
            if(msgID == -1){
                printf("Error making message queue\n");
                break;
            }
            else if(t < 0){
                printf("Error forking\n");
                break;
            }else if(t == 0){
                //sleep(0.1);
                printf("Entering child process PID: %d\n", t);
                msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,IPC_NOWAIT);
                if(msgCheck == -1){
                    printf("Error receiving message\n");
                }
                printf("Recieved message: %s\n",message.msgText);
                if(strncmp(message.msgText, "Sending success!", 16) == 0){
                    printf("Success!\n");
                    running = 0;
                    break;
                }
                msgctl(msgID, IPC_RMID, NULL);
                
            }else{
                sleep(0.5);
                printf("Entering parent process PID: %d\n", t);
                message.msgType = 1;
                strcpy(message.msgText,"Sending success!");
                msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
                if(msgCheck == -1){
                    printf("Error sending message\n");
                }else{
                    //running = 0;
                    break;
                }
            }
        } // end of success checking while loop
        //currentProcess++;
    //}

    msgctl(msgID, IPC_RMID, NULL);
    return 0;
}

