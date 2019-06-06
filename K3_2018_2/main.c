#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

int CheckStatus(const int status)
{
	if(WIFEXITED(status) == 1 && WEXITSTATUS(status) == 0)
	{
		return 1;
	}
	return 0;
}

int main(const int argc, const char* argv[])
{
	if(argc != 2)
	{
		errx(1, "arguments");
	}
	
	int status, find[2], sort[2], tail[2];
	pid_t pid;

	if(pipe(find) == -1)
	{
		err(2, "pipe find");
	}

	pid = fork();

	if(pid == -1)
	{
		ClosePipe(find[0], find[1], -1);
		err(6, "fork failed");
	}

	if(pid == 0)
	{
		close(find[0]);
		if(dup2(find[1], 1) == -1)
		{
			ClosePipe(find[1], -1, -1);
			err(3, "dup2 find[1] 1");
		}

		execlp("find", "find", argv[1], "-type", "f", "-printf", "%Ts\t%p\n", (char*) NULL);
		ClosePipe(find[1], -1, -1);
		err(4, "child find");
	}
	
	wait(&status);

	if(CheckStatus(status) == 0)
	{
		ClosePipe(find[0], find[1], -1);
		err(5, "find");
	}

	close(find[1]);

	if(pipe(sort) == -1)
	{
		ClosePipe(find[0], -1, -1);
		err(2, "pipe sort");
	}

	pid = fork();
	
	if(pid == -1)
	{
		ClosePipe(find[0], sort[0], sort[1]);
	}

	if(pid ==0)
	{
		close(sort[0]);
		
		if(dup2(find[0], 0) == -1)
		{
			ClosePipe(find[0], sort[1], -1);
			err(3, "dup2 find[0] 0");
		}

		if(dup2(sort[1], 1) == -1)
		{
			ClosePipe(find[0], sort[1], -1);
			err(3, "dup2 sort[1] 1");
		}

		execlp("sort", "sort", (char*) NULL);
		ClosePipe(find[0], sort[1], -1);
		err(4, "sort child");
	}

	wait(&status);

	if(CheckStatus(status) == 0)
	{
		ClosePipe(find[0], sort[0], sort[1]);
		err(5, "sort");
	}

	close(find[0]);
	close(sort[1]);

	if(pipe(tail) == -1)
	{
		ClosePipe(sort[0], -1, -1);
		err(2, "sort pipe");
	}

	pid = fork();

	if(pid == -1)
	{
		ClosePipe(sort[0], tail[0], tail[1]);
		err(6, "fork tail");
	}

	if(pid == 0)
	{
		close(tail[0]);

		if(dup2(sort[0], 0) == -1)
		{
			ClosePipe(sort[0], tail[1], -1);
			err(3, "dup2 sort[0] 0");
		}

		if(dup2(tail[1], 1) == -1)
		{
			ClosePipe(sort[0], tail[1], -1);
			err(3, "dup2 tail[1] 1");
		}

		execlp("tail", "tail", "-n", "1", (char*) NULL);
		ClosePipe(sort[0], tail[1], -1);
		err(4, "child tail");
	}

	wait(&status);

	if(CheckStatus(status) == 0)
	{
		ClosePipe(sort[0], tail[0], tail[1]);
		err(5, "tail");
	}

	close(sort[0]);
	close(tail[1]);

	if(dup2(tail[0], 0) == -1)
	{
		ClosePipe(tail[0], -1, -1);
	}

	execlp("cut", "cut", "-f", "2", (char*) NULL);
       	ClosePipe(tail[0], -1, -1);
	err(5, "cut");
}
