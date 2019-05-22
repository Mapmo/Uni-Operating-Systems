#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
//Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри).
//Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код -1.

int main (int argc, char* argv[])
{
	if(argc != 3)
	{
		errx(1, "Invalid ammount of arguments\n");
	}

	int pid = fork(), status;
	if(pid > 0)
	{
		wait(&status);
		if(WEXITSTATUS(status) == 0)
		{
			execl(argv[2], "second_process", (char*) NULL);
			err(2, "execl err calling %s", argv[2]);
		}
		else
		{
			exit(-1);
		}
	}

	execl(argv[1], "first_process", (char*) NULL);
	exit(-1);
}
