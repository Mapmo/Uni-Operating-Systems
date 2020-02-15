#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>

#define SOCK_PATH "server_socket"
sem_t* mutex;
const char* MUTEX = "mutex";
enum EXIT_STATUS{SUCCESS, ERR_ARGS, ERR_SOCKET, ERR_PRINTF, ERR_SEND, ERR_RECV, ERR_FILE, ERR_SEM};

void ValidateArgc(const int argc, const char* argv[])
{
        if(argc != 2)
        {
                errx(ERR_ARGS, "Wrong ammount of arguments in: %s", argv[0]);
        }
}

void Release(const int s)
{
	const int _errno = errno;
	sem_post(mutex);
        sem_close(mutex);
        close(s);
	errno = _errno;
}

void SetSocket(int* s)
{
        if ((*s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        {
                err(ERR_SOCKET, "error creating socket");
        }

        if(printf("Trying to connect...\n") < 0)
        {
                const int _errno = errno;
                close(*s);
                errno = _errno;
                err(ERR_PRINTF, "error printing");
        }
}

void SetMutex(const int s)
{
	if((mutex = sem_open(MUTEX, O_CREAT, 0666, 1)) == SEM_FAILED)
	{
		Release(s);
		err(ERR_SEM, "error opening the semaphore");
	}

        sem_wait(mutex);
}

void ConnectToBank(const int s, struct sockaddr_un* remote)
{
  	remote -> sun_family = AF_UNIX;
        strcpy(remote -> sun_path, SOCK_PATH);
        
	int len = strlen(remote -> sun_path) + sizeof(remote -> sun_family);
        if (connect(s, (struct sockaddr *) remote, len) == -1)
        {
                Release(s);
                err(ERR_SOCKET, "Can't connect to server");
        }

        if(printf("Connected.\n") < 0) 
        {
                Release(s);
                err(ERR_PRINTF, "error printing 'connected'");
        }
}

void ProcessDataFromBank(const int s, const char ACCOUNT, char* str)
{
	int t;
	
	if(send(s, &ACCOUNT, 1, 0) == -1)
        {
                Release(s);
                err(ERR_SEND, "error sending data");
        }

        if ((t=recv(s, str, 1, 0)) < 0)
        {
               	Release(s);
                err(ERR_RECV, "error receiving acc#");
        }

        if(str[0] == 0)
        {
                Release(s);
                errx(1, "Invalid ID");
        }

        if ((t=recv(s, str, 50, 0)) < 0)
        {
                Release(s);
                err(ERR_RECV, "error receiving prior acc#");
        }

        if(printf("%s", str) == -1)
	{
		Release(s);
		err(ERR_PRINTF, "error printing 'connected'");
        }

        str[t] = '\0';

        if ((t=recv(s, str, 1, 0)) < 0)
        {
                Release(s);
                err(ERR_RECV, "error receiving acc#");
        }

        str[t] = '\0';
        if(printf("%s", str) == -1)
	{
                Release(s);
                err(ERR_PRINTF, "error printing 'connected'");
        }


        if ((t=recv(s, str, 5, 0)) < 0)
        {
                Release(s);
                err(ERR_RECV, "error receiving after acc#");
        }
        str[t] = '\0';

        if(printf("%s", str) == -1)
	{
                Release(s);
                err(ERR_PRINTF, "error printing 'connected'");
        }


        if ((t=recv(s, str, 50, 0)) < 0)
        {
                Release(s);
                err(ERR_RECV, "error receiving balance");
        }
        str[t] = '\0';
        
	if(printf("%s\n", str) == -1)
	{
                Release(s);
                err(ERR_PRINTF, "error printing 'connected'");
        }

}

void SendRequest(const int s, char* str)
{
 	if(printf(">What should we do next?: ") && fgets(str, 100, stdin) == NULL)
	{
		Release(s);
		err(ERR_PRINTF, "error printing 'connected'");
	}

        if (send(s, str, strlen(str), 0) == -1)
        {
                Release(s);
                err(ERR_SEND, "error while sending input to server");
        }

        if (recv(s, str, 1, 0) <= 0)
        {
                Release(s);
                err(ERR_RECV, "error receiving transaction status");
        }
}

void Exit(const int s, const char status)
{
        Release(s);
        
	if(status == 1)
        {
                printf("User completed transaction successfully\n");
                exit(SUCCESS);
        }
        else
        {
                errx(ERR_FILE, "Transaction failed\n");
        }
}

int main(const int argc, const char* argv[])
{
	ValidateArgc(argc, argv);

	const char ACCOUNT = argv[1][0];
    	int s;
    	struct sockaddr_un remote;
    	char str[100];
	
	SetSocket(&s);

	SetMutex(s);

    	ConnectToBank(s, &remote); 
	ProcessDataFromBank(s, ACCOUNT, str);

    	SendRequest(s, str);

	Exit(s, str[0]); 	
}
