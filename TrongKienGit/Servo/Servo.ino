#include <Servo.h> 
Servo myservo;  
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 9, 8, 7, 6);     
 
int bientro = A0;      
int servoPin = 5;       
int nut1 = 2, nut2 = 3, nut3 = 4;
 
void setup ()
{
    myservo.attach(servoPin); 
    lcd.begin(16, 2);
    pinMode(nut1, INPUT);
    pinMode(nut2, INPUT);
    pinMode(nut3, INPUT);
}
 
void loop ()
{
 
  int stt1 = digitalRead(nut1);
  int stt2 = digitalRead(nut2);
  int stt3 = digitalRead(nut3);

  if(stt1 == 1)
  {
   lcd.clear();
   myservo.write(0);
   lcd.print("0 Do");

  }
  if(stt2 == 1)
  {
    lcd.clear();
    myservo.write(90);
    lcd.print("90 Do");
  }
   if(stt3 == 1)
   {
     lcd.clear();
    myservo.write(180);
    lcd.print("180 Do");
   }

/*
  int value = analogRead(bientro);
    // Chuyển giá trị analog (0-1023) đọc được từ biến trở sang số đo độ (0-180độ)
    // dùng để điều khiển góc quay cho servo
  int  servoPos = map(value, 0, 1023, 0, 180);
    // Cho servo quay một góc là servoPos độ
  myservo.write(servoPos);
  lcd.setCursor(0, 0);
  lcd.print("Gia tri goc: ");
  lcd.setCursor(0, 1);
  lcd.print(servoPos);
  delay(100);
  */
}
