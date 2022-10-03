// Group G
// Cody Ray
// 09/25/2022
// Code to initialize the server and the child servers that it creates to handle each client that connects to the main server

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    //port used for connection between client and server
    #define port 4567
    //structure that allows the storage of different values for the server address
    struct sockaddr_in serverAddress;
    //buffer that stores input from client to server and messages from server to client
    char buffer[512];
    //initializes the server socket and sets it up to be an ipv4 TCP socket with no protocols
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    //checks to make sure the socket was created successfully
    if(serverSocket < 0){
        printf("Error Creating Server Socket\n");
        return 0;
    }
    printf("Server Socket Created Successfully\n");

    //sets the server to an ipv4 socket
    serverAddress.sin_family = AF_INET;
    //ensures the server port value is stored correctly by using htons
    //which takes 16-bit host byte numbers and returns the 16-bit numbers in network byte order
    serverAddress.sin_port = htons(port);
    //sets the server address to 127.0.0.1 which is the ip for local host
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    //tries to bind the server to the socket using the server address
    int serverBind = bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
    //checks to make sure the server was able to bind to the socket successfully
    if(serverBind < 0){
        printf("Error Binding Server To Socket\n");
        return 0;
    }
    printf("Server Bound To Port %d\n", port);

    //Starts listening for connections on the socket we created for the server
    listen(serverSocket, 3);
    printf("Waiting For Connections...\n");

    //new values to store information for each of the child servers that are going to handle the
    //interactions with each new client
    int newSocket;
    struct sockaddr_in newServerAddress;
    socklen_t newServerAddressSize;
    pid_t childProcessID;

    //infinite loop to handle the rest of the server client connections and communication
    while(1){
        //return point for the child processes once their client disconnects
        ReturnToHere:
        //creates a new server socket for future clients to connect to
        newSocket = accept(serverSocket, (struct sockaddr*)&newServerAddress, &newServerAddressSize);
        if(newSocket < 0){
            return 0;
        }
        printf("Connection made on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));

        //creates each of the child processes
        if((childProcessID = fork()) == 0){
            //closes the main port that clients use to connect in the child processes
            close(serverSocket);
            while(1){
                //receives messages from the client
                recv(newSocket, buffer, 512, 0);
                //checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
                if(strcmp(buffer, "exit") == 0){
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    //temporary return point for the child process to break the loop with the current connected client
                    //dont think this is the correct way to kill the child process
                    goto ReturnToHere;
                }else{
                    //displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer);
                    //sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                    //resets the buffer to all null values
                    bzero(buffer,sizeof(buffer));
                }
            }
        }
    }
    return 0;
}

void display(char column[20], char uniqueValue[20], int socket) {
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    for (int i = 1; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) { 
            // printf("%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);
            send(socket, uniqueRecord.recordArray[0][j], strlen(uniqueRecord.recordArray[0][j]), 0);

            
            // printf("%s\n\n", uniqueRecord.recordArray[i][j]);

            send(socket, uniqueRecord.recordArray[i][j], strlen(uniqueRecord.recordArray[i][j]), 0);

        }
        // printf("---------------------------------\n");
    }
}

void save(char column[20], char uniqueValue[20], int socket) {
    struct uniqueRecordStruct uniqueRecord = fetchRecords(column, uniqueValue);

    FILE *outFile = fopen(filename, "w");
    printf("snd: filename -> %s\n", filename);

    if (outFile == NULL || !outFile) {
      printf("There was an error opening the file for writing\n");
      perror("fopen for writing");
      exit(EXIT_FAILURE);
    }

    for (int i = 1; i < uniqueRecord.rowSize; i++) {
        for (int j = 0; j < uniqueRecord.colSize; j++) { 
            fprintf(outFile, "%s\n", uniqueRecord.recordArray[0][j]);

            fprintf(outFile, "%s\n", uniqueRecord.recordArray[i][j]);
        }
        printf("---------------------------------\n");
    }
    fclose(outFile);
}