//file: TwoPipesTwoChildren.cpp
//author: Alfred Abenojar 
//date: 04/07/2017
//description:
//this program executes "ls -ltr | grep 3376 | wc -l" , by dividing the three commands 
// among the two children and parent process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char **argv)
{
 int status;
 int childpid,childpid2;
 char *ls_args[] = {"ls", "-ltr", NULL};
 char *grep_args[] = {"grep", "3376", NULL};
 char *wc_args[] = {"wc", "-l", NULL};

 // create one pipe to send the output of "ls" process to "grep" process
 int pipes[2];
 int pipes2[2];
 pipe(pipes);
 pipe(pipes2);
 
 // fork the first child (to execute ls)
 if((childpid = fork()) == -1)
 {
 	perror("Error creating a child process");
	exit(1);
 }

 if (childpid == 0) 
 {
        if ( (childpid2 = fork()) == -1)
	{
		perror("Error creating the 2nd child process");
		exit(1);
	}

	if (childpid2 == 0)
	{
		//replace ls's std out with write part of 1st pipe
		dup2(pipes[1], 1);

		//closing pipes before executing program
		close(pipes[0]);
 		close(pipes[1]);
		close(pipes2[0]);
		close(pipes2[1]);
 		execvp(*ls_args, ls_args);
 		exit(0);

	}

	else
	{
 		// replace grep's stdout with write part of 1st pipe
 		dup2(pipes[0], 0);
		close(pipes[0]);
 		// close all pipes (very important!); end we're using was safely copied
 		close(pipes[1]);
		close(pipes2[0]);
		dup2(pipes2[1],1);
		close(pipes2[1]);
 		execvp(*grep_args, grep_args);
 		exit(0);
	}
 }

 else 
 {
 	// replace grep's stdin with read end of 1st pipe
 	dup2(pipes2[0], 0);
	close(pipes2[0]);
 	close(pipes[0]);
 	close(pipes[1]);
	close(pipes2[1]);
 	execvp(*wc_args, wc_args);
 }
 
return(0);
}
