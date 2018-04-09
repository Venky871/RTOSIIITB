#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<pthread.h>
#include<time.h>
int fd,fd1;
void* receive()
{
char recvbuf[100];
int len;
while(1)
{
   len = recv(fd1, recvbuf, sizeof(recvbuf),0) ;
   if( len <=0)
   {
       perror("receive");
      exit(4);
   }
  recvbuf[len] = '\0' ;
   printf("%s\n",recvbuf);
  }
}

void* send1()
{
char sendbuf[100];
while(1)
{
printf("enter string\n");
fgets(sendbuf,sizeof(sendbuf),stdin);
if(send(fd1,sendbuf,sizeof(sendbuf),0)<0)
{
	perror("send");
	exit(5);
} 
}
}
int main()

{

static int  server_address_size, client_address_size;
static struct sockaddr_in client, server;
   if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
       exit(1);
   }

   /*
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network
    * interface.)
    */
   server.sin_family      = AF_INET;  /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port      */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */
   
   if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      
       exit(2);
   }
   /* Find out what port was really assigned and print it */
   server_address_size = sizeof(server);
   if (getsockname(fd, (struct sockaddr *) &server, &server_address_size) < 0)
   {  
       exit(3);
   }
   printf("Port assigned is %d\n", ntohs(server.sin_port));
   /*
    * Receive a message on socket s in buf  of maximum size 32
    * from a client. Because the last two paramters
    * are not null, the name of the client will be placed into the
    * client data structure and the size of the client address will
    * be placed into client_address_size.
    */
   if(listen(fd,1)<0)
   {
   perror("listen");
   exit(4);
   }
   client_address_size = sizeof(client);
   if((fd1=accept(fd, (struct sockaddr *) &client,
            &client_address_size)) <0)
   {
       perror("recvfrom");
      exit(4);
   }
   /*
    * Print the message and the name of the client.
    * The domain should be the internet domain (AF_INET).
    * The port is received in network byte order, so we translate it to
    * host byte order before printing it.
    * The internet address is received as 32 bits in network byte order
    * so we use a utility that converts it to a string printed in
    * dotted decimal format for readability.
    */
   printf("Received message   from domain %s port %d internet\
 address %s\n",
       
       (client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
       ntohs(client.sin_port),

       inet_ntoa(client.sin_addr));
client_address_size=sizeof(client);
static  int len=0;   
pthread_t p1,p2,p3;
pthread_create(&p1,NULL,send1,NULL);
pthread_create(&p2,NULL,receive,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
return 0;
}



