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
printf("enter two numbers for multiplication\n");  //numbers for multiplication
scanf("%d\n%d\n",&buf.mtext[0],&buf.mtext[1]);  
if((key=ftok("pipe.c",'1'))==-1)                // generating same key of server
{
    perror("ftok");
    exit(1);
}
buf.mtype=1;                                               
msgqid = msgget(key, 0666 | IPC_CREAT );                      //server msgqueue identifier
 if(msgsnd(msgqid, &buf, sizeof(buf.mtext)+1, IPC_NOWAIT)==-1)   // message send to queue
 {
        perror("msgsnd");                                                         
        exit(-1);                                          
 }                                                           
 if(msgrcv(msgqid, &buf, sizeof(buf.mtext)+1, SERVER, 0)<0)  // message received from server
 {
        perror("msgrcv");
        exit(-1);
 }
  printf("Message after conversion\n");
    printf("%d\n", buf.mtext[0]);
}
