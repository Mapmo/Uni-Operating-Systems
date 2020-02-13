#ifndef HEADER_FILE
#define HEADER_FILE

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

const int SHM_SIZE = 2048;
const char * const keyfile = "/dev/null";

char* connectString() 
{
	key_t key;
	int shmid;
	char* mem;
	
	int fd1;
	//validation that keyfile exists
	if((fd1 = open(keyfile, O_CREAT, 0700)) == -1) {
		err(1, "Error in opening the key file");
	}
	else {
		close(fd1);
	}

	//creating key
	if ((key = ftok(keyfile, 0)) == -1) {
        	err(1, "ftok");
    	}

	//creating segment
   	if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        	err(1, "shmget");
    	}

	//getting segment address
    	mem = shmat(shmid, NULL, 0);
    	if(mem == (char *)(-1)) {
		err(1, "shmat");
	}
	return mem;
}

void disconnectString(char* mem)
{
 	if (shmdt(mem) == -1) {
        	err(2, "shmdt");
    	}
}

#endif

