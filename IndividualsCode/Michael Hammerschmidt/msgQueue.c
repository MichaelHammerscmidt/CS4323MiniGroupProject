// Online C compiler to run C program online
/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
// Online C compiler to run C program online
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
    //int currentProcess = 0;
    int msgID;
    int msgCheck;
    //int running = 1;
    struct msgQue message;
    
    strcpy(message.msgText,"Sending failed:(");
    
    int t;

    //while(currentProcess < numOfProcesses && running == 1){
        msgID = msgget((key_t)t, 0666 | IPC_CREAT);
        printf("msgID =  %d\n", msgID);
        if(msgID == -1){
            printf("Error making message queue\n");
        }
        
        t = fork();
        printf("Fork value: %d\n",t);
        
            if(t < 0){
                printf("Error forking\n");
            }else if(t == 0){
                //sleep(3);
                printf("Entering child process PID: %d\n", t);
                msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
                if(msgCheck == -1){
                    printf("Error receiving message\n");
                }
                printf("Recieved message: %s\n",message.msgText);
                if(strncmp(message.msgText, "Sending success!", 16) == 0){
                    printf("Success!\n");
                    //running = 0;
                    //break;
                }
                
            }else{
                //sleep(1);
                printf("Entering parent process PID: %d\n", t);
                message.msgType = 1;
                strcpy(message.msgText,"Sending success!");
                printf("Size of msgText: %ld\n", sizeof(message.msgText));
                printf("Size of msgText 2: %ld\n", sizeof(struct msgQue) - sizeof(long int));
                msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
                printf("msgType = %ld\n", message.msgType);
                if(msgCheck == -1){
                    printf("Error sending message\n");
                }else{
                    //running = 0;
                    printf("MsgCheck = %d\n", msgCheck);
                }
                printf("\nMessage sent\n");
            }
        //currentProcess++;
    //}
    
    
    if(t == 0){
        printf("Destroying msgqueueueueu\n");
        int var = msgctl(msgID, IPC_RMID, NULL);
        printf("\n%d\n", var);
    }
    printf("Process %d finished\n", t);
    return 0;
}




