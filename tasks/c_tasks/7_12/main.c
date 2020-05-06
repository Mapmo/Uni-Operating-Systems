#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

/*
Да се напише програма на C, която получава като командни параметри две команди (без параметри). Изпълнява ги едновременно и извежда на стандартния 
изход номера на процеса на първата завършила успешно. Ако нито една не завърши успешно извежда -1.
*/

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "wrong amount of parameters in %s", argv[0]);
	}
	
	int status;
	pid_t pid1 = fork(), pid2, pid3;
	sem_t mutex;
	sem_init(&mutex, 1, 1);

	if(pid1 > 0)
	{
		pid2 = fork();
		if(pid2 > 0)
        	{

			pid3 = wait(&status);
			pid3 = WEXITSTATUS(status);
			if(pid3  == 101 || pid3 == 102)
			{
				//printf("pid is %d\n", pid3);
				pid3 -= 100;
				printf("%d\n", pid3);
				sem_post(&mutex);
				sem_destroy(&mutex);
				exit(0);
			} 

			sem_post(&mutex);
			sem_destroy(&mutex);

			pid3 = wait(&status);
                        pid3 = WEXITSTATUS(status);

			if(pid3 == 101 || pid3 == 102)
                       	{
				pid3 -= 100;
			}
			else
			{
				pid3 = -1;
			}
			
			printf("%d", pid3);
			exit(0);
		}
        	else if(pid2 == -1)
        	{
                	err( 4, "problem with second fork");
        	}
       		else
        	{
			sem_wait(&mutex);
			pid2 = fork();
			if(pid2 == 0)
			{
				int fd=open("/dev/null",O_WRONLY);
	   			dup2(fd,1);
	  			dup2(fd,2);
                		execl(argv[2], "process2", (char*) NULL);
                		err(5, "problem execling second command");
			}
			else
			{
				wait(&status);
				if(WEXITSTATUS(status) == 0)
				{
					exit(102);
				}
				exit(1);
			}
        	}
	}
	else if(pid1 == -1)
	{
		err(2, "problem with first fork");
	}
	else
	{
		sem_wait(&mutex);
		pid1 = fork();
		if(pid1 == 0)
		{
			int fd=open("/dev/null",O_WRONLY);
	    		dup2(fd,1);
	    		dup2(fd,2);
			execl(argv[1], "process1", (char*) NULL);
			err(3, "problem execling first command");
		}
		else
		{
				wait(&status);
                                if(WEXITSTATUS(status) == 0)
                                {
                                        exit(101);
                                }
                                exit(1);
		}
	}
}






	












