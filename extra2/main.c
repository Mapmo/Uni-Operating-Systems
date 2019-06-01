#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

int main(const int argc, const char* const argv[]) {

	if (argc != 2)
	{
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];

	if (pipe(pf) == -1)
	{
		errx(1, "Could not create pipe");
	}

	const pid_t child_pid = fork();
	if (child_pid == -1) 
	{
		err(1, "Could not fork.");
	}

	if (child_pid == 0)
	{
   		close(pf[0]);
        	write(pf[1], argv[1], strlen(argv[1]));
        	close(pf[1]);
		exit(0);		
	}

	wait(NULL);
	close(pf[1]);
	close(0);
	dup(pf[0]);
	
	execlp("wc", "wc", "-c", NULL);
	err(1, "Could not exec.");
}
