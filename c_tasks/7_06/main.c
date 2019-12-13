#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
 *Да се напише програма на С, която получава като параметър команда (без параметри) и при успешното ѝ изпълнение, извежда на стандартния изход името на командата.
*/

int main (int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(2, "sowie mayna");
	}
	int pid = fork();
	int status;
	if( pid > 0)
	{
		wait(&status);
		if(WIFEXITED(status) == 0)
		{
			err(5, "Child didn't terminate correctly");
		}

		if(WEXITSTATUS(status) ==  0)
		{
			//printf("child exited with code %d\n", status);
			execl("/usr/bin/basename", "velin", argv[1], NULL);
			err(1, "err while execling parent\n");
		}
		exit(1);
	}
	
	int fd = open("/dev/null", O_WRONLY);
	close(1);
	dup(fd);
	
	execl(argv[1], "argv", NULL); 
        err(1, "err while execling child\n");

}
