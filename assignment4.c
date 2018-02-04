#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include<unistd.h>
int a,b;
int c;
sem_t len1,len2,len3;
void * thread1proc()
{
int i,j,k;
printf("\n address of i in t1=%p",&i);
printf("\n address of j in t1=%p",(void*)&j);
printf(" \naddress of k in t1=%p",(void*)&k);
printf("\n address of a in t1=%p",(void*)&a);
printf("\n address of b in t1=%p",(void*)&b);
printf("\n address of c in t1=%p",(void*)&c);
while(1)
{
sem_wait(&len3);
printf("\na in t1=%d",a);
printf("\nb in t1=%d",b);
printf("\nc in t1=%d",c);
printf("\ni in t1=%d",i);
printf("\nj in t1=%d",j);
printf("\nk in t1=%d",k);
printf("\nenter global values in t1\n");
scanf("%d %d %d",&a,&b,&c);
printf("\nenter local variables values in t1\n");
scanf("%d %d %d",&i,&j,&k);
sem_post(&len1);
}

}
void * thread2proc()
{
int i,j,k;
printf("\n address of i in t2=%p",(void*)&i);
printf("\n address of j in t2=%p",(void*)&j);
printf(" \naddress of k in t2=%p",(void*)&k);
printf("\n address of a in t2=%p",(void*)&a);
printf("\n address of b in t2=%p",(void*)&b);
printf("\n address of c in t2=%p",(void*)&c);
while(1)
{
sem_wait(&len1);
printf("\na in t2=%d",a);
printf("\nb in t2=%d",b);
printf("\nc in t2=%d",c);
printf("\ni in t2=%d",i);
printf("\nj in t2=%d",j);
printf("\nk in t2=%d",k);
printf("\nenter global values in t2\n");
scanf("\n%d %d %d",&a,&b,&c);
printf("\nenter local variables values in t2\n");
scanf("%d %d %d",&i,&j,&k);
sem_post(&len2);
}
}
void * thread3proc()
{
int i,j,k;
printf("\n address of i in t3=%p",(void*)&i);
printf("\n address of j in t3=%p",(void*)&j);
printf(" \naddress of k in t3=%p",(void*)&k);
printf("\n address of a in t3=%p",(void*)&a);
printf("\n address of b in t3=%p",(void*)&b);
printf("\n address of c in t3=%p",(void*)&c);
sem_post(&len3);
while(1)
{
sem_wait(&len2);
printf("\na in t3=%d",a);
printf("\nb in t3=%d",b);
printf("\nc in t3=%d",c);
printf("\ni in t3=%d",i);
printf("\nj in t3=%d",j);
printf("\nk in t3=%d",k);
printf("\nenter global values in t3\n");
scanf("\n%d %d %d",&a,&b,&c);
printf("\nenter local variables values in t3\n");
scanf("\n%d %d %d",&i,&j,&k);
sem_post(&len3);
}
}
int main()
{int i,j,k;
printf("\na in main=%d",a);
printf("\nb in main=%d",b);
printf("\nc in main=%d",c);
printf("\n 1address of i in main=%p",(void*)&i);
printf("\n 1address of j in main=%p",(void*)&j);
printf(" \n1address of k in main=%p",(void*)&k);
pthread_t p1,p2,p3;
pthread_create(&p1,NULL,thread1proc,NULL);
pthread_create(&p2,NULL,thread2proc,NULL);
pthread_create(&p3,NULL,thread3proc,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
pthread_join(p3,NULL);
}
