#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include<stdlib.h>
int g_first,g_second,g_third;
void printvariable(int j)
{
int k,l;
printf("\nj is =%d",j);
printf("\n address of j =%p",(void*)&j);
printf("\n address of k =%p",(void*)&k);
printf(" \naddress of l =%p",(void*)&l);
printf("\n address of g_first=%p",(void*)&g_first);
printf("\n address of g_second =%p",(void*)&g_second);
printf("\n address of g_third =%p",(void*)&g_third);
}
int main()
{
int a,b,c;
printf("\n address of g_first=%p",(void*)&g_first);
printf("\n address of g_second=%p",(void*)&g_second);
printf(" \naddress of g_third=%p",(void*)&g_third);
printf("\n address of a =%p",(void*)&a);
printf("\n address of b =%p",(void*)&b);
printf("\n address of c =%p",(void*)&c);
	int pid_p1=fork();
	if(pid_p1!=-1)
	{
	if(pid_p1==0)
	{
          int j=1;
         printf("\nglobal variable in child1=%d",g_first);
         g_first=1;
      printf("\nchild1 global =%d",g_first);
	printvariable(j);
	while(1)
	{
	}
	}
	else
	{
	int pid_p2=fork(); 
	if(pid_p2!=-1)
	{
	if(pid_p2==0)
	{
          int j=2;
           printf("\nglobal variable in child2=%d",g_first);
         g_first=2;
      printf("\nchild1 global =%d",g_first);

	printvariable(j);
	while(1)
	{
	}
	}
	else
	{

	 int pid_p3=fork();
	if(pid_p3!=-1)
	{
	if(pid_p3==0)
	{
         int j=3;
         printf("\nglobal variable in child3=%d",g_first);
         g_first=3;
         printf("\nchild3 global =%d",g_first);
	printvariable(j);
	while(1)
	{
	}
	}
	else
	{
	
	                                                 FILE *fp = fopen("killfile.sh","w+");
                             	                         fprintf(fp,"%s\n","#/bin/bash");
						     	 fprintf(fp,"kill -9 %d\n",pid_p1);
				            	         fprintf(fp,"kill -9 %d\n",pid_p2);
			                	         fprintf(fp,"kill -9 %d\n",pid_p3);
			                   	         fprintf(fp,"kill -9 %d\n",getpid());
                                                         fputs("rm kill.sh",fp);
                                                         system("chmod +x kill.sh");
                             	                         fclose(fp);
while(1)
{
} 

	}
	}
	}
	}
	}
	}
	return 0;
	}























