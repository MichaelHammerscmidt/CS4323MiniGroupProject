#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

struct msgQue{
    long msgType;
    char msgText[256];
}message;

int main(){
    int numOfProcesses = 3;
    int currentProcess = 0;
    key_t processKey;
    pid_t parentID;
    pid_t childID;
    int msgID;

    strcpy(message.msgText,"hello");

    parentID = getpid();

    while(currentProcess < numOfProcesses){
        childID = fork();
        processKey = childID;
        msgID = msgget(processKey, 0666 | IPC_CREAT);

        if(parentID == getpid()){
            printf("current PID = %d\n", getpid());
            msgsnd(msgID, &message, sizeof(message), 0);
            wait(NULL);
        }
        if (childID != parentID) {
            printf("%d received\n", currentProcess);
            msgrcv(msgID, &message, sizeof(message),1,0);
            printf("%s",message);
        }
        currentProcess++;
    }

    msgctl(msgID, IPC_RMID, NULL);
    return 0;
}
