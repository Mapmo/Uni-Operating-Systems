#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

void Close(const int fd1, const int fd2)
{
	const int _errno = errno;
	close(fd1);
	close(fd2);
	errno = _errno;
}

void Swap(const int fd1, const int fd2)
{
	char c1, c2;
	if(read(fd1, &c1, 1) == -1 || read(fd2, &c2, 1) == -1)
	{
		Close(fd1, fd2);
		err(4, "err read in swap");
	}

	if(lseek(fd1, -1, SEEK_CUR) == -1 || lseek(fd2, -1, SEEK_CUR) == -1)
	{
		Close(fd1, fd2);
		err(5, "err lseek in swap");
	}

	if(write(fd1, &c2, 1) == -1 || write(fd2, &c1, 1) == -1)
	{
		Close(fd1, fd2);
		err(6, "err lseek in write");
	}
}

int main(const int argc, const char* argv[])
{
	if(argc != 2)
	{
		errx(1, "Invalid arguments");
	}

	int fd1, fd2;

	if((fd1 = open(argv[1], O_RDWR)) == -1)
	{
		err(2, "err open fd1");
	}
	if((fd2 = open(argv[1], O_RDWR)) == -1)
	{
		close(fd1);
		err(2, "err open fd2");
	}

	const off_t size = lseek(fd1, 0, SEEK_END);
	if(size == -1)
	{
		Close(fd1, fd2);
		err(3, "lseek end err");
	}

	if(lseek(fd1, 0, SEEK_SET) != 0)
	{
		Close(fd1, fd2);
		err(3, "lseek beg err");
	}

	off_t minInd;
	uint8_t cur, minVal;
	
	for(off_t i = 0; i < size - 1; i++)
	{
		minInd = i;
		if(lseek(fd1, i, SEEK_SET) == -1 || lseek(fd2, i, SEEK_SET) == -1)
		{
			Close(fd1, fd2);
			err(3, "lseek i err");
		}
		if(read(fd2, &minVal, 1) < 1)
		{
			Close(fd1, fd2);
			err(4, "read i err");
		}

		for(off_t j = i + 1; j < size; j++)
		{
			if(read(fd2, &cur, 1) < 1)
                	{
                        	Close(fd1, fd2);
                        	err(4, "read i err");
                	}

                //        printf("i:%ld j:%ld cur:%d\n", i, j, cur);

			if(cur < minVal)
			{
				minVal = cur;
				minInd = j;
			}
		}

		if(i != minInd)
		{
			if(lseek(fd1, i, SEEK_SET) == -1 || lseek(fd2, minInd, SEEK_SET) == -1)
	                {
        	                Close(fd1, fd2);
                	        err(3, "lseek swap err");
                	}

			Swap(fd1, fd2);
		}
	}

	close(fd1);
	close(fd2);
}
