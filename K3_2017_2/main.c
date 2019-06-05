#include <unistd.h>
#include <err.h>
#include <sys/types.h>
int main()
{
	int fd1[2];
	int fd2[2];
	int fd3[2];

	pipe(fd1);
	pipe(fd2);
	pipe(fd3);

	pid_t cut_pid;
	pid_t sort_pid;
	pid_t uniq_pid;

	if ((cut_pid = fork()) == -1)
 	{
		err(1, "fork cut");
	}

	if (cut_pid == 0) 
	{
		close(fd1[0]);
		if (dup2(fd1[1], 1) == -1) 
		{
			err(1, "dup2 cut");
		}

		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", NULL) == -1) 
		{
			err(1, "exec cut");
		}
	}
	
	close(fd1[1]);
	if ((sort_pid = fork()) == -1) 
	{
		err(1, "fork sort");
	}

	if (sort_pid == 0) 
	{
		close(fd2[0]);
		if (dup2(fd1[0], 0) == -1) 
		{
			err(1, "dup2 0 sort");
		}
		if (dup2(fd2[1], 1) == -1) 
		{
			err(1, "dup2 1 sort");
		}
		if (execlp("sort", "sort", "-", NULL) == -1) 
		{
			err(1, "exec sort");
		}
	}

	close(fd2[1]);
	if ((uniq_pid = fork()) == -1) 
	{
		err(1, "fork uniq");
	}	
	if (uniq_pid == 0) 
	{
		close(fd3[0]);
		if (dup2(fd2[0], 0) == -1) 
		{
			err(1, "dup2 2 uniq");
		}	
		if (dup2(fd3[1], 1) == -1) 
		{
			err(1, "dup2 3 uniq");
		}
		if (execlp("uniq", "uniq", "-c", NULL) == -1) 
		{
			err(1, "exec uniq");
		}
	}

	close(fd3[1]);
	if (dup2(fd3[0], 0) == -1) 
	{
		err(1, "dup2 3 sort -n");
	}
	execlp("sort", "sort", "-n", NULL);
	err(1, "exec sort -n");
}

