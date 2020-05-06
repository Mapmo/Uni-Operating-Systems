#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(const int argc, const char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Inappropriate args");
	}

	char buff;
        size_t size;
        char rd, print;

	if(argv[1][0] == '-')
	{
		if(argv[1][1] == 'd')
		{
			size = strlen(argv[2]);

			while((rd = read(0, &buff, 1)) > 0)
			{
				print = 1;
				for(size_t i = 0; i < size; i++)
				{
					if(argv[2][i] == buff)
					{
						print = 0;
						break;
					}	
				}

				if(print == 1)
				{
					if(write(1, &buff, 1) < 1)
					{
						err(3, "write err");
					}
				}
			}

		}
		else if(argv[1][1] == 's')
		{
			size = strlen(argv[2]);
			char truncActive = 0, charToTrunc; //truncActive means that there is a symbol to trunc
			
			while((rd = read(0, &buff, 1)) > 0)
			{
				if(truncActive == 1 && charToTrunc == buff)
				{
					continue;
				}
				
				truncActive = 0;

				if(write(1, &buff, 1) < 1)
                                {
                                    	err(3, "write err");
				}

				for(size_t i = 0; i < size; i++)
				{
					if(argv[2][i] == buff)
					{
						truncActive = 1;
						charToTrunc = buff;
						break;
					}
				}
			}
		}
		else
		{
			errx(1, "Unknown option");
		}
	}
	else
	{	
		size = strlen(argv[1]);
		if(size != strlen(argv[2]))
		{
			errx(1, "set1 and set2 have different sizes");
		}
		
		while((rd = read(0, &buff, 1)) > 0)
		{
			for(size_t i = 0; i < size; i++)
			{
				if(buff == argv[1][i])
				{
					buff = argv[2][i];
					break;
				}
			}

			if(write(1, &buff, 1) < 1)
                        {
                            	err(3, "write err");
                        }
		}
	}
	if(rd < 0)
	{
		err(2, "read err");
	}
	
	exit(0);
}
