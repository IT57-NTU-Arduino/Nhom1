/*Đèn tín hiệu giao thông*/
void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}
int x =400;
void loop() {
  tr();
  denvang();
  digitalWrite(11, HIGH);
  delay(x);
  digitalWrite(11, LOW);
}
void dendo(){
  digitalWrite(8, HIGH);
  delay(x);
  digitalWrite(8, LOW);
  }

void denvang(){
  digitalWrite(9, HIGH);
  delay(x);
  digitalWrite(9, LOW);
  }
  
void denxanh(){
  digitalWrite(10, HIGH);
  delay(x);
  digitalWrite(10, LOW);
  }
 void tr(){
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  delay(x);
  digitalWrite(8, LOW);
  digitalWrite(10, LOW);
 }

