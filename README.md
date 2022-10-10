# CS4323MiniGroupProject
Read text files and process them in parallel for multiple clients connected to the server.

## How to run server
gcc -o Server Server.c

## How to compile client
gcc -o Client Client.c

## How to run server
./Server

## How to run client
./Client

### Explanation
Launch the server and then any client you launch will connect to it.
Clients can be disconnected from the server by typing "exit" into the client terminal.
If there are any errors when starting the server like "Error Binding Server To Socket" or "Error Creating Child Socket".
You need to either type "reset" into the terminal and if that doesnt work you need to completely relauch your connecting on CSX on all of your terminals. 
You need to type anything as your first line of input to the server to get the client working
You will then be prompted in the main menu to enter your input
The main menu only takes input of integers or the terminating value "exit"
Once you see "Main Menu Input Completed" you again have to input anything into the client to move it along.
You can then see the Message Queues working in the server terminal both recieving messages and destroying their respective Message Queues after they are done sending and recieving the messages. 

### Note
The server and clients must be run in seperate command line terminals.