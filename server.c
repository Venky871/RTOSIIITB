#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
# define SERVER 1L
typedef struct  
{
	long mtype;
	int mtext[200];      // data structure for receiving and trasmission
}MESSAGE;
int main()
{
	int msqid;
	MESSAGE  buf;
	key_t key;
	if((key=ftok("pipe.c",'1'))==-1)
	{
		perror("ftok");                       // creating key for message queue using file                                                          name
		                                      // projections
		exit(1);                                  
	}
	if ((msqid = msgget(key, 0666 | IPC_CREAT )) == -1)
	{                                                   // creating message queue using key  
		perror("msgget");
		exit(1);
	}
	if(msgrcv(msqid, &buf, sizeof(buf.mtext)+1, SERVER, 0)<0)
	{
		perror("msgrcv");                          //  reciving message from client 
		exit(-1);
	}
	printf("SERVER receives:\n");
	printf("%d\n%d", buf.mtext[0],buf.mtext[1]);
	int a=buf.mtext[0];
	int b=buf.mtext[1];                              // multiplying numbers
	a*=b;
	buf.mtype=1;

	buf.mtext[0]=a;
	if(msgsnd(msqid, &buf, sizeof(buf.mtext)+1, IPC_NOWAIT)==-1)
	{                                                 
		perror("msgsnd");                       // sending result back to client
		exit(-1);
	}
	return 0;
}

