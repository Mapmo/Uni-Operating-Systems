#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
char CheckStatus(const int status)
{
	if(WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
	{
		return 0;
	}
	return 1;
}

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

int main(int argc, char* argv[])
{
	if(argc != 1)
	{
		errx(1, "Wrong amount of arguments in %s", argv[0]);
	}

	int cut[2], sort1[2], uniq[2], status;
	pid_t pid;

	if(pipe(cut) == -1)
	{
		err(2, "cut pipe failed");
	}

	pid = fork();

	if(pid == 0)
	{
		close(cut[0]);
		if(dup2(cut[1], 1) == -1)
		{
			ClosePipe(cut[1], -1, -1);
			err(5, "dup2 cut[1], 1");
		}
		execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*) NULL);
		ClosePipe(cut[1], -1, -1);
		err(3, "cut failed");
	}

	wait(&status);
	//printf("here");
	if(CheckStatus(status) == 0)
	{
		ClosePipe(cut[0], cut[1], -1);
		err(4, "child cut failed");
	}

	close(cut[1]);

	if(pipe(sort1) == -1)
	{
		ClosePipe(cut[0], -1, -1);
		err(2, "sort pipe failed");
	}

	pid = fork();

	if(pid == 0)
	{
		close(sort1[0]);
		if(dup2(cut[0], 0) == -1)
		{
			ClosePipe(cut[0], sort1[1], -1);
			err(5, "dup2 cut[0], 0");
		}
		if(dup2(sort1[1], 1) == -1)
		{
			ClosePipe(cut[0], sort1[1], -1);
			err(5, "dup2 sort1[1], 1");
		}
		execlp("sort", "sort1", (char*) NULL);
		ClosePipe(cut[0], sort1[1], -1);
		err(3, "sort1 failed");
	}

	wait(&status);
	if(CheckStatus(status) == 0)
	{
		ClosePipe(cut[0], sort1[0], sort1[1]);
		err(4, "Child sort 1 failed");
	}

	if(pipe(uniq) == -1)
	{
		ClosePipe(cut[0], sort1[0], sort1[1]);
		err(2, "uniq pipe failed");
	}

	ClosePipe(cut[0], sort1[1], -1);
	pid = fork();

	if(pid == 0)
	{
		close(uniq[0]);
		if(dup2(sort1[0], 0) == -1)
		{
			ClosePipe(sort1[0], uniq[1], -1);
			err(5, "dup2 sort1[0] 0");
		}
		if(dup2(uniq[1], 1) == -1)
		{
			ClosePipe(sort1[0], uniq[1], -1);
			err(5, "dup2 uniq[1] 1");
		}
		execlp("uniq", "uniq", "-c", (char*) NULL);
		ClosePipe(sort1[0], uniq[1], -1);
		err(3, "uniq failed");
	}

	wait(&status);
	if(CheckStatus(status) == 0)
	{
		ClosePipe(sort1[0], uniq[0], uniq[1]);
		err(4, "Child uniq failed");
	}

	ClosePipe(sort1[0], uniq[1], -1);

	if(dup2(uniq[0], 0) == -1)
	{
		ClosePipe(uniq[0], -1, -1);
		err(5, "dup2 uniq[0] 0");
	}

	execlp("sort", "sort2", "-n", (char*) NULL);
	ClosePipe(uniq[0], -1, -1);

	err(3, "sort2 failed");
}
