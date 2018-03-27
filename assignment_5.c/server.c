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
static int error=0; 
int mathematical_engine(int buf[])
{
int eq_var1,eq_var2;
int operation;
eq_var1=buf[1];                              // code for mathematical engine.
eq_var2=buf[2];
operation=buf[0];                            // does addition , multiplication , subtraction and division
if(operation==1)
return eq_var1+eq_var2;
if(operation==2)
return eq_var1-eq_var2;                     
if(operation==3)
return eq_var1*eq_var2;                    // sends  in case of invalid operations
if((operation==4)&(eq_var2!=0))
{
return eq_var1/eq_var2;
}
printf("not valid operation");
error=1;
return  0; 
}
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
	if((key=ftok("/home/venkatesh/RTOS/RTOSIIITB/pipe.c",'1'))==-1)
	{
		perror("ftok");                       // creating key for message queue using   file                                                          name
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
        int answer=mathematical_engine(buf.mtext);
	printf("%d\n%d", buf.mtext[1],buf.mtext[2]);      // calling mathematical equation  
        if(error==0)                                       
      {
	printf("\n%d",answer);                           // sending only valid answer 
	buf.mtype=1;
	buf.mtext[0]=answer;
        buf.mtext[1]=error;
        }
        else 
        buf.mtext[1]=error;
	if(msgsnd(msqid, &buf, sizeof(buf.mtext)+1, IPC_NOWAIT)==-1)
	{                                                 
		perror("msgsnd");                       // sending result back to client
		exit(-1);
	}
	return 0;
}

