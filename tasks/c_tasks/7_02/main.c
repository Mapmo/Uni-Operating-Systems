#include <unistd.h>
#include <err.h>

int main (int argc, char* argv[])
{
		if(argc != 2)
		{
			errx(1, "Wrong ammount of arguments");
		}
		execl("/bin/ls", "ls", "-l", "-h", "-a", argv[1], NULL); 
		//execl("/bin/ls", "ls", "-lh",argv[1], NULL); works
		err(99, "err execling");
}
