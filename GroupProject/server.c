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

#define LINE_LENGTH 256
#define INITIAL_NUM_ROWS 200
#define ROW_INCRIMENT 50

typedef struct
{
    char** data;
} DataColumn;

typedef struct
{
    DataColumn* columns;
    int numRows;
    int numCols;
} DataTable;


bool readFile(char fileName[], DataTable* ptable)
{
    FILE* file;

    char errorMsg[] = "Error. File Null";

    printf("Read file: %s", &fileName[0]);
    fopen_s(&file, fileName, "r");
    if (file == NULL)
    {
        printf("%s", errorMsg);
        return false;
    }

    char line[LINE_LENGTH];
    char* temp;
    int currentLine = 0, j = 0;
    const char comma[] = ",";

    fgets(line, LINE_LENGTH, file);
    char* bufferPt = line;
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
        ptable->columns[i].data = calloc(INITIAL_NUM_ROWS, sizeof(char*));
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
                    realloc(ptable->columns[i].data, ptable->numRows * sizeof(char*));
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
            realloc(ptable->columns[i].data, ptable->numRows * sizeof(char*));
        for (int r = j; r < ptable->numRows; r++)
        {
            ptable->columns[i].data[r] = NULL;
        }
    }

    return true;
}



int main() {
    //port used for connection between client and server
#define port 4567
//structure that allows the storage of different values for the server address
    struct sockaddr_in serverAddress;
    //buffer that stores input from client to server and messages from server to client
    char buffer[512];
    //initializes the server socket and sets it up to be an ipv4 TCP socket with no protocols
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    //checks to make sure the socket was created successfully
    if (serverSocket < 0) {
        printf("Error Creating Server Socket\n");
        return 0;
    }
    printf("Server Socket Created Successfully\n");

    //sets the server address to null values to ensure there are no unexpected values
    memset(&serverAddress, '\0', sizeof(serverAddress));
    //sets the server to an ipv4 socket
    serverAddress.sin_family = AF_INET;
    //ensures the server port value is stored correctly by using htons
    //which takes 16-bit host byte numbers and returns the 16-bit numbers in network byte order
    serverAddress.sin_port = htons(port);
    //sets the server address to 127.0.0.1 which is the ip for local host
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    //tries to bind the server to the socket using the server address
    int serverBind = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    //checks to make sure the server was able to bind to the socket successfully
    if (serverBind < 0) {
        printf("Error Binding Server To Socket\n");
        return 0;
    }
    printf("Server Bound To Port %d\n", port);

    //makes sure there are no current connections to the server and if there are closes the program 
    //also limits the amount of clients that can be connected 
    if (listen(serverSocket, 3) == 0) {
        printf("Waiting For Connections\n");
    }
    else {
        return 0;
    }

    //new values to store information for each of the child servers that are interacting with each client
    int newSocket;
    struct sockaddr_in newServerAddress;
    socklen_t newServerAddressSize;
    pid_t childProcessID;

    while (1) {
        //return point to get rid of child processes once their client disconnects
    ReturnToHere:
        //creates a new server socket for future clients to connect to
        newSocket = accept(serverSocket, (struct sockaddr*)&newServerAddress, &newServerAddressSize);
        if (newSocket < 0) {
            return 0;
        }
        printf("Connection made on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));

        //creates each of the child processes
        if ((childProcessID = fork()) == 0) {
            //closes the main port that clients use to connect on the child processes
            close(serverSocket);
            while (1) {
                //receives messages from the client
                recv(newSocket, buffer, 512, 0);
                //checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
                if (strcmp(buffer, "exit") == 0) {
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    //
                    goto ReturnToHere;
                }
                else {
                    //displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer);
                    //sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                    //resets the buffer to all null values
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
    return 0;

}