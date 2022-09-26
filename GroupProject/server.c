#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Christian's includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

// Jacob's Code   ----------------------------------------------------------------


int saveFile(DataTable *ptable, DataColumn *pcolumn)
{


int i, j, k; //initializing variables for later for loops

int n = gets(DataTable.numCols); //initialize n for the number of columns in the table
int z = gets(DataTable.numRows); //initialize z for the number of rows in the table

FILE *output; //creating file

output = fopen ("output.txt", "w"); //open the file for writing



for(k = 0; k < n, k++) //loop for adding the data headers to the file
{
	gets((ptable.colHeaders);
	printf("\t\t");



}

for(j = 0; j < z; j++) //loop for adding the rest of the data to the file. This loop is for the rows
{
	for(i = 0; i < n; i++) //this loop is for the columns
	{
		gets(*pcolumn.columns);
		printf("\t\t"); //formatting

	
	}

	printf("\n"); //formatting

}


/*
fwrite (&input, sizeof(struct DataTable), 1, outfile);
this is commented out because I think im gonna stop trying to use gets and change to fwrite to get things to work better.
}
*/


// Rebecca's Code ----------------------------------------------------------------

// Group G
// Rebecca Wasserott
// rebecca.wasserott@okstate.edu
// 09/25
// The logic to enable the server to display the contents of the struct array on the client
// Not finished. This is for the progress report.

//#include "process.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main() {

//    //struct definition
//    typedef struct{
//        //dynamic array of strings
//        char **data;
//    }
//    DataColumn;

//    typedef struct{
//        //dynamic array of strings for the column headers
//        char** colHeaders;
//        //dynamic array of datacolumns
//        DataColumn *cloumns;

//        int numRows;
//        int numCols;
//    }
//    DataTable;

//    char *filename = "bookInfo1.txt";
//    char *column = "Stock";
//    char *uniqueValue = "Out of stock";

//    int row = 0;
//    int col = 0;

//    char*** recordsArray = getRecordsByUniqueValue(filename, column, uniqueValue, &row, &col);

//    for (int i = 1; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            printf("%d. %s\n", (j + 1), recordsArray[0][j]);

//            printf("%s\n\n", recordsArray[i][j]);
//        }
//        printf("---------------------------------\n");
//    }

// }

// Michael's Code ---------------------------------------------------------------
#define LINE_LENGTH 256
#define INITIAL_NUM_ROWS 200
#define ROW_INCRIMENT 50

typedef struct
{
    char **data;
} DataColumn;

typedef struct
{
    DataColumn *columns;
    int numRows;
    int numCols;
} DataTable;

bool readFile(char fileName[], DataTable *ptable)
{
    FILE *file;

    char errorMsg[] = "Error. File Null";

    printf("Read file: %s", &fileName[0]);
    // fopen_s(&file, fileName, "r");
    fopen(fileName, "r");
    if (&fileName == NULL)
    {
        printf("%s", errorMsg);
        return false;
    }

    char line[LINE_LENGTH];
    char *temp;
    int currentLine = 0, j = 0;
    const char comma[] = ",";

    fgets(line, LINE_LENGTH, file);
    char *bufferPt = line;
    ptable->numCols = 0;
    printf("\nBuffer pointer: %s", bufferPt);
    while (*bufferPt)
    {
        if (*bufferPt != ',' && *bufferPt != '\t')
        {
            ptable->numCols++, bufferPt++;
            // printf("\nnumCol: %d", ptable->numCols);
        }
        while (*bufferPt && *bufferPt != ',' && *bufferPt != '\t')
        {
            bufferPt++;
        }
        while (*bufferPt && (*bufferPt == ',' || *bufferPt == '\t'))
        {
            bufferPt++;
        }
    }

    printf("\nInt col = %d\n\n", ptable->numCols);

    ptable->numRows = INITIAL_NUM_ROWS;
    ptable->columns = calloc(ptable->numCols, sizeof(DataColumn));
    for (int i = 0; i < ptable->numCols; i++)
    {
        ptable->columns[i].data = calloc(INITIAL_NUM_ROWS, sizeof(char *));
    }

    while (fgets(line, LINE_LENGTH, file))
    {
        if (line[0] == '\0')
        {
            continue;
        }

        if (line[strlen(line) - 1] == '\n')
        { // Discard the newline character
            line[strlen(line) - 1] = '\0';
        }

        if (j == ptable->numRows)
        {
            ptable->numRows += ROW_INCRIMENT;
            for (int i = 0; i < ptable->numCols; i++)
            {
                ptable->columns[i].data =
                    realloc(ptable->columns[i].data, ptable->numRows * sizeof(char *));
                for (int r = j; r < ptable->numRows; r++)
                {
                    ptable->columns[i].data[r] = NULL;
                }
            }
        }

        temp = strtok(line, comma);
        for (int i = 0; i < ptable->numCols; i++)
        {
            ptable->columns[i].data[j] = malloc(strlen(temp) + 1);
            strcpy(ptable->columns[i].data[j], temp);

            // printf("%s\n", temp);
            printf("%s  ", ptable->columns[i].data[j]);

            temp = strtok(NULL, comma);

            if (i % ptable->numCols == 0)
            {
                printf("\n");
            }
        }

        j++;
    }

    printf("\nClosing\n");
    fclose(file);

    ptable->numRows = j;
    for (int i = 0; i < ptable->numCols; i++)
    {
        ptable->columns[i].data =
            realloc(ptable->columns[i].data, ptable->numRows * sizeof(char *));
        for (int r = j; r < ptable->numRows; r++)
        {
            ptable->columns[i].data[r] = NULL;
        }
    }

    return true;
}

// Christian's code ---------------------------------------------------------------

#define MAX_TEXT 512 // maximum length of the message that can be sent allowed
#define N_PROCESSES 2
struct my_msg
{
    long int msg_type;
    char *uniqueValue;
};

int processes[N_PROCESSES];

char ***readFileByUniqueValue(char *filename, char *column, char *uniqueValue, int *row_, int *col_)
{

    int nRows = 705;
    int nCols = 5;
    int nValues = 300;

    if (strcmp(filename, "amazonBestsellers.txt") == 0)
    {
        nRows = 555;
        nCols = 6;
    }

    char ***recordArray = (char ***)malloc(nRows * sizeof(char **));

    for (int i = 0; i < nRows; i++)
    {
        recordArray[i] = (char **)malloc(nCols * sizeof(char *));

        for (int j = 0; j < nCols; j++)
        {
            recordArray[i][j] = (char *)malloc(nValues * sizeof(char));
        }
    }

    FILE *inFile = fopen(filename, "r");
    if (!inFile)
    {
        perror("fopen for reading");
        exit(EXIT_FAILURE);
    }

    char *line = (char *)malloc(sizeof(char));
    int row = 0;
    int col = 0;

    int columnIndex = 0;

    while (fscanf(inFile, "%[^\n] ", line) != EOF)
    {

        char *pt = strtok(line, ",");

        col = 0;

        int i = 0;
        while (pt != NULL)
        {

            if (row == 0 && strcmp(pt, column) == 0)
            {
                columnIndex = i;
            }

            if (columnIndex != 0 && columnIndex == i && strcmp(pt, uniqueValue) != 0 && strcmp(pt, column) != 0)
            {
                row--;
                // printf("row = %d, col = %d\n", row, col);
                break;
            }

            if (columnIndex == 0 || columnIndex != i)
            {

                for (int k = 0; k < strlen(pt); k++)
                {
                    recordArray[row][col][k] = pt[k];
                }

                //                 printf("recordArray[%d][%d] = %s\n", row, col, recordArray[row][col]);
                //                 printf("recordArray[%d][%d] = %s\n", row, col, pt);

                col++;
            }

            pt = strtok(NULL, ",");
            i++;
        }

        row++;
    }

    //     for (int i = 0; i < row; i++) {
    //         for (int j = 0; j < nCols; j++) {
    //             printf("recordArray[%d][%d] = %s\n", i, j, recordArray[i][j]);
    //         }
    //         printf("\n");
    //     }

    //     fclose(inFile);

    *row_ = row;
    *col_ = nCols;
    return recordArray;
}

void createProcesses(int nProcess, char *filename, char *column, char uniques[][15])
{

    pid_t pid;

    pid_t parent = getpid();
    pid_t pip = parent;

    printf("Starting id is %d\n\n", getpid());

    for (int i = 0; i < nProcess; i++)
    {

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork failed\n");
            exit(1);
            return;
        }

        if (pid == 0 && parent == getppid())
        {
            processes[i] = getpid();

            printf("If block - My pid is %d, and parent pid is %d\n", getpid(), getppid());

            char *unique = uniques[i];

            char ***recordArray = readFileByUniqueValue(filename, column, unique, 0, 0);
            // sendData(getpid(), unique);
        }
    }

    if (parent != getpid())
    {
        // receiveData(getpid());
    }

    wait(NULL);
}

void sendData(int pid, char *uniqueValue)
{
    int running = 1;
    struct my_msg some_data;

    char buffer[50]; // array to store user input

    int msgid = msgget((key_t)14534, 0666 | IPC_CREAT);

    if (msgid == -1)
    { // -1 means the message queue is not created
        printf("Error in creating queue\n");
        exit(0);
    }

    some_data.msg_type = 1;

    some_data.uniqueValue = uniqueValue;

    // strcpy(some_data.some_text, "Hey ");

    // char temp[5];
    // sprintf(temp, "%d", pid);
    // strcat(some_data.some_text, temp);

    if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
    { // msgsnd returns -1 if the message is not sent
        printf("Msg not sent\n");
    }
}

void receiveData(int pid)
{
    int running = 1;
    struct my_msg some_data;
    long int msg_to_rec = 0;

    int msgid = msgget((key_t)14534, 0666 | IPC_CREAT);

    msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_rec, 0);
    printf("Data received: %s\n", some_data.uniqueValue);
    msgctl(msgid, IPC_RMID, 0);
}

struct uniqueRecordStruct
{
    int sizeOfVal;
    char *uniqueValue;
    char **field;
    char ***values;
};

extern struct uniqueRecordStruct *uniqueRecordArray;

int row = 0;
int col = 0;

// int main() {

//     char *filename = "bookInfo1.txt";
//     char *column = "Stock";
//     char *uniqueValue = "In stock";

//     char ***recordArray = readFileByUniqueValue(filename, column, uniqueValue, &row, &col);

//     int count = 0;
//     // char*** uniqueRecordArray_ = getByUniqueValue(recordArray, column, "Out of stock", row, col, &count);

//     // char uniqueValues[2][20] = {"In stock", "Out of stock"};
//     // createProcesses(2, uniqueValues, recordArray, column, row, col);
// }

// Cody's server ---------------------------------------------------------------

int main()
{
    // port used for connection between client and server
#define port 4567
    // structure that allows the storage of different values for the server address
    struct sockaddr_in serverAddress;
    // buffer that stores input from client to server and messages from server to client
    char buffer[512];
    // initializes the server socket and sets it up to be an ipv4 TCP socket with no protocols
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // checks to make sure the socket was created successfully
    if (serverSocket < 0)
    {
        printf("Error Creating Server Socket\n");
        return 0;
    }
    printf("Server Socket Created Successfully\n");

    // sets the server address to null values to ensure there are no unexpected values
    memset(&serverAddress, '\0', sizeof(serverAddress));
    // sets the server to an ipv4 socket
    serverAddress.sin_family = AF_INET;
    // ensures the server port value is stored correctly by using htons
    // which takes 16-bit host byte numbers and returns the 16-bit numbers in network byte order
    serverAddress.sin_port = htons(port);
    // sets the server address to 127.0.0.1 which is the ip for local host
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // tries to bind the server to the socket using the server address
    int serverBind = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    // checks to make sure the server was able to bind to the socket successfully
    if (serverBind < 0)
    {
        printf("Error Binding Server To Socket\n");
        return 0;
    }
    printf("Server Bound To Port %d\n", port);

    // makes sure there are no current connections to the server and if there are closes the program
    // also limits the amount of clients that can be connected
    if (listen(serverSocket, 3) == 0)
    {
        printf("Waiting For Connections\n");
    }
    else
    {
        return 0;
    }

    // new values to store information for each of the child servers that are interacting with each client
    int newSocket;
    struct sockaddr_in newServerAddress;
    socklen_t newServerAddressSize;
    pid_t childProcessID;

    while (1)
    {
        // return point to get rid of child processes once their client disconnects
    ReturnToHere:
        // creates a new server socket for future clients to connect to
        newSocket = accept(serverSocket, (struct sockaddr *)&newServerAddress, &newServerAddressSize);
        if (newSocket < 0)
        {
            return 0;
        }
        printf("Connection made on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));

        // creates each of the child processes
        if ((childProcessID = fork()) == 0)
        {
            // closes the main port that clients use to connect on the child processes
            close(serverSocket);
            while (1)
            {
                // receives messages from the client
                recv(newSocket, buffer, 512, 0);
                // checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
                if (strcmp(buffer, "exit") == 0)
                {
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    //
                    goto ReturnToHere;
                }
                else
                {
                    // displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer);
                    // sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                    // resets the buffer to all null values
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
    return 0;
}