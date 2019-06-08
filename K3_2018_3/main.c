#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

void iClose(const int fd1, const int fd2)
{
	const int _errno = errno;
	close(fd1);
	close(fd2);
	errno = _errno;
}

void Swap(const int fd1, const int fd2) //Swap returns fd1 one step further than before
{
	unsigned tmp1, tmp2;
	if(read(fd1, &tmp1, 4) <= 0)
	{
		iClose(fd1, fd2);
		err(5, "Problem reading");
	}
	if(lseek(fd1, -4, SEEK_CUR) == -1)
	{
		iClose(fd1, fd2);
		err(6, "Problem seeking");
	}
	if(read(fd2, &tmp2, 4) <= 0)
	{
                iClose(fd1, fd2);
                err(5, "Problem reading");
	}
	if(lseek(fd2, -4, SEEK_CUR) == -1)
        {
                iClose(fd1, fd2);
                err(6, "Problem seeking");
        }
	//printf("in swap tmp1 is %u, tmp2 is %u\n", tmp1, tmp2);

	if(write(fd1, &tmp2, 4) == -1)
	{
		iClose(fd1, fd2);
                err(7, "Problem writing");
	}

        if(write(fd2, &tmp1, 4) == -1)
        {
                iClose(fd1, fd2);
                err(7, "Problem writing");
        }

}

int main(const int argc, const char* argv[])
{
	if(argc != 3)
	{
		errx(1, "wrong ammount of arguments");
	}

	pid_t pid = fork();

	if(pid == -1)
	{
		err(2, "fork failed");
	}

	if(pid == 0)
	{
		execlp("cp", "cp", argv[1], argv[2], (char*) NULL);
		err(3, "fork failed");
	}

	int status;
	wait(&status);

	if(WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
	{
		err(3, "fork failed");
	}

	int fdInd, iterator;
	
	fdInd = open(argv[2], O_RDWR);
	if(fdInd == -1)
	{
		err(4, "failed to open fdInd");
	}

	iterator = open(argv[2], O_RDWR);
	if(iterator == -1)
	{
		err(4, "failed to open iterator");
	}

	unsigned size, minInd, minVal, cur;
	if(lseek(fdInd, 0, SEEK_END) > 0)
	{
		size = lseek(fdInd, 0, SEEK_CUR);
	}

	for(unsigned i = 0; i < size - 4; i += 4)
	{
		minInd = i;
		if(lseek(fdInd, i, SEEK_SET) == -1)
                {
                        iClose(fdInd, iterator);
                        err(6, "Problem seeking");
                }

		if(lseek(iterator, i, SEEK_SET) == -1)
                {
                        iClose(fdInd, iterator);
                        err(6, "Problem seeking");
                }

		if(read(iterator, &minVal, 4) == -1)
		{
			iClose(fdInd, iterator);
			err(7, "read err");
		}
		
		for(unsigned j = i + 4; j < size; j += 4)
		{
			if(read(iterator, &cur, 4) == -1)
			{
				iClose(fdInd, iterator);
				err(7, "read err");
			}

			printf("After i:%u j:%u minInd:%u minVal:%u cur:%u\n", i, j, minInd, minVal, cur);
			
			if(cur < minVal)
			{
				
			// 	printf("After i:%u j:%u minVal:%u cur:%u\n", i, j, minVal, cur);
				minInd = j;
				minVal = cur;	
			}
		}
		
		if(i != minInd)
		{
			if(lseek(fdInd, i, SEEK_SET) == -1)
                	{
                        	iClose(fdInd, iterator);
                        	err(6, "Problem seeking");
                	}

                	if(lseek(iterator, minInd, SEEK_SET) == -1)
                	{
                        	iClose(fdInd, iterator);
                        	err(6, "Problem seeking");
                	}
			Swap(fdInd, iterator);
		}

	}

	exit(0);
}
