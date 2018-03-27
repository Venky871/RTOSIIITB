#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
 
#define SERVER 1L
 struct my_msgbuf {
    long    mtype;
    int    mtext[200];
} buf;
int main()
{
key_t key;
int msgqid;
printf("enter number for operation addition=1,subtraction=2,multiplication=3,division=4\n");
scanf("%d",&buf.mtext[0]);
printf("enter two numbers \n");                                     //numbers for operation
scanf("%d\n%d\n",&buf.mtext[1],&buf.mtext[2]);  
if((key=ftok("/home/venkatesh/RTOS/RTOSIIITB/pipe.c",'1'))==-1)                // generating same key of server
{
    perror("ftok");
    exit(1);
}
buf.mtype=1;                                               
msgqid = msgget(key, 0666 | IPC_CREAT );                                //server msgqueue identifier
 if(msgsnd(msgqid, &buf, sizeof(buf.mtext)+1, IPC_NOWAIT)==-1)          // message send to queue
 {
        perror("msgsnd");                                                         
        exit(-1);                                          
 }                                                           
 if(msgrcv(msgqid, &buf, sizeof(buf.mtext)+1, SERVER, 0)<0)             // message received from server
 {
        perror("msgrcv");
        exit(-1);
 }
if(buf.mtext[1]==0)
{
  printf("answer\n");
    printf("%d\n", buf.mtext[0]);
}
else if(buf.mtext[1]==1)
printf("invalid expression");
}









