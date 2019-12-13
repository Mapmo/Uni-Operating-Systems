#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

/*
 *This program reads a binary file similar to the xxd program.
*/

void AddExtra0(const unsigned short numb)
{
	if(numb<4096)
        {
		printf("0");
                if(numb<256)
                {
			printf("0");
                        if(numb<16)
                        {
	                        printf("0");
                        }
                }
	}
}

int main ()
{
	int fd = open("b", O_RDONLY);

	if(fd == -1)
	{
		err(1, "ashko precaka otvarqneto na faila\n");
	}

	ssize_t rsize;//, wsize;
	char buff[2];
	unsigned short int numb;
       	char endOfLineCounter = 0;

	while((rsize=read(fd, &buff, 2)) > 0)
	{
		numb = (short int)buff[1] | (short int)buff[0]<<8;
		AddExtra0(numb);
		printf("%x\t", numb);
		endOfLineCounter++;
		if((endOfLineCounter % 8) == 0)
		{
			printf("\n");
			endOfLineCounter = 0; //to avoid overflow
		}
	}

	close(fd);

	if(rsize == -1)
	{
		err(3, "problem with reading\n");
	}

	if((endOfLineCounter % 8) != 0)
	{
		printf("\n");
	}

	exit(0);
}
