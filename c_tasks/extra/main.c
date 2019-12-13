#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Напишете програма, която приема като аргументи имена на две команди, и след това прочита редове от STDIN. Четните редове се пращат на STDIN на първата команда,
 *  а нечетните - на STDIN на втората. Програмата излиза, когато и двете команди излязат.
 */

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "%s has wrong arguments", argv[0]);
	}

	char buff;
	size_t rd;

	int pipe1[2];
	int pipe2[2];
	int status;

	if(pipe(pipe1) == -1)
	{
		err(3, "Could not pipe1");
	}

	pid_t first = fork();

	if(first == -1)
	{
		close(pipe1[0]);
		close(pipe1[1]);
		err(4, "Could not fork 1");
	}
	if(first > 0)
	{
		close(pipe1[0]);

		if(pipe(pipe2) == -1)
		{
			close(pipe1[1]);
			err(5, "Could not pipe2");
		}

		pid_t second = fork();
		if(second ==-1)
		{
			close(pipe1[1]);
			close(pipe2[0]);
			close(pipe2[1]);
			err(6, "Could not fork 2");
		}

		if(second > 0)
		{
			close(pipe2[0]);
			int counter = 1; //counter values: 1 means uneven, -1 means that only process 1 is running, 2 means even, -2 means only process 2 is running
			while(1)
			{	
                        	close(1);
                                if(counter == -1)
                                {
					waitpid(first, &status, WNOHANG);
					if(WIFEXITED(status))
					{
						break;
					}
                                	dup(pipe1[1]);
                                }
                                else if(counter == -2)
                                {	
					waitpid(second, &status, WNOHANG);
                                        if(WIFEXITED(status))
                                        {
                                                break;
                                        }
                                	dup(pipe2[1]);
                               	}
				else if(counter == 1)
				{
					waitpid(second, &status, WNOHANG);
                                        if(WIFEXITED(status))
                                        {
                                	        counter = -1;
                                        }
					else
					{
                                		counter = 2;
					}
					dup(pipe1[1]);
				}
				else if(counter == 2)
				{
					waitpid(first, &status, WNOHANG);
                                        if(WIFEXITED(status))
                                        {
                                        	counter = -2;
                                        }
                                       	else
                                        {
					     	counter = 1;	
					}
					dup(pipe2[1]);
				}
				else
				{
					errx(9, "Not supposed to have counter value: %d", counter);
				}

				do
				{
					rd = read(0, &buff, 1);
					if(rd <= 0)
					{
						errx(2, "Problem while reading");
					}
					write(1, &buff, 1);
				} while(buff != '\n');	
			}
			close(pipe1[1]);
			close(pipe2[1]);
		}
		else
		{
			close(pipe1[1]);
			close(pipe2[1]);
			close(0);
			dup(pipe2[0]);
			execl(argv[2], "second pipe", (char *) NULL);
			err(8, "Problem with 2nd exec");
		}
	}
	else
	{	
		close(pipe1[1]);
		close(0);
		dup(pipe1[0]);
		execl(argv[1], "first pipe", (char *) NULL);
		err(7, "Error while execling in first child");
	}
	exit(0);
}
