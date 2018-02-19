#include <unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<error.h>
 void  main(argc, argv)
int argc;
char **argv;
{
int server_address_size;
int fd,n;
int i;
unsigned short port;
pid_t pid;
struct sockaddr_in server;
char *sendbuf,*recvbuf;
sendbuf=(char *)malloc(sizeof(char)*100);
recvbuf=(char *)malloc(sizeof(char)*100);
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

 if((pid=fork())==-1)
 {
perror("fork");
exit(4);
 } 

 if(pid>0)
{ 
 while(1) 
 {   
 printf("enter string\n");
 scanf("%s",sendbuf);
if(send(fd,sendbuf,sizeof(sendbuf),0)<0)
{
	perror("send");
	exit(5);
} 
}
} 
else
{

printf("hello");
	while(1)
	{    
		if(recvfrom(fd,recvbuf,sizeof(recvbuf),0,NULL,NULL)<0)
		{
			perror("recive");
			exit(6);
		}  
		printf("%s",recvbuf);
	}


}
}
