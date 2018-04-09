#include <unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<error.h>
#include<sys/socket.h>
#include <pthread.h>
static int fd;
void * receive()
{
int len;
char recvbuf[100];
while(1)
{
 len=recv(fd,recvbuf,sizeof(recvbuf),0);
		if(len<0)
		{
			perror("recive");
			exit(6);
		}  
                recvbuf[len]='\0';
		printf("%s",recvbuf);   
 
}
}
void * send1()
{
char sendbuf[100];
while(1)
{
printf("enter string\n");
fgets(sendbuf,sizeof(sendbuf),stdin);
if(send(fd,sendbuf,sizeof(sendbuf),0)<0)
{
	perror("send");
	exit(5);
} 
}
}
void  main(argc, argv)
int argc;
char **argv;
{
static int server_address_size;
static int n;
int i;
unsigned short port;
struct sockaddr_in server;
   /* argv[1] is internet address of server argv[2] is port of server.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
   if(argc != 3)
   {
      printf("Usage: %s <host address> <port> \n",argv[0]);
      exit(1);
   }
   port = htons(atoi(argv[2]));
   if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
       perror("socket");
       exit(2);
   }

   /* Set up the server name */
   server.sin_family      = AF_INET;            /* Internet Domain    */
   server.sin_port        = port;               /* Server Port        */
   server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */
 if(connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf(" non `Connect()");
        exit(3);
    }

pthread_t p1,p2,p3;
pthread_create(&p1,NULL,send1,NULL);
pthread_create(&p2,NULL,receive,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
}
