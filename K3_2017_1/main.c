#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

const char M1 = -1;

void CloseAll(const int fd1, const int fd2, const int patch)
{
	const int _errno = errno;
	close(fd1);
	if(fd2 != M1)
	{
		close(fd2);
		if(patch != M1)
		{
			close(patch);
		}
	}
	errno = _errno;
}

int main(const int argc, const char* argv[])
{
	if(argc != 4)
	{
		errx(1, "Wrong ammount of arguments");
	}

	int fd1, fd2, patch;

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == M1)
	{
		err(2, "Can't open f1");
	}

	fd2 = open(argv[2], O_RDONLY);
	if(fd2 == M1)
	{
		CloseAll(fd1, M1, M1);
		err(3, "Can't open f2");
	}

	off_t size1 = lseek(fd1, 0, SEEK_END);
	off_t size2 = lseek(fd2, 0, SEEK_END);
	
	if(size1 == -1 || size2 == -1)
	{
		CloseAll(fd1, fd2, M1);
		err(5, "Problem lseeking");
	}

	if(size1 != size2)
	{
		CloseAll(fd1, fd2, M1);	
		errx(4, "Different sizes");
	}

	if(lseek(fd1, 0, SEEK_SET) == -1 || lseek(fd2, 0, SEEK_SET) == -1)
        {
		CloseAll(fd1, fd2, M1);
                err(4, "problem going to beg");
        }

        patch = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(patch == M1)
        {
                CloseAll(fd1, fd2, M1);
        }

	int rd1, rd2;
	char buff1, buff2;

	while((rd1 = read(fd1, &buff1, 1)) > 0 && (rd2 = read(fd2, &buff2, 1)) > 0)
	{
		if(buff1 != buff2)
		{
			int16_t tmp = lseek(fd1, 0, SEEK_CUR) - 1;
			if(tmp == -1)
			{
				CloseAll(fd1, fd2, patch);
				err(8, "Error trying to find prev fd position");
			}

			if(write(patch, &tmp, 2) < 2)
			{
				CloseAll(fd1, fd2, patch);
				err(7, "Error while writing");
			}
			if(write(patch, &buff1, 1) < 1)
			{
				CloseAll(fd1, fd2, patch);
                                err(7, "Error while writing");
			}
			if(write(patch, &buff2, 1) < 1)
                        {
                                CloseAll(fd1, fd2, patch);
                                err(7, "Error while writing");
                        }
		}
	}
	CloseAll(fd1, fd2, patch);

	if(rd1 == -1 || rd2 == -1)
	{
		err(8, "Problem while reading");
	}

	exit(0);
}
