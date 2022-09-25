#include "ipc.h"
#include "process.c"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_TEXT 512   //maximum length of the message that can be sent allowed
#define N_PROCESSES 2
struct my_msg{
    long int msg_type;
    char *uniqueValue;
};

int processes[N_PROCESSES];

int main() {

    char *filename = "bookInfo1.txt";
    char *column = "Stock";

    char unique[2][15] = {"In stock", "Out of stock"};

    createProcesses(N_PROCESSES, filename, column, unique);

    for (int i = 0; i < N_PROCESSES; i++) {
        if (processes[i] == getpid()) {
            printf("Outside loopp - My pid is %d, and parent pid is %d\n", getpid(), getppid());
            receiveData(getpid());
            break;
        }
    }
}

void createProcesses(int nProcess, char *filename, char *column, char uniques[][15]) {

    pid_t pid;
    
    pid_t parent = getpid();
    pid_t pip = parent;
    
    printf("Starting id is %d\n\n", getpid());

    for(int i = 0; i < nProcess; i++) {
        
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n"); 
            exit(1);
            return;
        }

        if(pid == 0 && parent == getppid()) { 
            processes[i] = getpid();

            printf("If block - My pid is %d, and parent pid is %d\n", getpid(), getppid());

            char *unique = uniques[i];

            char ***recordArray = readFileByUniqueValue(filename, column, unique, 0, 0);
            // sendData(getpid(), unique);

            

        }

    }

    if (parent != getpid()) {
        // receiveData(getpid());
    }

    
    wait(NULL);
}

void sendData(int pid, char *uniqueValue) {
    int running = 1;
    struct my_msg some_data;

    char buffer[50];    // array to store user input

    int msgid = msgget((key_t)14534, 0666 | IPC_CREAT);

    if (msgid == -1) {  // -1 means the message queue is not created
        printf("Error in creating queue\n");
        exit(0);
    }

    some_data.msg_type = 1;

    some_data.uniqueValue = uniqueValue;

    // strcpy(some_data.some_text, "Hey ");

    // char temp[5];
    // sprintf(temp, "%d", pid);
    // strcat(some_data.some_text, temp);

    if(msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1) { // msgsnd returns -1 if the message is not sent
        printf("Msg not sent\n");
    }
}

void receiveData(int pid) {
    int running = 1;
    struct my_msg some_data;
    long int msg_to_rec = 0;

    int msgid = msgget((key_t)14534, 0666 | IPC_CREAT);

    msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_rec, 0);                 
    printf("Data received: %s\n", some_data.uniqueValue);
    msgctl(msgid, IPC_RMID, 0);
}