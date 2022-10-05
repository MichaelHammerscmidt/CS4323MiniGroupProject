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

// Michael's Code ---------------------------------------------------------------
// Group G
// Michael Hammerschmidt
// mihamme@okstate.edu
// 09/25
// The function to read in all the contents from the filename passed in as the first argument and
// save them to the struct passed in as the second argument
// Finished.
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

bool readFile(char filesName[], DataTable *saveTable)
{
    FILE *fp;

    printf("Read file: %s", &filesName[0]);
    // fopen_s(&fp, filesName, "r");
    // if (fp == NULL)
    fopen(filesName, "r");

    if (&filesName == NULL)
    {
        printf("%s", "Error: The file is Null");
        return false;
    }

    char bufferIn[LINE_LENGTH];
    char *tokenizedBuffer;
    int rowsSoFar = 0;
    const char delim[] = ",";

    fgets(bufferIn, LINE_LENGTH, fp);
    char *bufferPt = bufferIn;
    saveTable->numCols = 0;
    printf("\nBuffer pointer: %s", bufferPt);
    while (*bufferPt)
    {
        if (*bufferPt != ',' && *bufferPt != '\t')
        {
            saveTable->numCols++, bufferPt++;
            // printf("\nnumCol: %d", saveTable->numCols);
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

    printf("\nNumber of col in file = %d\n\n", saveTable->numCols);

    saveTable->numRows = INITIAL_NUM_ROWS;
    saveTable->columns = calloc(saveTable->numCols, sizeof(DataColumn));
    for (int i = 0; i < saveTable->numCols; i++)
    {
        saveTable->columns[i].data = calloc(INITIAL_NUM_ROWS, sizeof(char *));
    }

    while (fgets(bufferIn, LINE_LENGTH, fp))
    {
        if (bufferIn[0] == '\0')
        {
            continue;
        }

        if (bufferIn[strlen(bufferIn) - 1] == '\n')
        { // Get rid of the newline character in the buffer
            bufferIn[strlen(bufferIn) - 1] = '\0';
        }

        if (rowsSoFar == saveTable->numRows)
        {
            saveTable->numRows = saveTable->numRows + ROW_INCRIMENT;
            for (int i = 0; i < saveTable->numCols; i++)
            {
                saveTable->columns[i].data = realloc(saveTable->columns[i].data, saveTable->numRows * sizeof(char *));
                for (int r = rowsSoFar; r < saveTable->numRows; r++)
                {
                    saveTable->columns[i].data[r] = NULL;
                }
            }
        }

        tokenizedBuffer = strtok(bufferIn, delim);
        for (int i = 0; i < saveTable->numCols; i++)
        {
            saveTable->columns[i].data[rowsSoFar] = malloc(strlen(tokenizedBuffer) + 1);
            strcpy(saveTable->columns[i].data[rowsSoFar], tokenizedBuffer);

            // For testing:
            // printf("%s\n", tokenizedBuffer);
            printf("%s  ", saveTable->columns[i].data[rowsSoFar]);

            tokenizedBuffer = strtok(NULL, delim);

            if (i % saveTable->numCols == 0)
            {
                printf("\n");
            }
        }

        rowsSoFar++;
    }

    printf("\nClosed file\n");
    fclose(fp);

    saveTable->numRows = rowsSoFar;
    for (int i = 0; i < saveTable->numCols; i++)
    {
        saveTable->columns[i].data =
            realloc(saveTable->columns[i].data, saveTable->numRows * sizeof(char *));
        for (int r = rowsSoFar; r < saveTable->numRows; r++)
        {
            saveTable->columns[i].data[r] = NULL;
        }
    }

    return true;
}

// Rebecca's Code ----------------------------------------------------------------

// Group G
// Rebecca Wasserott
// rebecca.wasserott@okstate.edu
// 09/25
// The logic to enable the server to display the contents of the struct array on the client
// Not finished. This is for the progress report.

//#include "process.c"

// This code is commented out because it is defined within a seperate main function

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

// Jacob's Code   ----------------------------------------------------------------

// int saveFile(DataTable *ptable, DataColumn *pcolumn)
// {

// int i, j, k; //initializing variables for later for loops

// int n = gets(DataTable.numCols); //initialize n for the number of columns in the table
// int z = gets(DataTable.numRows); //initialize z for the number of rows in the table

// FILE *output; //creating file

// output = fopen ("output.txt", "w"); //open the file for writing

// for(k = 0; k < n, k++) //loop for adding the data headers to the file
// {
// 	gets((ptable.colHeaders);
// 	printf("\t\t");

// }

// for(j = 0; j < z; j++) //loop for adding the rest of the data to the file. This loop is for the rows
// {
// 	for(i = 0; i < n; i++) //this loop is for the columns
// 	{
// 		gets(*pcolumn.columns);
// 		printf("\t\t"); //formatting

// 	}

// 	printf("\n"); //formatting

// }

/*
fwrite (&input, sizeof(struct DataTable), 1, outfile);
this is commented out because I think im gonna stop trying to use gets and change to fwrite to get things to work better.
}
*/

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

// This code is commented out because it is defined within a seperate main function

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

// Group G
// Cody Ray
// 09/25/2022
// Code to initialize the server and the child servers that it creates to handle each client that connects to the main server

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

    // Starts listening for connections on the socket we created for the server
    listen(serverSocket, 3);
    printf("Waiting For Connections...\n");

    // new values to store information for each of the child servers that are going to handle the
    // interactions with each new client
    int newSocket;
    struct sockaddr_in newServerAddress;
    socklen_t newServerAddressSize;
    pid_t childProcessID;

    // infinite loop to handle the rest of the server client connections and communication
    while (1)
    {
    // return point for the child processes once their client disconnects
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
            // closes the main port that clients use to connect in the child processes
            close(serverSocket);
            while (1)
            {
                // Create struct to save options.txt file into
                // DataTable optionsTable;
                // readFile("options.txt", &optionsTable);

                // Sends Main menu to client
                send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
                send(newSocket, "1. bookInfo.txt  \n", strlen("1. bookInfo.txt  \n"), 0);
                send(newSocket, "2. amazonBestsellers.txt  \n", strlen("2. amazonBestsellers.txt  \n"), 0);
                send(newSocket, "or type exit  \n", strlen("or type exit  \n"), 0);

                // receives messages from the client
                recv(newSocket, buffer, 512, 0);
                // checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
                if (strncmp("exit", buffer, strlen("exit")) == 0)
                {
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    // temporary return point for the child process to break the loop with the current connected client
                    // dont think this is the correct way to kill the child process
                    goto ReturnToHere;
                }
                else
                {
                    // displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer); // sends the message back to the client

                    // Declare variables to be determined by the user.
                    char filename[36];
                    char column[36];
                    char uniqueValue[128];
                    int numberOfUniques;

                    send(newSocket, buffer, strlen(buffer), 0);
                    if (strncmp("1", buffer, strlen("1")) == 0)
                    {
                        send(newSocket, "You selected bookInfo.txt \n", strlen("You selected bookInfo.txt \n"), 0);
                        strcpy(filename, "bookInfo.txt"); // Assign the correct filename

                        while (strncmp("4", buffer, strlen("4")) != 0) // while exit isn't selected, program runs
                        {
                            send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
                            send(newSocket, "1.Display Records  \n", strlen("1.Display Records  \n"), 0);
                            send(newSocket, "2.Save Records \n", strlen("2.Save Records \n"), 0);
                            send(newSocket, "3.Display Summary \n", strlen("3.Display Summary \n"), 0);
                            send(newSocket, "4. Exit \n", strlen("4. Exit \n"), 0);

                            // scanf("%i", &choice);
                            recv(newSocket, buffer, 512, 0); // saves choice made from user

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses to display records
                            {
                                while (1)
                                {
                                    send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
                                    send(newSocket, "1.Title  \n", strlen("1.Title  \n"), 0);
                                    send(newSocket, "2. Book Category\n", strlen("2. Book Category\n"), 0);
                                    send(newSocket, "3. Star Rating \n", strlen("3. Star Rating \n"), 0);
                                    send(newSocket, "4. Price \n", strlen("4. Price \n"), 0);
                                    send(newSocket, "5. Stock \n", strlen("5. Stock \n"), 0);
                                    send(newSocket, "6. Quantity \n", strlen("6. Quantity \n"), 0);

                                    // scanf("%i", &midChoice); // saves choice made from user
                                    //  receives messages from the client
                                    recv(newSocket, buffer, 512, 0);

                                    if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Title");
                                        // display()
                                        break;
                                    }

                                    else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Book category");
                                        // display()
                                        break;
                                    }

                                    else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Star rating"); // rating must remain lowercase, as it is in the file
                                        // display()
                                        break;
                                    }
                                    else if (strncmp("4", buffer, strlen("4")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Price");
                                        // display()
                                        break;
                                    }

                                    else if (strncmp("5", buffer, strlen("5")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Stock");
                                        // display()
                                        break;
                                    }

                                    else if (strncmp("6", buffer, strlen("6")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "6. Quantity");
                                        // display()
                                        break;
                                    }
                                    else
                                    {
                                        send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
                                    }
                                }
                            }
                            else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses to save records
                            {
                                while (1)
                                {
                                    send(newSocket, "Which record to save: \n", strlen("Which record to save: \n"), 0);
                                    send(newSocket, "1.Title  \n", strlen("1.Title  \n"), 0);
                                    send(newSocket, "2. Book Category\n", strlen("2. Book Category\n"), 0);
                                    send(newSocket, "3. Star rating \n", strlen("3. Star Rating \n"), 0);
                                    send(newSocket, "4. Price \n", strlen("4. Price \n"), 0);
                                    send(newSocket, "5. Stock \n", strlen("5. Stock \n"), 0);
                                    send(newSocket, "6. Quantity \n", strlen("6. Quantity \n"), 0);

                                    // saves choice made from user
                                    recv(newSocket, buffer, 512, 0);

                                    if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Title");
                                        // saveFile()
                                        break;
                                    }
                                    else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Book category");
                                        // saveFile()
                                        break;
                                    }
                                    else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Star rating");
                                        // saveFile()
                                        break;
                                    }
                                    else if (strncmp("4", buffer, strlen("4")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Price");
                                        // saveFile()
                                        break;
                                    }
                                    else if (strncmp("5", buffer, strlen("5")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Stock");
                                        // saveFile()
                                        break;
                                    }
                                    else if (strncmp("6", buffer, strlen("6")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Quantity");
                                        // saveFile()
                                        break;
                                    }
                                    else
                                    {
                                        send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
                                    }
                                }
                            }
                            else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses to display summary
                            {
                                // displaySummary function
                            }
                            else
                            {
                                send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
                            }
                        }
                    }
                    else if (strncmp("2", buffer, strlen("2")) == 0)
                    {

                        send(newSocket, "You selected amazonBestsellers.txt  \n", strlen("You selected amazonBestsellers.txt  \n"), 0);
                        strcpy(filename, "amazonBestsellers.txt"); // Assign the filename to filename variable

                        while (strncmp("4", buffer, strlen("4")) != 0) // while exist isn't selected, program runs
                        {
                            send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
                            send(newSocket, "1.Display Records  \n", strlen("1.Display Records  \n"), 0);
                            send(newSocket, "2.Save Records \n", strlen("2.Save Records \n"), 0);
                            send(newSocket, "3.Display Summary \n", strlen("3.Display Summary \n"), 0);
                            send(newSocket, "4. Exit \n", strlen("4. Exit \n"), 0);

                            // scanf("%i", &choice);
                            recv(newSocket, buffer, 512, 0); // saves choice made from user

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses to display records
                            {
                                while (1)
                                {
                                    send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
                                    send(newSocket, "1. Name \n", strlen("1. Name \n"), 0);
                                    send(newSocket, "2. Author \n", strlen("2. Author \n"), 0);
                                    send(newSocket, "3. User rating \n", strlen("3. User Rating \n"), 0);
                                    send(newSocket, "4. Reviews \n", strlen("4. Reviews \n"), 0);
                                    send(newSocket, "5. Price \n", strlen("5. Price \n"), 0);
                                    send(newSocket, "6. Year \n", strlen("6. Year \n"), 0);
                                    send(newSocket, "7. Genre \n", strlen("7. Genre \n"), 0);

                                    // scanf("%i", &midChoice);
                                    recv(newSocket, buffer, 512, 0); // saves choice made from user

                                    if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Name");
                                        // display(name)
                                        break;
                                    }
                                    else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Author");
                                        // display(author)
                                        break;
                                    }
                                    else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "User rating");
                                        // display(Star rating)
                                        break;
                                    }
                                    else if (strncmp("4", buffer, strlen("4")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Reviews");
                                        // display(reviews)
                                        break;
                                    }
                                    else if (strncmp("5", buffer, strlen("5")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Price");
                                        // display(price)
                                        break;
                                    }
                                    else if (strncmp("6", buffer, strlen("6")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Year");
                                        // display(year)
                                        break;
                                    }
                                    else if (strncmp("7", buffer, strlen("7")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Genre");
                                        // display(genre)
                                        break;
                                    }
                                    else
                                    {
                                        send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
                                    }
                                }
                            }
                            else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses to save records
                            {
                                while (1)
                                {
                                    send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
                                    send(newSocket, "1. Name \n", strlen("1. Name \n"), 0);
                                    send(newSocket, "2. Author \n", strlen("2. Author \n"), 0);
                                    send(newSocket, "3. User Rating \n", strlen("3. User Rating \n"), 0);
                                    send(newSocket, "4. Reviews \n", strlen("4. Reviews \n"), 0);
                                    send(newSocket, "5. Price \n", strlen("5. Price \n"), 0);
                                    send(newSocket, "6. Year\n", strlen("6. Year \n"), 0);
                                    send(newSocket, "7. Genre \n", strlen("7. Genre \n"), 0);

                                    // scanf("%i", &midChoice);
                                    recv(newSocket, buffer, 512, 0); // saves choice made from user

                                    if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Name");
                                        // saveFile(name)
                                        break;
                                    }
                                    else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Author");
                                        // saveFile(author)
                                        break;
                                    }
                                    else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "User rating");
                                        // saveFile(user rating)
                                        break;
                                    }
                                    else if (strncmp("4", buffer, strlen("4")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Reviews");
                                        // saveFile(reviews)
                                        break;
                                    }
                                    else if (strncmp("5", buffer, strlen("5")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Price");
                                        // saveFile(price)
                                        break;
                                    }
                                    else if (strncmp("6", buffer, strlen("6")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Year");
                                        // saveFile(year)
                                        break;
                                    }
                                    else if (strncmp("7", buffer, strlen("7")) == 0) // if user chooses option
                                    {
                                        strcpy(column, "Genre");
                                        // saveFile(genre)
                                        break;
                                    }
                                    else
                                    {
                                        send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
                                    }
                                }
                            }

                            else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses to display summary
                            {
                                // displaySummary(args)
                            }
                            else
                            {
                                send(newSocket, "Not valid input  \n", strlen("Not valid input  \n"), 0);
                            }
                        }
                    }
                    else
                    {
                        send(newSocket, "Not valid input  \n", strlen("Not valid input  \n"), 0);
                    }

                    // TODO: impliment Cody's message passing and call Christian's readFileByUniqueValue
                    // function passing in the variables: (char *filename, char *column, char *uniqueValue)

                    // resets the buffer to all null values
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
    return 0;
}

void display(char column[20], char uniqueValue[20], int socket)
{
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    for (int i = 1; i < uniqueRecord.rowSize; i++)
    {
        for (int j = 0; j < uniqueRecord.colSize; j++)
        {
            // printf("%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);
            send(socket, uniqueRecord.recordArray[0][j], strlen(uniqueRecord.recordArray[0][j]), 0);

            // printf("%s\n\n", uniqueRecord.recordArray[i][j]);

            send(socket, uniqueRecord.recordArray[i][j], strlen(uniqueRecord.recordArray[i][j]), 0);
        }
        // printf("---------------------------------\n");
    }
}

void save(char column[20], char uniqueValue[20], int socket)
{
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    FILE *outFile = fopen(filename, "w");
    printf("snd: filename -> %s\n", filename);

    if (outFile == NULL || !outFile)
    {
        printf("There was an error opening the file for writing\n");
        perror("fopen for writing");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < uniqueRecord.rowSize; i++)
    {
        for (int j = 0; j < uniqueRecord.colSize; j++)
        {
            fprintf(outFile, "%s\n", uniqueRecord.recordArray[0][j]);

            fprintf(outFile, "%s\n", uniqueRecord.recordArray[i][j]);
        }
        printf("---------------------------------\n");
    }
    fclose(outFile);
}
