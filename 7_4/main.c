#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main ()
{
	int n = 101, status = 1;

	if (fork() > 0) {
		wait(&status);
		for (int i = 0; i < n; i++) {
			printf("father\n");
		}
	} else {
		for (int i = 0; i < n; i++) {
			printf("son\n");
		}
	}
	printf("Final status is %d\n", status);
	exit(0);
}
