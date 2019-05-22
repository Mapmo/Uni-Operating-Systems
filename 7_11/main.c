#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри) и име на файл в текущата директория. Ако файлът не съществува,
 * го създава. Програмата изпълнява командите последователно, по реда на подаването им. Ако команда завърши успешно, програмата добавя нейното име към 
 * съдържанието на файла, подаден като команден параметър. Ако командата завърши неуспешно, програмата уведомява потребителя чрез подходящо съобщение.
*/

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		errx(1, "Wrong ammount of parameter\n");
	}

	int fd = open(argv[3], O_WRONLY | O_APPEND |O_CREAT, 0666);
	if(fd == -1)
	{
		err(2, "Error while reading");
	}
	
	pid_t pid;
	int status;
	for(int i = 1; i < 3; i++)
	{
		pid = fork();
		
		if(pid>0)
		{
			wait(&status);

			if(WEXITSTATUS(status) == 0)
			{
				int j = 0;
				while(argv[i][j] != '\0')
				{
					if(write(fd, &(argv[i][j]), 1) == -1)
					{
						err(3, "Error while writing to file");
					}
					j++;
				}
				char buff = '\n';
				if(write(fd, &buff, 1) == -1)
                                {
                                	err(3, "Error while writing to file");
                                }
			}
		}
		else
		{
			execl(argv[i], "task", (char*) NULL);
			err(4, "Process %s failed", argv[i]);
		}
	}
	close(fd);
	exit(0);
}
