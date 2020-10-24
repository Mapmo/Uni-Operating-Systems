#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

/*
Напишете програма, която приема точно 2 аргумента. Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). 
Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа (uint16_t - вижте man stdint.h). Ако първият аргумент е:

--min - програмата отпечатва кое е най-малкото число в двоичния файл.
--max - програмата отпечатва кое е най-голямото число в двоичния файл.
--print - програмата отпечатва на нов ред всяко число.
*/
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Sowie wrong arguments");
	}

	if( strcmp(argv[1], "--min") && strcmp(argv[1], "--max") && strcmp(argv[1], "--print") )
	{
		errx(2, "%s is invalid", argv[1]);
	}
	
	int fd = open(argv[2], O_RDONLY);
	if ( fd == -1 )
	{
		err(3, "Can't open %s", argv[2]);
	}
	char buff;
	short int max=-30000, min=30000,curr;
	ssize_t read_size;
	
	while( (read_size = read(fd, &buff, 1)) > 0)
	{
		if( buff == ':')
		{	
			for(int i = 0; i<8; i++)
		{
			if((read_size = read(fd, &buff, 1)) == -1 ) //chete space
			{
				break;
			}
						
			if( (read_size = read(fd, &curr, 2)) == 1)
			{
				break;			
			}	
			if(curr > max)
			{
				max=curr;
			}
			if(curr < min)
			{
				min=curr;
			}
			if(strcmp(argv[1], "--print") == 0)
			{
				printf("%d\n", curr);
			}
			}
		}
	}
	close(fd);
	if(read_size == -1)
	{
		err(4, "Error while reading");
	}
	 if(strcmp(argv[1], "--max") == 0)
         {
        	printf("%d\n", max);
         }
	if(strcmp(argv[1], "--min") == 0)
        {
        	printf("%d\n", min);
        }

	exit(0);
}
