#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
int main ()
{

		int pid = fork();
	       	if(pid > 0)
		{
			wait(&pid); //needed for the last condition
			pid = fork();
		
			if(pid == 0)
			{
				printf("bar\n");
			}
		}	
		else
		{
			printf("foo\n");
		}

		exit(0);
}
