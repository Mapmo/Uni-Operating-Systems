#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>

//TO DO nomerirane na typite error codes
const char ACCOUNTS = 8;
const char* SOCK_PATH = "server_socket";
const char* MUTEX = "mutex";

void ValidateArgc(const int argc, const char* argv[])
{
        if(argc != 2)
        {
                errx(1, "Wrong ammount of arguments in: %s", argv[0]);
        }
}

int OpenFile(const char* arg)
{
	int fd = open(arg, O_RDWR | O_CREAT, 0666);

        if(fd == -1)
        {
                err(6, "Problem opening %s", arg);
        }

        int seeker = lseek(fd, 0, SEEK_END);

	if(seeker == -1)
	{
		err(6, "Problem seeking the end in %s", arg);
	}
	else if(seeker == 0)
        {
		uint32_t zero = 0;
	       	for(int i = 0; i < ACCOUNTS; i++)
		{
			if(write(fd, &zero, 4) == -1)
			{
				err(6, "Problem while writing in the new file");
			}
		}
        }

	if(lseek(fd, 0, SEEK_SET) == -1)
	{
		err(6, "Problem returning to beginning in %s", arg);
	}	

	return fd;
}

int OpenSocket(const int * fd)
{
	int sock = socket(AF_UNIX, SOCK_STREAM, 0); //moje AF_LOCAL vmesto AF_UNIX
	
	if(sock == -1)
	{
		const int _errno = errno;
		close(*fd);
		errno = _errno;
		err(2, "Problem opening socket");
	}

	return sock;
}

void SetSocket(const int * sock, struct sockaddr_un * local, int * len, int * fd)
{
	(*local).sun_family = AF_UNIX;
        strcpy((*local).sun_path, SOCK_PATH);
        unlink((*local).sun_path);

        (*len) = strlen((*local).sun_path) + sizeof((*local).sun_family);
 
 	if(bind(*sock, (struct sockaddr *)& *local, *len) == -1)
        {
		const int _errno = errno;
		close(*fd);
		close(*sock);
		errno = _errno;
 		err(2, "Problem bounding");
        }
	
	if(listen(*sock, 1) == -1)
	{
		const int _errno = errno;
		close(*sock);
		close(*fd);
		errno = _errno;
		err(2, "Problem listening");
	}
}

sem_t * SetMutex(const int* fd, const int* sock)
{
	sem_t* mutex = sem_open(MUTEX, O_CREAT| O_EXCL, 0666, 1);
        if(mutex == SEM_FAILED) //in case the mutex was left unlinked
        {
                if(sem_unlink(MUTEX) == -1)
		{
			const int _errno = errno;
			close(*fd);
			close(*sock);
			errno = _errno;
			err(7, "Can't unlink already opened mutex");
		}
                printf("Mutex recreated!\n");
                mutex = sem_open("mutex", O_CREAT| O_EXCL, 0666, 1);
        }
	return mutex;
}

void Release(const int* fd, const int* sock, const int* sock2)
{
	const int _errno = errno;
	close(*fd);
	close(*sock);
	close(*sock2);
	sem_unlink(MUTEX);
	errno = _errno;
}

void PrintWaitingForClient(const int fd, const int sock, const int sock2)
{
     	if(printf("Waiting for a client...\n") < 0)
     	{
        	Release(&fd, &sock, &sock2);
                err(3, "Problem whith printf");
	}
}

void AcceptClient(const int* fd, const int* sock, int* sock2, struct sockaddr_un* remote, int* t)
{
	if ((*sock2 = accept(*sock, (struct sockaddr*) remote, (socklen_t *)t)) == -1)
	{
        	Release(fd, sock, sock2);
                err(2, "error while accepting client");
        }
}

void PrintClientConnected(const int fd, const int sock, const int sock2)
{
    	if(printf("A Client Connected To The Bank.\n\n") < 0)
	{
                Release(&fd, &sock, &sock2);
                err(3, "Problem whith printf");
        }
}

char ProcessClientId(const int fd, const int sock, const int sock2, char* str, char* accountNumber)
{
	if(recv(sock2, str, 1, 0) != 1)
        {
                Release(&fd, &sock, &sock2);
                err(5, "error while receiving acc from client");
        }

        *accountNumber = str[0] - 'A';
        if(*accountNumber < 0 || *accountNumber > 7)
        {
                str[0] = 0;
                send(sock2, str, 1, 0);
        	return 0;
        }
        else
        {
                str[0] = 1;
                send(sock2, str, 1, 0);
		return 1;
	}
}

void SendClientCurrentBalance(const int fd, const int sock, const int sock2, const char accountNumber, char* str, uint32_t* parser1)
{
                if(lseek(fd, 4 * accountNumber, SEEK_SET) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(6, "Problem while seeking account data");
                }
                if(read(fd, parser1, 4) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(6, "error while reading the account data");
                }
                if(send(sock2, "Current Balance of Account#", 50, 0) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(4, "error sending balance to client");
                }
		str[0] = accountNumber + 'A';
                if(send(sock2, str, 1, 0) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(4, "error sending id to client");
                }
                if(send(sock2, " is ", 5, 0) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(4, "error sending 'is' to client");
                }
                if(sprintf(str, "%d", *parser1) < 0)
                {
                        Release(&fd, &sock, &sock2);
                        err(3, "error converting data");
                }
                if(send(sock2, str, 50, 0) == -1)
                {
                        Release(&fd, &sock, &sock2);
                        err(4, "error sending balance to client");
                }

}

void ProcessClientInput(const int fd, const int sock, const int sock2, const char accountNumber, char* str, uint32_t parser1, int t)
{
	int16_t parser2;
	if ((t=recv(sock2, str, 50, 0)) < 0)
        {
		Release(&fd, &sock, &sock2);
                err(5, "error receiving input from client");
        }
        str[t] = '\0';

        if(sscanf(str, "%hd", &parser2) == EOF)
        {
                Release(&fd, &sock, &sock2);
                err(3, "error while converting input to short");
        }
        else
        {
		int64_t calc = parser1;
                calc+= parser2;

                if(calc >= 0 && calc <= UINT32_MAX)
                {
                	parser1 += parser2;
                        if(lseek(fd, 4 * accountNumber, SEEK_SET) == -1)
                        {
                                Release(&fd, &sock, &sock2);
                                err(6, "error while seeking to update balance");
                        }
                        if(write(fd, &parser1, 4) != 4)
                        {
                                Release(&fd, &sock, &sock2);
                                err(6, "Error while reading");
                        }
			if(write(fd, &parser1, 4) != 4)
                        {
                                Release(&fd, &sock, &sock2);
                                err(6, "Error while reading");
                        }
                        str[0] = 1;
            	}
                else
              	{
                	str[0] = 0;
                }
     	}

        if (send(sock2, str, 1, 0) == -1)
       	{
		Release(&fd, &sock, &sock2);
                err(4, "error while sending to client");
	}
}

int main(const int argc, const char* argv[])
{
	ValidateArgc(argc, argv);
	
	int fd = OpenFile(argv[1]), t;
	int sock = OpenSocket(&fd), sock2, len;
	struct sockaddr_un local, remote;
	char accountNumber, str[100];
	uint32_t parser1;

	SetSocket(&sock, &local, &len, &fd);

	SetMutex(&fd, &sock);
	
	while(1)
	{      	
		PrintWaitingForClient(fd, sock, sock2);

		t = sizeof(remote);

        	AcceptClient(&fd, &sock, &sock2, &remote, &t);

        	PrintClientConnected(fd, sock, sock2);

		if(ProcessClientId(fd, sock, sock2, str, &accountNumber) == 0)
		{
			continue;
		}

		SendClientCurrentBalance(fd, sock, sock2, accountNumber, str, &parser1);

		ProcessClientInput(fd, sock, sock2, accountNumber, str, parser1, t);

        	close(sock2);
	}

	sem_unlink(MUTEX);	
	close(sock);
	close(fd);
	exit(0);
}
