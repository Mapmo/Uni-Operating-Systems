#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	int n = 101;

	if (fork() > 0) {
		for (int i = 0; i < n; i++) {
			printf("father\n");
		}
	} else {
		for (int i = 0; i < n; i++) {
			printf("son\n");
		}
	}

	exit(0);
}
