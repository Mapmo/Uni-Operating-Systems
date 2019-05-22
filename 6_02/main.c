#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1, "Only 1 argument please\n");
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		err(2, "Problem loading the file\n");
	}

	ssize_t read_size;
	char buff;
	unsigned int lines = 0;
	unsigned int words = 0;
	unsigned int chars = 0;
	while( (read_size = read(fd, &buff, 1)) > 0)
	{
		if(buff == '\n')
		{
			lines++;
			words++;
		}
		else if(buff == ' ' || buff == '\t')
		{
			words++;
		}
		chars++;		
	}
	close(fd);
	if(read_size<0)
	{
		err(3, "Error during reading\n");
	}
	printf("%d %d %d\n", lines, words, chars);
	exit(0);
}
