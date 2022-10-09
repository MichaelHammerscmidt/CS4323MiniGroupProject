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
        
        //creates a new server socket for future clients to connect to and sends an error message if something is wrong
        newSocket = accept(serverSocket, (struct sockaddr*)&newServerAddress, &newServerAddressSize);
        if(newSocket < 0){
            printf("Error Creating Child Socket\n");
            return 0;
        }
        printf("Connection made on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));

        //creates each of the child processes
        if((childProcessID = fork()) == 0){
            //closes the main port that clients use to connect in the child processes
            close(serverSocket);

            //values that we use for the message que
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
                //checks to make sure the client has not exited and if it has it disconnects the child process and closes the socket
                if(strcmp(buffer, "exit") == 0){
                    close(newSocket);
                    printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
                    return 0;
                    
                }else if(strlen(buffer) > 0){
                    //displays what the client has sent to the server
                    printf("Client sent: %s\n", buffer);
                    //sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                    //resets the buffer to all null values
                    bzero(buffer,sizeof(buffer));
                }

                //loop to create the number of child processes we need to 
                while(currentProcess < numOfProcesses && running == 1){
                    //creates a message queue
                    msgID = msgget((key_t)t, 0666 | IPC_CREAT);
                    //makes sure there where no problems in the message queue creation
                    if(msgID == -1){
                        printf("Error making message queue\n");
                    }
        
                    //creates a child process and prints the PID
                    t = fork();
                    printf("Fork value: %d\n",t);
        
                    //makes sure there where no errors while forking.
                    if(t < 0){
                        printf("Error forking\n");
                        return 0;
                    }else if(t == 0){ //child process
                        printf("Entering child process PID: %d\n", t);
                        //checks the message queue for a message
                        msgCheck = msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR);
                        //makes sure there where no errors recieving the message
                        if(msgCheck == -1){
                            printf("Error receiving message\n");
                        }
                        //prints out the recieved message
                        printf("Recieved message: %s\n",message.msgText);
                        //determines if the message is the correct message
                        if(strncmp(message.msgText, "Sending success!", 16) == 0){
                            printf("Success!\n");
                            //breaks the child process out of the loop after it has recieved its message so it doesnt fork and create children as well
                            running = 0;
                        }
                    }else{ //parent process
                        printf("Entering parent process PID: %d\n", t);
                        message.msgType = 1;
                        //copies the message into the value we are passing in the queue
                        strcpy(message.msgText,"Sending success!");
                        //sends the current message to the message queue
                        msgCheck = msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0);
                        //makes sure there where no errors sending the message
                        if(msgCheck == -1){
                            printf("Error sending message\n");
                        }
                        printf("\nMessage sent\n");
                    }
                    //iterates through the loop to create a new process and print when processes are finished
                    currentProcess++;
                    printf("Process %d finished\n", t);

                    //if the process is a child process deletes the message queue it was using 
                    if(t == 0 && running == 0){
                        int var = msgctl(msgID, IPC_RMID, NULL);
                        printf("Pipe Destoryed %d\n", var);
                    }
                }
 
            }
        }
    }
    return 0;
}