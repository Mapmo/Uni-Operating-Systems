PROG_SERVER=	server
SRCS_SERVER=	server.c
OBJS_SERVER=	server.o

PROG_CLIENT=	client
SRCS_CLIENT=	client.c
OBJS_CLIENT=	client.o

CPPFLAGS_STD?=	-D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700
CPPFLAGS+=	${CPPFLAGS_STD}

CFLAGS_OPT?=	-O2 -g -pipe
CFLAGS_STD?=	-std=c99
CFLAGS_WARN?=	-Wall -W -Wextra -Werror -Wpedantic

CFLAGS?=	${CFLAGS_OPT}
CFLAGS+=	${CFLAGS_STD} ${CFLAGS_WARN}

PTHREAD= 	-pthread #used for semaphores

all:		${PROG_SERVER} ${PROG_CLIENT}

${PROG_SERVER}:	${OBJS_SERVER}
		${CC} ${LDFLAGS} ${PTHREAD} -o ${PROG_SERVER} ${OBJS_SERVER}

${PROG_CLIENT}:	${OBJS_CLIENT}
		${CC} ${LDFLAGS} ${PTHREAD} -o ${PROG_CLIENT} ${OBJS_CLIENT}

client.o:	client.c comm.h

server.o:	server.c comm.h

clean:
		rm -f ${PROG_SERVER} ${OBJS_SERVER} ${PROG_CLIENT} ${OBJS_CLIENT}
