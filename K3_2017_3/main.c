#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ReadClose(const int rd, const int fd)
{
	if(rd < 0)
	{
		const int _errno = errno;
		close(fd);
		errno = _errno;
		err(2, "problem reading");
	}
}

int main(const int argc, char* argv[])
{
	if(argc == 1)
	{
		exit(0);
	}

	char buf, in = 0;
      	const char n = (strcmp(argv[1], "-n") == 0) ? 2 : 1;
	int fd, rd, counter = 1;

	for(int i = n; i < argc; i++) //ako argv[1] e option shte zapochnem ot 2
	{
		if(strcmp(argv[i], "-") == 0)
		{
			fd = 0;
			in = 1;
		}
		else
		{
			fd = open(argv[i], O_RDONLY);
			if(fd < 0)
			{
				err(1, "Can't open %s", argv[i]);
			}
		}

		if(n == 1)
		{
			while((rd = read(fd, &buf, 1)) > 0)
			{
				printf("%c", buf);
			}
			
			ReadClose(rd, fd);
		}
		else
		{
			while((rd = read(fd, &buf, 1)) > 0)
			{
				printf("\t%d %c", counter, buf);
				if(buf == '\n')
                                {
                                         counter++;
                                         continue;
                                }

				while((rd = read(fd, &buf, 1)) > 0)
                        	{
					printf("%c", buf);
					if(buf == '\n')
					{
						counter++;
						break;
					}	
                       	 	}

				ReadClose(rd, fd);
			}
			ReadClose(rd, fd);
		}

		if(in == 1)
		{
			in = 0;
		}
		else
		{
			close(fd);
		}
	}
}
