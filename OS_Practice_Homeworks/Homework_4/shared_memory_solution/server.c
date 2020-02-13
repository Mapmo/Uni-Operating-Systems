#include <stdint.h>
#include "comm.h"

const int NUM_OF_ACCOUNTS = 8;
const int ACCOUNT_SIZE = 4;

sem_t * createSemaphore(const char* semaphoreName, const int semaphoreSize)
{
        sem_t* sem = sem_open(semaphoreName, O_CREAT | O_EXCL, 0700, semaphoreSize);
        if(sem == SEM_FAILED)
        {
                // if the semaphore was not unlinked, unlink it
                sem_unlink(semaphoreName);
                sem = sem_open(semaphoreName, O_CREAT, 0700, semaphoreSize);
        }
        return sem;
}

int openAccountsFile(const char* accountsFile)
{
	int fd1 = open(accountsFile, O_RDWR | O_CREAT, 0700);

        if(fd1 < 0) {
                err(2, "Error in opening %s", accountsFile);
        }

        int marker = lseek(fd1, 0, SEEK_END);

	if(marker < 0) {
		close(fd1);
		err(2, "Error in moving marker to SEEK_END in %s", accountsFile);
	}
	else 
	{
		if(marker != 32) {
			if(ftruncate(fd1, 0) < 0) {
				close(fd1);
				err(2, "Error in truncating %s", accountsFile);
			}
			marker = 0;
		}

		if(marker == 0) {
			int initializer = 0;
	       		for(int i = 0; i < NUM_OF_ACCOUNTS; ++i) {
				if(write(fd1, &initializer, ACCOUNT_SIZE) < ACCOUNT_SIZE) {
					close(fd1);
					err(1, "Error in writing in the %s", accountsFile);
				}
			}
		}
        }

	return fd1;
}

void release(const int fd, const char* a, const char* b, const char* c)
{
	close(fd);
	sem_unlink(a);
	sem_unlink(b);
	sem_unlink(c);
}
int main(const int argc, const char* argv[])
{

	if(argc != 2) {
                errx(1, "Wrong number of arguments! Usage: ./server name_of_file_with_accounts");
        }
	
	int fd1 = openAccountsFile(argv[1]);

	if(fd1 < 0) {
		err(2, "Error in opening accounts' file");
	}

	char * mem = connectString();

	sem_t* s = createSemaphore("s", 0), *c = createSemaphore("c", 1),* a = createSemaphore("a", 0);

	while(ACCOUNT_SIZE < NUM_OF_ACCOUNTS) {      	
		
		sem_wait(s);
        
                int accountNumber = mem[0];

		if(lseek(fd1, accountNumber * ACCOUNT_SIZE, SEEK_SET) < 0) {
			release(fd1, "c", "s", "a");
			err(2, "Error in lseek 1 main");
		}

		uint32_t currentBalance;
		int16_t clientAction;

		if(read(fd1, &currentBalance, ACCOUNT_SIZE) < ACCOUNT_SIZE ) {
			release(fd1, "c", "s", "a");
			err(2, "Error in reading current account information");
		}

        sprintf(mem, "%u", currentBalance);

		sem_post(a);
		sem_wait(s);
		
		if(sscanf(mem, "%hd", &clientAction) == EOF) {
		        release(fd1, "c", "s", "a");
			err(2, "Error in converting integer to string");
		}
		else {
			if(mem[0] == '@'){
				if(printf("Error in client input\n") < 0) {
					release(fd1, "c", "s", "a");
					err(2, "Error in printf client input");
				}
				continue;
			}
			
			int64_t estimation = currentBalance;
		       	estimation += clientAction; //in order to avoid overflow or underflow using int64_t
			if(estimation < 0 || estimation > UINT32_MAX) {
				 mem[0] = 'N'; // failure
			}
			else
			{
				currentBalance = estimation;
                                if(lseek(fd1, -ACCOUNT_SIZE, SEEK_CUR) < 0) {
					release(fd1, "c", "s", "a");
					err(2, "Error in lseek 2 main");
				}

                                if(write(fd1, &currentBalance, ACCOUNT_SIZE) < 4){
					release(fd1, "c", "s", "a");
                                        err(1, "Error in writing to current balance");
                                }

                                mem[0] = 'Y'; // success
                             
			}
		}
		
		sem_post(a);
	}
	release(fd1, "c", "s", "a");
	exit(0);
	sem_post(c); // it was needed to declare c in the server, but c was never used, so this is to prevent compile error
}
