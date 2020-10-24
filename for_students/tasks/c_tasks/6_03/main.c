#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
Реализирайте команда cat, работеща с произволен брой подадени входни параметри.
*/
int main( int argc, char* argv[])
{
	int i = 1;
	while( i < argc )
	{
		int fd = open(argv[i], O_RDONLY);
		if ( fd == -1 )
		{
			err(1, "File missing %s", argv[i]);
		}
		char c;
		int read_size;
		while ( (read_size = read(fd, &c, 1)) > 0 )
		{
			printf("%s", &c);
		}
		if(read_size == -1)
		{
			err(2, "Error while reading\n");
		}
		i++;
	}
	exit(0);
}
