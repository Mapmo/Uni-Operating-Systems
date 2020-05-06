#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"
*/
int main(int argc, char* argv[])
{
	if (argc != 1)
	{
		err(1, "%s requires no arguments", argv[0]);
	}
	int src = open("/etc/passwd", O_RDONLY);
	
	if (src == -1)
	{
		err(2, "Cant't open /etc/passwd");
	}
	
	int dst = open("etc_passwd", O_WRONLY|O_TRUNC|O_CREAT, 0666);
	
	if ( dst == -1)
	{
		close(src);
		err(3, "Can't open the new file :(");
	}

	char buff;
	ssize_t read_size, write_size;	
	while((read_size = read(src, &buff, 1)) > 0)
	{
		if(buff == ':')
		{
			buff = '?';
		}

		if((write_size = write(dst, &buff, read_size)) == -1)
		{
			close(src);
			close(dst);
			err(4, "Problem while writing");
		}
	}

	close(src);
        close(dst);

	if(read_size == -1)
	{
		err(5, "Problem while reading");
	}

	exit(0);
}
