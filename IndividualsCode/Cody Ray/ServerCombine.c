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
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/wait.h>

struct msgQue{
    long int msgType;
    char msgText[50];
};

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

            int numOfProcesses = 3;
            int currentProcess = 0;
            int msgID;
            int msgCheck;
            int running = 1;
            struct msgQue message;
            int t;
            strcpy(message.msgText,"Sending failed:(");

            while(1){
                //receives messages from the client
                recv(newSocket, buffer, 512, 0);
                //checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
                if(strcmp(buffer, "exit") == 0){
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    //temporary return point for the child process to break the loop with the current connected client
                    //dont think this is the correct way to kill the child process
                    if(childProcessID == 0){
                        exit(0);
                    }
                    goto ReturnToHere;
                }else{
                    //displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer);
                    //sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                    //resets the buffer to all null values
                    bzero(buffer,sizeof(buffer));
                }

                while(currentProcess < numOfProcesses && running == 1){
                    msgID = msgget((key_t)t, 0666 | IPC_CREAT);
                    if(msgID == -1){
                        printf("Error making message queue\n");
                    }
        
                    t = fork();
                    printf("Fork value: %d\n",t);
        
                    if(t < 0){
                        printf("Error forking\n");
                    }else if(t == 0){
                        printf("Entering child process PID: %d\n", t);
                        msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
                        if(msgCheck == -1){
                            printf("Error receiving message\n");
                        }
                        printf("Recieved message: %s\n",message.msgText);
                        if(strncmp(message.msgText, "Sending success!", 16) == 0){
                            printf("Success!\n");
                            running = 0;
                        }
                    }else{
                        printf("Entering parent process PID: %d\n", t);
                        message.msgType = 1;
                        strcpy(message.msgText,"Sending success!");
                        //printf("Size of msgText: %ld\n", sizeof(message.msgText));
                        msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
                        //printf("msgType = %ld\n", message.msgType);
                        if(msgCheck == -1){
                            printf("Error sending message\n");
                        }
                        printf("\nMessage sent\n");
                    }
                    currentProcess++;
                    printf("Process %d finished\n", t);
                }
 
                if(t == 0){
                    int var = msgctl(msgID, IPC_RMID, NULL);
                    printf("\nDestoryed %d\n", var);
                }
            }
        }
    }
    return 0;
}