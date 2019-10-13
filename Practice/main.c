#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//implementation of cp

int main(const int argc, const char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Wrong amount of arguments");
	}

	int fd1, fd2;

	if((fd1 = open(argv[1], O_RDONLY)) == -1)
	{
		err(2, "Can't open arg1");
	}

	if((fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0777)) == -1)
	{
		close(fd1);
		err(2, "Can't open arg2");
	}
			
	char buff;

	while(read(fd1, &buff, 1) > 0)
	{
		if(write(fd2, &buff, 1) < 0)
		{
			err(3, "write err");
		}
	}
	
	close(fd1);
	close(fd2);

	exit(0);
}
