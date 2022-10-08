// Group G
// Cody Ray
// 09/25/2022
// Code to initialize the client and attempt to connect to the server

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
    //initializes the client socket and sets it up to be an ipv4 TCP socket with no protocols
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    //checks to make sure the socket was created successfully
    if(clientSocket < 0){
        printf("Error Creating Client Socket\n");
        return 0;
    }
    printf("Client Socket Created Successfully\n");

    //sets the server to an ipv4 socket
    serverAddress.sin_family = AF_INET;
    //ensures the server port value is stored correctly by using htons
    //which takes 16-bit host byte numbers and returns the 16-bit numbers in network byte order
    serverAddress.sin_port = htons(port);
    //sets the server address to 127.0.0.1 which is the ip for local host
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    //tries to connect to the server using the clients socket and the server address
    int serverConnect = connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
    //checks to make sure the client was able to connect to the server successfully
    if(serverConnect < 0){
        printf("Unable To Connect To Server\n");
        return 0;
    }
    printf("Successfully Connected To Server\n");

    send(clientSocket,".", strlen("."),0);

    //infinite loop which handles the rest of the client server connections
    while(1){
        //scans user input and sends it to the server
        printf("Client: ");
        scanf("%s", &buffer[0]);
        send(clientSocket, buffer, strlen(buffer), 0);

        //checks if there are errors in input sent by server and displays input if there are no issues
        if(recv(clientSocket, buffer, 512, 0) < 0){
            printf("Error In Message Sent to Server");
        }else{
            printf("%s\n", buffer);
        }

        //ends the connection with the server if exit is inputted
        if(strcmp(buffer, "exit") == 0){
            return 0;
        }
    }
    return 0;
}