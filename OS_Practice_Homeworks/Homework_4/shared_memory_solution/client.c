#include "comm.h"

void validateAccount(const char account)
{
	if(account < 'A' || account > 'H') {
		errx(1, "Error! Account is invalid!");
	}
}

void release(sem_t* c, sem_t* s, sem_t* a, char* mem)
{
	sem_close(a);
	sem_post(c);
        sem_close(c);
	sem_close(s);
	disconnectString(mem);
}

char convertAccountToInt(const char account)
{
	return account - 'A';
}

char validateInput(char* mem)
{
	int len = strlen(mem);
	if(len < 1)
	{
		return 0;
	}
	
	int i;
	if(len == 1)
	{
		i = 0;
	}
	else
	{
		i = 1;
               	if((mem[0] < '0' || mem[0] > '9') && mem[0] != '-' && mem[0] != '+' ){
			return 0;
		}
	}

	while(i < len){
		if(mem[i] < '0' || mem[i] > '9')
		{
			return 0;
		}
		i++;
	}
	return 1;
}

int main(const int argc, const char* argv[])
{
	if(argc != 2) {
                errx(1, "Wrong number of arguments! Usage: ./client ACCOUNT");
        }


	const char currentAccount = argv[1][0];
	validateAccount(currentAccount);

	sem_t* s = sem_open("s", O_CREAT, 0700, 0),* c = sem_open("c", O_CREAT, 0700, 1),* a = sem_open("a", O_CREAT, 0700, 0);

	char* mem = connectString();

	sem_wait(c);
	
	mem[0] = convertAccountToInt(currentAccount);

	sem_post(s);
	sem_wait(a);

	if(mem[0] == '@') {
		sem_post(s);
		release(c, s, a, mem);
		errx(1, "Error! Account ID is invalid!");
	}

	if (strlen(mem) < 1) {
		sem_post(s);
		release(c, s, a, mem);
		err(1, "Error in receiving account data");
	}
        
        if(printf("%s\n", mem) < 0) {
		sem_post(s);
                release(c, s, a, mem);
		err(2, "Error in printf current balance is");
	}

       	if(scanf("%s", mem) < 0)
	{
		sem_post(s);
		release(c, s, a, mem);
		err(2, "Error reading client input");
	}

	if(validateInput(mem) == 0)
	{
		mem[0] = '@';
		sem_post(s);
		release(c, s, a, mem);
		errx(3, "Bad input");
	}
	sem_post(s);
	sem_wait(a);
 
        if(mem[0] != 'Y') {
		release(c, s, a, mem);
		exit(1);
	}
	
    	release(c, s, a, mem);
    	exit(0);
}
