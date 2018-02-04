#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<string.h>
int counter=0;
void myInterrupt(void)
{
int a,b,c,d;
a=digitalRead(2);
b=digitalRead(3);
c=digitalRead(4);
d=digitalRead(5);
counter=a*1+b*2+c*4+d*8;
printf("\n%d",counter);
}
int main(void)
{
wiringPiSetup();
waitForInterrupt(0,-1);
wiringPiISR(0,INT_EDGE_RISING,&myInterrupt);
while(1)
{
}
return 0;
}
