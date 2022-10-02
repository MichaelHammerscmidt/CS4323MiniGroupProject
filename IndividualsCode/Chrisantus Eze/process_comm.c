#include "process_comm.h"
#include "process_file.c"
#include "process_data_struct.h"
#include "unique_record_struct.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<sys/ipc.h>
#include <sys/stat.h> 	//required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h> 	//required for open (i.e. file control options)

#define MAX_TEXT 512   //maximum length of the message that can be sent allowed

extern struct processDataStruct processData;

int main() {

    char *filename = "amazonBestsellers.txt";
    char *column = "User rating";

//     char unique[2][15] = {"In stock", "Out of stock"};
    char unique[11][15] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

    createProcesses(11, filename, column, unique);

    // receiveDataFromProcess("Out of stock.txt", column, "Out of stock");

    return 0;
}

void createProcesses(int nProcess, char *filename, char *column, char uniques[][15]) {
    
    pid_t parent = getpid();
    pid_t pip = parent;
    
    printf("Starting id is %d\n\n", getpid());

    int msgid;
    pid_t pid;

    msgid = msgget(IPC_PRIVATE, 0600);
    if(msgid == -1){
        perror("msgget");
        printf("Error in creating queue\n");
        exit(0);
    }

    for (int i = 0; i < nProcess; i++) {
        struct processDataStruct data;
        strcpy(data.uniqueValue, uniques[i]);
        strcpy(data.filename, filename);
        strcpy(data.column, column);
        strcpy(data.tdata, uniques[i]);
        sendViaQueue(msgid, data);
    }

    int count = 0;
    for(int i = 0; i < nProcess; i++) {
        
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n"); 
            exit(1);
            return;
        }

        if (pid > 0 && parent == getpid()) {
            // Send using message queue

            struct processDataStruct data;
            data.pid = pid;
            strcpy(data.uniqueValue, uniques[count]);
            strcpy(data.filename, filename);
            strcpy(data.column, column);
            strcpy(data.tdata, uniques[count]);
            count++;

            // printf("Unique value is: %s\n", data.uniqueValue);

            // send all the records corresponding to a unique value to a child process 
            sendViaQueue(msgid, data);

        } else if (pid == 0 && parent == getppid()) { 
            // printf("If block - My pid is %d, and parent pid is %d\n", getpid(), getppid());

            // receive records from paren
            struct processDataStruct data = receiveViaQueue(msgid, uniques[count]);
            
            // send data to parent          
            sendDataToParent(data);
        }

    }

    
    wait(NULL);
    msgctl(msgid, IPC_RMID, 0);
}

/**
 * @brief Allows the parent send records corresponding to a unique value to a child process
 * 
 * @param msgid 
 * @param data 
 */
void sendViaQueue(int msgid, struct processDataStruct data) {
    msgsnd_(msgid, data, MAX_TEXT, 0600);
}


/**
 * @brief Child process calls this to retrieve the sent records
 * 
 * @param msgid 
 * @param unique 
 * @return struct processDataStruct 
 */
struct processDataStruct receiveViaQueue(int msgid, char unique[20]) {
    struct processDataStruct processData;

    strcpy(processData.uniqueValue, unique);
    // printf("The unique value is: %s\n", processData.uniqueValue);
    struct processDataStruct data = msgrcv_(msgid, unique, MAX_TEXT, 0, 0);

    return data;
}

/**
 * @brief This allows the child process send records via pipe
 * 
 * @param data 
 */
void sendDatToParent(struct processDataStruct data) {
    // printf("About to send message with unique value: %s\n", data.uniqueValue);

    char pipePath[20] = "./pipe_";

    char *pt = strtok(data.uniqueValue, " ");

    strcat(pipePath, pt);
    printf("\nPipe path: %s\n\n", pipePath);

    // This hack forces the program to throw at runtime time, forcing the program to work :)
    char dump[10] = "dump";
    // strcat(dump, data.uniqueValue);

    while (mkfifo(pipePath, 0666) == -1) {
        if (remove(pipePath) == 0) {
            printf("Deleted successfully\n");

        } else {
            printf("Unable to delete the file\n");
        }
    }

    int fd; // file descriptor
    if ((fd = open(pipePath, O_WRONLY | O_CREAT)) < 0) {
		printf("1: Error opening..\n");
    } else {
        printf("1: No error opening..\n");
    }
		
	if (write(fd, &data, sizeof(data)) < 0) { 
		printf("1: Error writing..\n");
    } else {
        printf("1: No error writing..\n");
    }

    printf("Message sent! Unique value is: %s\n", data.uniqueValue);
		
    close(fd); 
}


/**
 * @brief Parent process calls this to retrieve records from a child process
 * 
 * @param uniqueValue 
 */
void receiveDatFromProcess(char uniqueValue[20]) {
    char pipePath[20] = "./pipe_";
    strcat(pipePath, uniqueValue);

	mkfifo(pipePath, 0666);
	
    int fd;
	if ((fd = open(pipePath, O_RDONLY)) < 0) {
		printf("2: Error opening..");
    }

    struct processDataStruct data;
	if (read(fd, &data, sizeof(data)) < 0) {
		printf("2: Error reading..");
    }

	printf("\nThe final result is: %s\n", data.uniqueValue);

	close(fd);
}

/**
 * @brief The server calls this to retrieve data corresponding to filename, column, and unique value
 * 
 * @param column 
 * @param uniqueValue 
 * @return struct uniqueRecordStruct 
 */
struct uniqueRecordStruct fetchRecords(char column[20], char uniqueValue[20]) {
    char filename[20];
    strcpy(filename, uniqueValue);
    strcat(filename, ".txt");

    return receiveDataFromProcess(filename, column, uniqueValue);
}
 