#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/*
Напишете програма, която приема име на команда като параметър. След това чете редове от STDIN, като за всеки ред fork-ва ново 
копие на подадената команда и подава ѝ реда като аргументи (разделени по спейс).
*/

int main(const int argc, const char* const argv[]) {

	if (argc != 2)
	{
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int status, argCounter=1, buffCounter=0;
	//const int buff2Size = 500;
	char buff1;
	char * buff2[12];
	buff2[0] = malloc(1);
	buff2[0][0] = 'q';
	for(int i = 1; i<11; i++)
	{
		buff2[i] = NULL;
	}
	ssize_t rd;
	pid_t pid;
	while( (rd = read(0, &buff1, 1)) > 0)
	{
		if(buff1 == '\n')
		{
			pid = fork();
			if(pid == -1)
			{
				err(2, "Problem while forking");
			}
			
			if(pid == 0)
			{
				//printf("Buff is %s\n", buff2[0]);
				execv(argv[1], buff2);
				err(3, "problem execing in child");
			}

			wait(&status);
			if(WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
			{
				errx(4, "Problem with execution of child");
			}
			
			for(int i = 1; i < 11; i++)
			{
				free(buff2[i]);
				buff2[i] = NULL;
			}
			argCounter = 1;
			buffCounter = 0;
		}
		else if(buff1 == ' ')
		{
			argCounter++;
			buffCounter=0;	
		}
		else
		{	
			if(buffCounter == 0)
                        {
			
				if(argCounter == 11)
				{
					errx(5, "Screwed, more than 10 arguments");
				}
			
				buff2[argCounter] = malloc(50);
				for(int i =0; i<50; i++)
				{
					buff2[argCounter][i] = '\0';
				}
			}

			buff2[argCounter][buffCounter++] = buff1;
		}		
	}
	free(buff2[0]);
}
