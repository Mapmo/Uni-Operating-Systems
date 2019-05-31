#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>

int main (int numb, char* args[])
{
	if(numb != 3)
	{
		//printf("wrong amount of arguments\n");
		errx(1, "wrong amount of arguments\n");
	}
	else
	{
		int fd1 = open(args[1], O_RDONLY);
		if (fd1 == -1)
		{
			err(2, "problem opening %s\n", args[1]);
		}

		int fd2 = open(args[2], O_WRONLY|O_TRUNC|O_CREAT, 0777);
		if (fd2 == -1)
		{
			close(fd1);
			err(3, "problem opening %s\n", args[2]);
		}

		char buff;
		ssize_t read_size;
		while( (read_size = read(fd1, &buff, 1)) > 0)
		{
			if (write(fd2, &buff, read_size) != read_size)
			{
				close(fd1);
		        close(fd2);
				err(4, "error occurred while writing\n");
			}
		}
		close(fd1);
        close(fd2);
		if(read_size < 0)
		{
			err(5, "error occurred while reading\n");
		}	
	}
	exit(0);
}
