#define sensor 11
#define buzzer 9
#define led 10
void setup()
{
  pinMode(sensor,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  int s = digitalRead(sensor);
  Serial.println("Gia tri cam bien: " + s);
  if (s == 1)
  {
    
      buzz(1000);
      delay(100);
      buzz(900);
      delay(100);
      buzz(1000);
        
  }
  else digitalWrite(led, 0);
  delay(100);
  
}
void buzz(int n)
{
      tone(buzzer, n, 500);
      digitalWrite(led,HIGH);
      delay(600);
      noTone(buzzer);
      digitalWrite(led,0);

}


