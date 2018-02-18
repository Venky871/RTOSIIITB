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
char mtext[200];
}MESSAGE;
int main()
{
int msqid;
MESSAGE  buf;
key_t key;
if((key=ftok("kirk.c ",'1'))==-1)
{
    perror("ftok");
    exit(1);
}
if ((msqid = msgget(key, IPC_EXCL | IPC_CREAT | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH )) == -1)
{
perror("msgget");
exit(1);
}
 if(msgrcv(msqid, &buf, sizeof(buf.mtext), SERVER, 0)<0)
    {
        perror("msgrcv");
        exit(-1);
    }
 printf("SERVER receives:\n");
 printf("%s\n", buf.mtext);
//int a=atoi(&buf.mtext[0]);
//int b=atoi(&buf.mtext[1]);
buf.mtype=1;

buf.mtext[0]='h';
 if(msgsnd(msqid, &buf, strlen(buf.mtext)+1, IPC_NOWAIT)==-1)
    {
        perror("msgsnd");
        exit(-1);
    }
return 0;
}
