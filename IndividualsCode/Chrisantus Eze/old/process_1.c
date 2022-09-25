#include "record_struct.h"
#include "process.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



#include <stdio.h> 	// Baisc standard I/O like printf
#include <unistd.h> 	// defines standard symbolic constants and types
#include <sys/types.h> 	// basic derived types like pid_t
#include <sys/stat.h> 	//required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h> 	//required for open (i.e. file control options)

struct uniqueRecordStruct {
    int sizeOfVal;
    char* uniqueValue;
    char** field;
    char*** values;
};

extern struct uniqueRecordStruct* uniqueRecordArray;

struct recordStruct *recordArray = NULL;
int row = 0;
int col = 0;

// int main() {

//     char *filename = "bookInfo.txt";
//     char *column = "Stock";

//     recordArray = readFile(filename, column, &row, &col);

//     int count = 0;
//     // char*** uniqueRecordArray_ = getByUniqueValue(recordArray, column, "Out of stock", row, col, &count);


//     // char uniqueValues[2][20] = {"In stock", "Out of stock"};
//     // createProcesses(2, uniqueValues, recordArray, column, row, col);
// }

struct recordStruct *readFile(char* filename, char* column, int* row_, int* col_) {

    int nRows = 1000;
    int nCols = 12;
    int nValues = 300;

    // Each record has a uniqueValue
    struct recordStruct *recordArray = (struct recordStruct*)malloc(nRows * sizeof(struct recordStruct));
    for (int i = 0; i < nRows; i++) {
        recordArray[i].uniqueValue = (char*)malloc(nValues * sizeof(char));

        recordArray[i].values = (char**)malloc(nCols * sizeof(char*));
        for (int j = 0; j < nCols; j++) {
            recordArray[i].values[j] = (char*)malloc(nValues * sizeof(char));
        }
    }

    FILE *inFile = fopen(filename, "r");
    if (!inFile) {
        perror("fopen for reading");
        exit(EXIT_FAILURE);
    }

    char *line = (char*)malloc(sizeof(char));
    int row = 0;
    int col = 0;

    int columnIndex = 0;

    while (fscanf(inFile, "%[^\n] ", line) != EOF) {

        char *pt = strtok(line, ",");
        
        col = 0;
        while (pt != NULL) {

            if (row == 0) {
                for (int k = 0; k < strlen(pt); k++) {
                    recordArray[row].values[col][k] = pt[k];
                }
                if (strcmp(pt, column) == 0) {
                    columnIndex = col;
                }

            } else {
                for (int k = 0; k < strlen(pt); k++) {
                    recordArray[row].values[col][k] = pt[k];
                }

                if (col == columnIndex) {
                    for (int k = 0; k < strlen(pt); k++) {
                        recordArray[row].uniqueValue[k] = pt[k];
                    }
                }

                // printf("recordArray[%d].values[%d] = %s\n", row, col, recordArray[row].values[col]);
            }
            
            pt = strtok (NULL, ",");
            col++;
        }

        row++;
    }

    fclose(inFile);

    *row_ = row;
    *col_ = col;
    return recordArray;

}


char*** getByUniqueValue(struct recordStruct* recordArray, char* column, char* uniqueValue, int row, int col, int *count) {

    char*** uniqueRecordArray_ = (char***)malloc(row * sizeof(char**));
    for (int i = 0; i < row; i++) {
        uniqueRecordArray_[i] = (char**)malloc(col * sizeof(char*));

        for (int j = 0; j < col; j++) {
            uniqueRecordArray_[i][j] = (char*)malloc(300 * sizeof(char));
        }
    }

    int colCount = 0;
    int colIndex = 0;
    for (int j = 0; j < col; j++) {
        if (strcmp(column, recordArray[0].values[j]) != 0) {
            uniqueRecordArray_[0][colCount] = recordArray[0].values[j];
            colCount++;
        } else {
            colIndex = j;
        }
    }

    int rowCount = 1; 
    for (int i = 1; i < row; i++) {
        colCount = 0;
        if (strcmp(recordArray[i].uniqueValue, uniqueValue) == 0) {
            for (int j = 0; j < col; j++) {
                if (colIndex != j) {
                    uniqueRecordArray_[rowCount][colCount] = recordArray[i].values[j];
                    // printf("uniqueRecordArray[%d][%d] = %s\n", rowCount, colCount, uniqueRecordArray_[rowCount][colCount]); 
                    colCount++;
                } 
            }
            rowCount++;
        }
    }

    // for (int i = 0; i < rowCount; i++) {
    //     for (int j = 0; j < col-1; j++) {
    //         printf("new - uniqueRecordArray[%d][%d] = %s\n", i, j, uniqueRecordArray_[i][j]); 
    //     }
    // }

    *count = rowCount;
    return uniqueRecordArray_;
}

/*
struct processInfoStruct *createProcesses(int nProcess, char uniqueValues[][20], struct recordStruct* recordArray, char* column, int row, int col) {

    int pipefds1[2], pipefds2[2];
    int returnstatus1, returnstatus2;
    // char pipe1writemessage[20] = "Hi";
    char pipe2writemessage[20] = "Hello";
    char readmessage[20];

    struct processInfoStruct *processInfoArray = (struct processInfoStruct*)malloc(nProcess * sizeof(struct processInfoStruct));
    
    returnstatus1 = pipe(pipefds1);
    if (returnstatus1 == -1) {
        printf("Unable to create pipe 1 \n");
        return processInfoArray;
    }

    returnstatus2 = pipe(pipefds2);
    if (returnstatus2 == -1) {
        printf("Unable to create pipe 2 \n");
        return processInfoArray;
    }

    pid_t pid;
    
    pid_t parent = getpid();
    pid_t pip = parent;
    
    printf("Starting id is %d\n\n", getpid());

    for(int i = 0; i < nProcess; i++) {  // loop will run n times (n=5)
        
        // pipe(fd);

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n"); 
            exit(1);
            return processInfoArray;
        }

        if(pid == 0 && parent == getppid()) { 
            processInfoArray[i].id = getpid();
            // processInfoArray[i].uniqueValue = uniqueValues[i];

            int count = 0;
            char*** uniqueRecordArray_ = getByUniqueValue(recordArray, column, uniqueValues[i], row, col, &count);

            // sendData(getpid());

            // close(fd[0]); 	// Close the output side of the child process 

            // int result = getpid();
            // write(fd[1], &result, sizeof(result));
		    // close(fd[1]);

            printf("My pid is %d, and parent pid is %d\n\n", getpid(), getppid());
        }

    }
    wait(NULL);

    return processInfoArray;
}

// void getData(struct processInfoStruct *processInfoArray, char* uniqueValue) {
    
//     for (int i = 0; i < 5; i++) {
//         if (processInfoArray[i].id == getpid()) {
//             receiveData(getpid());
//             // Return its data
//         }
//     }
// }

void getData(struct processInfoStruct *processInfoArray, char* uniqueValue) {
    
    for (int i = 0; i < 5; i++) {
        if (processInfoArray[i].id == getpid()) {
            close(fd[1]);	// Close the input side of the pipe

            int pid_ = 0;
            wait(NULL);

            read(fd[0], &pid_, (sizeof(getpid())));
            // close(fd[0]);

            printf("\nThe final result is: %d\n", pid_); 	// Print the read message 

        }
    }
}


void receiveData(int childPid) {
    int fd; // file descriptor

	long long int result = 0;	// computing results for addition
	long long int finalResult;	// computing final results for addition

	long long int i;			// loop counter
	
	// Perform addition
	for (i = MAX/2+1; i <= MAX; i++){
		result = result + i;
	}
	
	char myPipe[] = "./NamedPipeServer"; 	// FIFO file path 
	mkfifo(myPipe, childPid);	// Creating the named file(FIFO) 
	

	if ((fd = open(myPipe, O_RDONLY)) < 0) 	// Open FIFO for Read only
		printf("2: Error opening..\n");
		
	if (read(fd, &finalResult, sizeof(long long int)) < 0)	// Read from FIFO 
		printf("2: Error reading..\n");

	// Display the results
	printf("\nThe final result is: %lld\n", (finalResult)); 	// Print the read message 

	close(fd); 			// Close the pipe
}

void sendData(int childPid) {

    int fd; // file descriptor
	
	long long int result = 0;
	long long int i;

	// for (i = 1; i <= MAX/2; i++){
	// 	result = result + i;
	// }
	
	printf("Computation done...\n");
	
	char myPipe[] = "./NamedPipeServer"; 	// FIFO file path 
	mkfifo(myPipe, childPid);	// Creating the named file(FIFO)
	
	
   	if ((fd = open(myPipe,O_WRONLY | O_CREAT)) < 0) 	// Open FIFO for write only
		printf("1: Error opening..\n");
		
	if (write(fd, &childPid, sizeof(long long int)) < 0) // Write on the FIFO
		printf("1: Error writing..\n");
		
    close(fd); 			// Close the pipe

    
}

*/
char*** getRecordsByUniqueValue(char* filename, char* column, char* uniqueValue, int *row_, int *col_) {
    int count = 0;
    if (recordArray == NULL) {
        recordArray = readFile(filename, column, &row, &col);
    }

    char*** recordsByUniqueValue = getByUniqueValue(recordArray, column, uniqueValue, row, col, &count);
    *row_ = count;
    *col_ = col-1;

    return recordsByUniqueValue;
}