#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
Да се напише програма на С, която получава като параметър име на файл. Създава процес син, който записва стринга foobar във файла (ако не съществува, го създава, в противен случай го занулява), след което процеса родител прочита записаното във файла съдържание и го извежда на стандартния изход, добавяйки по един интервал между всеки два символа.
*/

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		errx(1, "wrong ammount of arguments\n");
	}
	
	int pid = fork();
	int status, fd, rsize, wsize;
	if(pid > 0)
	{
		wait(&status);

        	if(WIFEXITED(status) == 0)
        	{
          		err(5, "Child didn't terminate correctly");
        	}

		if(WEXITSTATUS(status) == 0)
		{
			char buff[2];

			fd = open(argv[1], O_RDONLY);
           	 	if (fd == -1)
            		{
			        err(2, "Parent can't open file");
            		}		

			while( (rsize = read(fd, buff, 2)) > 0)
			{
				if ((wsize = write(1, buff, rsize)) < 1)
				{
					err(3, "error while writing");
				}
				if(wsize == 2)
				{
					buff[0] = ' ';
					if ((wsize = write(1, buff, 1)) < 1)
	                                {
                	                        err(4, "error while writing space");
        	                        }
				}
			}

			if(rsize == -1)
			{	
				err(5, "error while reading");
			}
		}
	}
	else
	{
		fd=open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0777); //подредбата е много важна!!! fd=open(argv[1], O_WRONLY |O_CREAT, 0777|O_TRUNC) не trunc-ва 
		if(fd == -1)
		{
			err(6, "Child can't open file");
		}
	
		if ( write(fd, "foobar\n", 7) == -1)
		{
			err(7, "error while writing in child");
		}
	}

	close(fd);
	exit(0);
}




	




