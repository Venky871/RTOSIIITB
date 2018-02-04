#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<string.h>
int counter=0;
void myInterrupt(int a, int b,int c,int d)
{
counter=d*8+c*4+b*2+a*1;
printf("\n%d",counter);
}
int main(void)
{
wiringPiSetup();
pinMode(0,INPUT);
pinMode(2,INPUT);
pinMode(3,INPUT);
pinMode(4,INPUT);
int a=0,b=0,c=0,d=0;
while(1)
{
if(((a^digitalRead(0))|(b^digitalRead(2))|(c^digitalRead(3))|(d^digitalRead(4)))==1)
{
	a=digitalRead(0);
	b=digitalRead(2);
	c=digitalRead(3);
	d=digitalRead(4);
	myInterrupt(a,b,c,d);
}
delay(500);
}
return 0;
}
