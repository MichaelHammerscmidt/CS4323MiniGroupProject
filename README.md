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
You can type anything to the server and the server will recieve it and then send it back to the clinet
and you get rid of each client by typing exit to the server.

### Note
The server and clients must be run in seperate command line terminals.