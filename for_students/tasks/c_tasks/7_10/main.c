#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

//Да се напише програма на C, която изпълнява последователно подадените ѝ като параметри команди, като реализира следната функционалност постъпково:
//main cmd1 ... cmdN Изпълнява всяка от командите в отделен дъщерен процес.
//... при което се запазва броя на изпълнените команди, които са дали грешка и броя на завършилите успешно.

int main (int argc, char* argv[])
{
	pid_t pid;
	int status;
	unsigned suc_counter = 0, err_counter = 0;
	for(int i = 1; i < argc; i++)
	{
		pid = fork();
		
		if(pid > 0)
		{
			wait(&status);
			if(WEXITSTATUS(status) == 0)
			{
				suc_counter++;
			}
			else
			{
				err_counter++;
			}
		}
		else
		{
			execl(argv[i], "child", (char*) NULL);
			exit(1);
		}
	}
	printf("The amount of successful calls is: %d\nThe amount of failed calls is: %d\n", suc_counter, err_counter);	
	exit(0);
}
