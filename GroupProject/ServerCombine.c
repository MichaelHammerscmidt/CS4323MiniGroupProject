// Group G
// Cody Ray
// 09/25/2022
// Code to initialize the server and the child servers that it creates to handle each client that connects to the main server
#include "pipe.c"
#include "read_file.c"
#include "read_file.h"
#include "unique_record_struct.h"
#include "process_data_struct.h"
#include "ServerCombine.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //required for open (i.e. file control options)
#define MAX_RECORD_STRING 80000


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
      
        // Main Menu goes here ----------------------------------------
      
        // Declare variables to be determined by the user.
        char filename[36];
        char column[36];
        char uniqueValue[128];
        int numberOfUniques = 0;
      
        
//         while (1)
//         {
//             // Create struct to save options.txt file into
//             // DataTable optionsTable;
//             // readFile("options.txt", &optionsTable);

//             // Sends Main menu to client
//             send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
//             send(newSocket, "1. bookInfo.txt  \n", strlen("1. bookInfo.txt  \n"), 0);
//             send(newSocket, "2. amazonBestsellers.txt  \n", strlen("2. amazonBestsellers.txt  \n"), 0);
//             send(newSocket, "or type exit  \n", strlen("or type exit  \n"), 0);

            
//             // receives messages from the client
//             recv(newSocket, buffer, 512, 0);
          
// //             int countOfMessages = 0;
// //             if(countOfMessages == 0){
// //               send(newSocket, "Welcome to the server", strlen("Welcome to the server"), 0);
// //             }
// //             else if(countOfMessages > 0){
// //               //sends the message back to the client
// //               send(newSocket, buffer, strlen(buffer), 0);
// //             }
// //             countOfMessages++;
// //             //resets the buffer to all null values
// //             bzero(buffer,sizeof(buffer));
          
//             // checks to make sure the client has not exited and if it has breaks out of the loop that controls child processes
//             if (strncmp("exit", buffer, strlen("exit")) == 0)
//             {
//                 printf("Disconnect on the IP %s and port %d\n", inet_ntoa(newServerAddress.sin_addr), ntohs(newServerAddress.sin_port));
//                 // temporary return point for the child process to break the loop with the current connected client
//                 // dont think this is the correct way to kill the child process
//             }
//             else
//             {
//                 // displays what the client has sent to the server
//                 printf("Client sent: %s\n", buffer); // sends the message back to the client

//                 send(newSocket, buffer, strlen(buffer), 0);
//                 if (strncmp("1", buffer, strlen("1")) == 0)
//                 {
//                     send(newSocket, "You selected bookInfo.txt \n", strlen("You selected bookInfo.txt \n"), 0);
//                     strcpy(filename, "bookInfo.txt"); // Assign the correct filename

//                     while (strncmp("4", buffer, strlen("4")) != 0) // while exit isn't selected, program runs
//                     {
//                         send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
//                         send(newSocket, "1. Display Records  \n", strlen("1.Display Records  \n"), 0);
//                         send(newSocket, "2. Save Records \n", strlen("2.Save Records \n"), 0);
//                         send(newSocket, "3. Display Summary \n", strlen("3.Display Summary \n"), 0);
//                         send(newSocket, "4. Exit \n", strlen("4. Exit \n"), 0);

//                         // resets the buffer to all null values
//                         bzero(buffer, sizeof(buffer));
//                         recv(newSocket, buffer, 512, 0); // saves choice made from user

//                         if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses to display records
//                         {
//                             while (1)
//                             {
//                                 send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
//                                 send(newSocket, "1. Book Category  \n", strlen("1.Book Category  \n"), 0);
//                                 send(newSocket, "2. Star Rating\n", strlen("2. Star Rating\n"), 0);
//                                 send(newSocket, "3. Stock\n", strlen("3. Stock \n"), 0);

//                                 // resets the buffer to all null values
//                                 bzero(buffer, sizeof(buffer));
//                                 //  receives messages from the client
//                                 recv(newSocket, buffer, 512, 0);

//                                 if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
//                                 {
//                                     if(strcmp(buffer, "1")==0)
//                                     {
//                                         strcpy(column, "Book Category");
//                                         // display()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "2")==0)
//                                     {
//                                         strcpy(column, "Star Rating");
//                                         // display()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "3")==0)
//                                     {
//                                         strcpy(column, "Stock");
//                                         // display()
//                                         break;
//                                     }
//                                 }

//                                 else
//                                 {
//                                     send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
//                                 }
//                             }
//                         }
//                         else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses to save records
//                         {
//                             while (1)
//                             {
//                                 send(newSocket, "Which record to save: \n", strlen("Which record to save: \n"), 0);
//                                 send(newSocket, "1. Book Category  \n", strlen("1.Book Category  \n"), 0);
//                                 send(newSocket, "2. Star Rating\n", strlen("2. Star Rating\n"), 0);
//                                 send(newSocket, "3. Stock\n", strlen("3. Stock \n"), 0);

//                                 // resets the buffer to all null values
//                                 bzero(buffer, sizeof(buffer));
//                                 // saves choice made from user
//                                 recv(newSocket, buffer, 512, 0);

//                                 if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
//                                 {
//                                     if(strcmp(buffer, "1")==0)
//                                     {
//                                         strcpy(column, "Book Category");
//                                         // save()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "2")==0)
//                                     {
//                                         strcpy(column, "Star Rating");
//                                         // save()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "3")==0)
//                                     {
//                                         strcpy(column, "Stock");
//                                         // save()
//                                         break;
//                                     }
//                                 }
//                                 else
//                                 {
//                                     send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
//                                 }
//                             }
//                         }
//                         else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses to display summary
//                         {
//                             // displaySummary function
//                         }
//                         else
//                         {
//                             send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
//                         }
//                     }
//                 }
//                 else if (strncmp("2", buffer, strlen("2")) == 0)
//                 {

//                     send(newSocket, "You selected amazonBestsellers.txt  \n", strlen("You selected amazonBestsellers.txt  \n"), 0);
//                     strcpy(filename, "amazonBestsellers.txt"); // Assign the filename to filename variable

//                     while (strncmp("4", buffer, strlen("4")) != 0) // while exist isn't selected, program runs
//                     {
//                         send(newSocket, "Enter a selection: \n", strlen("Enter a selection: \n"), 0);
//                         send(newSocket, "1. Display Records  \n", strlen("1.Display Records  \n"), 0);
//                         send(newSocket, "2. Save Records \n", strlen("2.Save Records \n"), 0);
//                         send(newSocket, "3. Display Summary \n", strlen("3.Display Summary \n"), 0);
//                         send(newSocket, "4. Exit \n", strlen("4. Exit \n"), 0);

//                         // scanf("%i", &choice);
//                         recv(newSocket, buffer, 512, 0); // saves choice made from user

//                         if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses to display records
//                         {
//                             while (1)
//                             {
//                                 send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
//                                 send(newSocket, "1. User Rating \n", strlen("1. User Rating \n"), 0);
//                                 send(newSocket, "2. Year \n", strlen("2. Year \n"), 0);
//                                 send(newSocket, "3. Genre \n", strlen("3. Genre \n"), 0);

//                                 // resets the buffer to all null values
//                                 bzero(buffer, sizeof(buffer));
//                                 // scanf("%i", &midChoice);
//                                 recv(newSocket, buffer, 512, 0); // saves choice made from user

//                                 if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
//                                 {
//                                     if(strcmp(buffer, "1")==0)
//                                     {
//                                         strcpy(column, "User rating");
//                                         // display()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "2")==0)
//                                     {
//                                         strcpy(column, "Year");
//                                         // display()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "3")==0)
//                                     {
//                                         strcpy(column, "Genre");
//                                         // display()
//                                         break;
//                                     }
//                                 }
//                                 else
//                                 {
//                                     send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
//                                 }
//                             }
//                         }
//                         else if (strncmp("2", buffer, strlen("2")) == 0) // if user chooses to save records
//                         {
//                             while (1)
//                             {
//                                 send(newSocket, "Which record to save: \n", strlen("Which record to display: \n"), 0);
//                                 send(newSocket, "1. Name \n", strlen("1. Name \n"), 0);
//                                 send(newSocket, "2. Author \n", strlen("2. Author \n"), 0);
//                                 send(newSocket, "3. User Rating \n", strlen("3. User Rating \n"), 0);

//                                 // resets the buffer to all null values
//                                 bzero(buffer, sizeof(buffer));
//                                 // scanf("%i", &midChoice);
//                                 recv(newSocket, buffer, 512, 0); // saves choice made from user

//                                 if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
//                                 {
//                                     if(strcmp(buffer, "1")==0)
//                                     {
//                                         strcpy(column, "User Rating");
//                                         // save()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "2")==0)
//                                     {
//                                         strcpy(column, "Year");
//                                         // save()
//                                         break;
//                                     }

//                                     else if(strcmp(buffer, "3")==0)
//                                     {
//                                         strcpy(column, "Genre");
//                                         // save()
//                                         break;
//                                     }
//                                 }
//                                 else
//                                 {
//                                     send(newSocket, "Invalid input \n", strlen("Invalid input \n"), 0);
//                                 }
//                             }
//                         }

//                         else if (strncmp("3", buffer, strlen("3")) == 0) // if user chooses to display summary
//                         {
//                             // displaySummary(args)
//                         }
//                         else
//                         {
//                             send(newSocket, "\nNot valid input  \n", strlen("\nNot valid input  \n"), 0);
//                         }
//                     }
//                 }
//                 else
//                 {
//                     send(newSocket, "\nNot valid input  \n", strlen("\nNot valid input  \n"), 0);
//                 }

//                 // TODO: impliment Cody's message passing and call Christian's readFileByUniqueValue
//                 // function passing in the variables: (char *filename, char *column, char *uniqueValue)

//                 // resets the buffer to all null values
//                 bzero(buffer, sizeof(buffer));
//             }
//         }
      
      
        strcpy(filename, "amazonBestsellers.txt");
        strcpy(column, "Genre");
        strcpy(uniqueValue, "Fiction");
        
        // Read the file and save into the struct.
        struct uniqueRecordStruct uniqueRecordArray = readFile(filename);
      
      
        //Define struct to save unique values into
        struct uniqueRecordStruct uniqueValueRecord;
      
        // TODO:  save the unique values ------------------------------
        char **uniqueArray = getUniqueValues(uniqueRecordArray, column, &numberOfUniques);

        //creates each of the child processes
        if((childProcessID = fork()) == 0){
            //closes the main port that clients use to connect in the child processes
            close(serverSocket);

            //values that we use for the message que
            int numOfProcesses = 2;
            int currentProcess = 0;
            int msgID;
            int running = 1;
            struct msgQue message;
            int t;
            strcpy(message.msgText,"Sending failed:(");
            int countOfMessages = 0;

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
                  if(countOfMessages == 0){
                    send(newSocket, "Welcome to the server", strlen("Welcome to the server"), 0);
                  }
                  else if(countOfMessages > 0){
                    //sends the message back to the client
                    send(newSocket, buffer, strlen(buffer), 0);
                  }
                  countOfMessages++;
                    //resets the buffer to all null values
                    bzero(buffer,sizeof(buffer));
                }

                //loop to create the number of child processes we need to
              
                int count = 0;
                while(currentProcess < numOfProcesses && running == 1){
                  
                    int fd[2];
                    pipe(fd);
                  
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
                      
                    } else if(t == 0){ //child process
                        printf("Entering child process PID: %d\n", t);
                      
                        //makes sure there where no errors recieving the message
                        if(msgrcv(msgID, (void *)&message, sizeof(message.msgText),1,MSG_NOERROR) == -1){
                            printf("Error receiving message\n");
                          
                        } else{
                            //prints out the recieved message
                            printf("Recieved message: %s\n", message.msgText);
//                         
                            printf("Success!\n");
                            //breaks the child process out of the loop after it has recieved its message so it doesnt fork and create children as well
                            // params include our struct instance called records, column, uniqueValue
                            
                            struct uniqueRecordStruct uniqueRecord = getRecordsByUniqueValue(uniqueRecordArray, column, message.msgText);
                            close(fd[0]);
                          
                            char* myArray = concatenate(uniqueRecord.recordArray, uniqueRecord.rowSize, uniqueRecord.colSize);
  
                            char temp[MAX_RECORD_STRING];

                            strcpy(temp, myArray);
//                             printf("%s", temp);

                            write(fd[1], &temp, sizeof(char[MAX_RECORD_STRING]));
                            close(fd[1]);
                          
                            //sendDataViaPipe(message.msgText, records);
                            
                            running = 0;
                        }
                    }else{ //parent process
                        printf("Entering parent process PID: %d\n", t);
                        message.msgType = 1;
                      
                        strcpy(message.msgText, uniqueArray[count]);
                      
                        //makes sure there where no errors sending the message
                        if(msgsnd(msgID, (void *)&message, sizeof(message.msgText), 0) == -1){
                            printf("Error sending message\n");
                          
                        } else{
                          printf("\nMessage sent\n");
                          
                          close(fd[1]);
        
                //           wait(NULL);
                          /* Send "text" through the output side of pipe */

                          char recordString[MAX_RECORD_STRING];

                          read(fd[0], &recordString, sizeof(char[MAX_RECORD_STRING]));

//                           printf("In parent-%s", recordString);

                          struct uniqueRecordStruct uniqueRecord = unwrap(recordString);
                          strcpy(uniqueRecord.uniqueValue, uniqueArray[count]);
//                           printf("The received message is: %s\n", uniqueRecord.recordArray[12][1]);
                          
                          if (strncmp(uniqueArray[count], uniqueValue, strlen(uniqueValue)) == 0) {
                            // Either display or save
                            displayOrSave(uniqueRecord, false);
                          }


                          close(fd[0]);

                          count++;
                        }
                    }
                    //iterates through the loop to create a new process and print when processes are finished
                    currentProcess++;
                    printf("Process %d finished\n", t);

                    //if the process is a child ands it has recieved its message deletes the message queue it was using
                    if(t == 0 && running == 0){
                        int var = msgctl(msgID, IPC_RMID, NULL);
                        printf("Message Queue Destoryed %d\n", var);
                    }
                }
            }
        }
    }
    return 0;
}

void displayOrSave(struct uniqueRecordStruct uniqueRecord, bool isDisplay) {
  if (isDisplay) {
    display(uniqueRecord);
  } else {
    save(uniqueRecord);
  }
}

void display(struct uniqueRecordStruct uniqueRecord) {
  for (int i = 1; i < uniqueRecord.rowSize; i++) {
      for (int j = 0; j < uniqueRecord.colSize; j++) { 
          printf("%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);

          printf("%s\n\n", uniqueRecord.recordArray[i][j]);
      }
      printf("---------------------------------\n");
  }
}

void save(struct uniqueRecordStruct uniqueRecord) {
  char filename[50];
  strcpy(filename, uniqueRecord.uniqueValue);
  strcat(filename, ".txt");
  
  FILE *outFile = fopen(filename, "w");
  if (outFile == NULL || !outFile) {
    perror("There was an error opening the file for writing");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 1; i < uniqueRecord.rowSize; i++) {
      for (int j = 0; j < uniqueRecord.colSize; j++) {
        fprintf(outFile, "%d. %s\n", (j + 1), uniqueRecord.recordArray[0][j]);
        fprintf(outFile, "%s\n\n", uniqueRecord.recordArray[i][j]);
      }
      fprintf(outFile, "---------------------------------\n");
  }
  fclose(outFile);
}

