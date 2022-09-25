// Author: Dr. Shital Joshi
// Email: shital.joshi@okstate.edu

// Description: Using ordinary pipe to pass data between the parent and the child processes

#include <stdio.h>
#include <unistd.h>	// for fork
#include <sys/types.h>	// for pid_t
#include <stdlib.h>	// for exit(1)
#include <string.h>	// for strlen
#include <sys/wait.h>	// for wait

#define MAX 9000000000 // 9 billion times

int main(void)
{
        int fd[2];	//fd[0] for read end of the pipe; fd[1] for write end of the pipe
	
	long long int result = 0;
	long long int i;

        pipe(fd);
        
	pid_t pid = fork();

        if(pid < 0) { 	// fork failed: exit
                perror("fork");
                exit(1);
        }

        else if(pid == 0) {	// Child process: new process 
                close(fd[0]); 	// Close the output side of the child process 

		for (i = 1; i <= MAX/2; i++){
			result = result + i;
		}
                /* Read in a string from the pipe */
                write(fd[1], &result, sizeof(result));
		close(fd[1]);
        }

        else {	// Parent process
		long long resultChild = 0;
                close(fd[1]);	// Close the input side of the pipe
 		for (i = MAX/2+1; i <= MAX; i++){
			result = result + i;
		}
		wait(NULL);
                /* Send "text" through the output side of pipe */
                read(fd[0], &resultChild, (sizeof(result)));
		close(fd[0]);
		
		printf("\nThe final result is: %lld\n", (result + resultChild)); 	// Print the read message 

                exit(0);   

        }
        
        return(0);
}