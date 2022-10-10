//Group G
//Douglas Edmondson
//douglas.edmondson@okstate.edu
//09/25
//The logic to enable the server to display the contents of the struct array on the client
//


//DOES NOT COMPILE

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

                    // resets the buffer to all null values
                    bzero(buffer, sizeof(buffer));
                    recv(newSocket, buffer, 512, 0); // saves choice made from user

                    if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses to display records
                    {
                        while (1)
                        {
                            send(newSocket, "Which record to display: \n", strlen("Which record to display: \n"), 0);
                            send(newSocket, "1. Book Category  \n", strlen("1.Book Category  \n"), 0);
                            send(newSocket, "2. Star Rating\n", strlen("2. Star Rating\n"), 0);
                            send(newSocket, "3. Stock\n", strlen("3. Stock \n"), 0);

                            // resets the buffer to all null values
                            bzero(buffer, sizeof(buffer));
                            //  receives messages from the client
                            recv(newSocket, buffer, 512, 0);

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                            {
                                if(strcmp(buffer, "1")==0)
                                {
                                    strcpy(column, "Book Category");
                                    // display()
                                    break;
                                }

                                else if(strcmp(buffer, "2")==0)
                                {
                                    strcpy(column, "Star Rating");
                                    // display()
                                    break;
                                }

                                else if(strcmp(buffer, "3")==0)
                                {
                                    strcpy(column, "Stock");
                                    // display()
                                    break;
                                }
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
                            send(newSocket, "1. Book Category  \n", strlen("1.Book Category  \n"), 0);
                            send(newSocket, "2. Star Rating\n", strlen("2. Star Rating\n"), 0);
                            send(newSocket, "3. Stock\n", strlen("3. Stock \n"), 0);

                            // resets the buffer to all null values
                            bzero(buffer, sizeof(buffer));
                            // saves choice made from user
                            recv(newSocket, buffer, 512, 0);

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                            {
                                if(strcmp(buffer, "1")==0)
                                {
                                    strcpy(column, "Book Category");
                                    // save()
                                    break;
                                }

                                else if(strcmp(buffer, "2")==0)
                                {
                                    strcpy(column, "Star Rating");
                                    // save()
                                    break;
                                }

                                else if(strcmp(buffer, "3")==0)
                                {
                                    strcpy(column, "Stock");
                                    // save()
                                    break;
                                }
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
                            send(newSocket, "1. User Rating \n", strlen("1. User Rating \n"), 0);
                            send(newSocket, "2. Year \n", strlen("2. Year \n"), 0);
                            send(newSocket, "3. Genre \n", strlen("3. Genre \n"), 0);

                            // resets the buffer to all null values
                            bzero(buffer, sizeof(buffer));
                            // scanf("%i", &midChoice);
                            recv(newSocket, buffer, 512, 0); // saves choice made from user

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                            {
                                if(strcmp(buffer, "1")==0)
                                {
                                    strcpy(column, "User Rating");
                                    // display()
                                    break;
                                }

                                else if(strcmp(buffer, "2")==0)
                                {
                                    strcpy(column, "Year");
                                    // display()
                                    break;
                                }

                                else if(strcmp(buffer, "3")==0)
                                {
                                    strcpy(column, "Genre");
                                    // display()
                                    break;
                                }
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
                            send(newSocket, "Which record to save: \n", strlen("Which record to display: \n"), 0);
                            send(newSocket, "1. Name \n", strlen("1. Name \n"), 0);
                            send(newSocket, "2. Author \n", strlen("2. Author \n"), 0);
                            send(newSocket, "3. User Rating \n", strlen("3. User Rating \n"), 0);

                            // resets the buffer to all null values
                            bzero(buffer, sizeof(buffer));
                            // scanf("%i", &midChoice);
                            recv(newSocket, buffer, 512, 0); // saves choice made from user

                            if (strncmp("1", buffer, strlen("1")) == 0) // if user chooses option
                            {
                                if(strcmp(buffer, "1")==0)
                                {
                                    strcpy(column, "User Rating");
                                    // save()
                                    break;
                                }

                                else if(strcmp(buffer, "2")==0)
                                {
                                    strcpy(column, "Year");
                                    // save()
                                    break;
                                }

                                else if(strcmp(buffer, "3")==0)
                                {
                                    strcpy(column, "Genre");
                                    // save()
                                    break;
                                }
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
