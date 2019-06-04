#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
 #include <sys/types.h>
#include <sys/wait.h>
/*
Ако родител умре преди дете, то ppid става 1
*/
int main()
{
	pid_t parent = getpid(), child, childchild;
	printf("Parent pid:%d\n", parent);
	wait(NULL);	
	child = fork();
	
	if(child == 0)
	{
		child = getpid();
		for(int i = 0; i < 500000; i++);
		printf("Child pid: %d\n", child);
		
		childchild = fork();
		
		if(childchild > 0)
		{
			printf("Parent's pid that was passed via for is: %d\n", parent);
			parent = getppid();
			printf("ppid in child is: %d\n", parent);
			printf("Child's child's pid:%d\n", childchild);
		}	
	}
	else
	{
		printf("Parent killed\n");
		exit(0); //sled kato terminirame roditelq outputa v terminala shte e kofti :(((
	}
	
	exit(0);
}
