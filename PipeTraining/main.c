#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

void ClosePipe(const int fd1, const int fd2, const int fd3)
{
	const int _errno = errno;
	close(fd1);
	if(fd2 != -1)
	{
		close(fd2);
		if(fd3 != -1)
		{	
			close(fd3);
		}
	}
	errno = _errno;
}	

int main(const int argc, const char* argv[])
{	
	if(argc != 1)
	{
		errx(1, "wrong args in %s", argv[0]);
	}

	int tail[2], sed[2], sort[2];
	pid_t pid;

	if(pipe(tail) == -1)
	{
		err(2, "pipe failed");
	}
	
	pid = fork();
	if(pid == -1)
	{
		ClosePipe(tail[0], tail[1], -1);
		err(3, "fork failed");
	}

	if(pid == 0)
	{
		close(tail[0]);
		if(dup2(tail[1], 1) == -1)
		{
			ClosePipe(tail[1], -1, -1);
			err(4, "dup2 fail");
		}
		execlp("tail", "tail", "-n", "+2", "example", (char*) NULL);
		ClosePipe(tail[1], -1, -1);
		err(5, "tail fail");
	}
	
	close(tail[1]);
	
	if(pipe(sed) == -1)
	{
		ClosePipe(tail[0], -1, -1);		
	}

	pid = fork();

        if(pid == -1)
        {
                ClosePipe(tail[0], sed[1], sed[0]);
        }

        if(pid == 0)
        {
                close(sed[0]);
                if(dup2(sed[1], 1) == -1 || dup2(tail[0], 0))
                {
                        ClosePipe(sed[1], tail[0], -1);
                        err(4, "dup2 fail");
                }

                execlp("sed", "sed", "-r", "s/([^\t]*)\t([^\t]*)/\\2\t\\1/", (char*) NULL);
          	//execlp("tail", "tail", "-n", "+2", "example", (char*) NULL);       
		ClosePipe(tail[0], sed[1], -1);
                err(5, "sed fail");
        }
	
	
	close(tail[0]);
        close(sed[1]);

	if(pipe(sort) == -1)
	{	
		ClosePipe(sed[0], -1, -1);
	}
	
	pid = fork();

        if(pid == -1)
        {
                ClosePipe(sort[0], sort[1], sed[0]);
        }

        if(pid == 0)
        {
                close(sort[0]);
                if(dup2(sort[1], 1) == -1 || dup2(sed[0], 0))
                {
                        ClosePipe(sort[1], sed[0], -1);
                        err(4, "dup2 fail");
                }
                execlp("sort", "sort", (char*) NULL);
                ClosePipe(sed[0], sort[1], -1);
                err(5, "sort fail");
        }

	close(sed[0]);
	close(sort[1]);
	
	if(dup2(sort[0], 0) == -1)
	{
	      	err(4, "dup2 fail");
	}

	execlp("head", "head", "-n", "1", (char*) NULL);
	ClosePipe(sort[0], -1, -1);
	err(5, "head failed");
}








































