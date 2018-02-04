void setup() {
  pinMode(13,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
}

void loop() {
  int i=0,j,k;
  while(1)
  {
  if(i>14)
  i=0;
  j=i;
  k=j&8;
  if(k==8)
  digitalWrite(4,HIGH);
  else 
  digitalWrite(4,LOW);
  k=j&4;
  if(k==4)
  digitalWrite(3,HIGH);
  else
  digitalWrite(3,LOW);
  k=j&2;
  if(k==2)
  digitalWrite(2,HIGH);
  else
  digitalWrite(2,LOW);
  k=j&1;
  if(k==1)
  digitalWrite(13,HIGH);
  else
  digitalWrite(13,LOW);
  i++;
  delay(1000);
  }
}
