#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct msgQue{
    long msgType;
    char msgText[256];
}message;

int main(){
    int numOfProcesses;
    int mqIDs[numOfProcesses];
    int currentProcess = 0;
    key_t processKey;
    int msgID;

    processKey = fork();

    while(currentProcess < numOfProcesses){
        if (getpid() == 0) {
            msgID = msgget(processKey, 0666 | IPC_CREAT);
            break;
        } else {
            msgID = msgget(processKey, 0666 | IPC_CREAT);
            mqIDs[currentProcess] = processKey;
            currentProcess++;
            processKey = fork();
        }
    }

    // child process waiting for messages and storing them
    if(getpid() == 0){
        msgrcv(msgID, &message, sizeof(message),1,0);

        //call read file by unique value

    }

    //parent process sending messages
    if(getpid() > 0){
        //maybe need message.msgType = 1;

        //get data from structure stored from main menu
        // message = data from main menu

        for(int i = 0; i < sizeof(mqIDs); i++){
            msgsnd(mqIDs[i], &message, sizeof(message), 0);
        }
    }

    msgctl(msgID, IPC_RMID, NULL);
    return 0;
}
