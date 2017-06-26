////---------------------------------------------------------------///// 
//  Phím * ngoài để kích NHẬP MẬT KHẨU
//  Phím OK là #
//  phím # ngoài là ĐỔI MẬT KHẨU
//
//
//  MẬT KHẨU ĐẦU TIÊN LÀ 1 2 3
//
/////
#include<Keypad.h>
#include<string.h>
#define SER_PIN   4   //Chan ds
#define SCK_PIN   13    // chan SHCP
#define RCK_PIN   3   // chan STCP  
#define SER_1     digitalWrite(SER_PIN, HIGH)  // DS(Serial Data Input)
#define SER_0     digitalWrite(SER_PIN, LOW)
#define SCK_1     digitalWrite(SCK_PIN, HIGH)  // SH_CP(Shift resister Clock Pin)
#define SCK_0     digitalWrite(SCK_PIN, LOW)
#define RCK_1     digitalWrite(RCK_PIN, HIGH)  // ST_CP(Storage resister Latch Pin)
#define RCK_0     digitalWrite(RCK_PIN, LOW)
#define RS1_EN1   0x05
#define RS1_EN0   0x01
#define RS0_EN1   0x04
#define RS0_EN0   0x00
#define BackLight 0x08
#define delay_ms  delay
#define delay_us  delayMicroseconds
#include <Servo.h>


Servo servo;



const byte rows = 4; //số hàng
const byte columns = 4; //số cột
 
int holdDelay = 700; //Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3; // 
int state = 0; //nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu
char key = 0;
char NhapMatKhau[32], LuuMatKhau[32] = {'1','2','3'};

int i;
bool Status = 0;//Status điều khiển trạng thái trong hàm BanPhim 
        //  và ngăn không cho nhảy liên tục làm thay đổi i
 
//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
 
byte rowPins[rows] = {5, 6, 7, 8}; //Cách nối chân với Arduino
byte columnPins[columns] = {9, 10, 11, 12};
 
//cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);



int tx=1;
int rx=0;
byte state1;

///-----------------------------------------------------------------------------------///
///-----------------------------------------------------------------------------------///  
///------------------------------- MÀN HÌNH -------------------------------------------///


 
// RS-Q0 / RW-Q1 / EN-Q2 / BackLight-Q3 / D4-Q4 / D5-Q5 / D6-Q6 / D7-Q7 
 
void RCK_PULSE() { RCK_1; delay_us(4); RCK_0; }
 
void SCK_PULSE() { SCK_1; delay_us(4); SCK_0; }
 
void Shift_595(char data) {
  for(int i=0; i<8; i++) {
    if(data & (0x80 >> i)) SER_1;  // MSBFIRST
    else SER_0;
    SCK_PULSE();
  }
  RCK_PULSE();
}
 
void LCD_DATA(char data) {
  RCK_0;
  Shift_595((data & 0xF0) | RS1_EN1 | BackLight);  
  Shift_595((data & 0xF0) | RS1_EN0 | BackLight); 
  delay_us(4);
    
  Shift_595(((data<<4) & 0xF0) | RS1_EN1 | BackLight); 
  Shift_595(((data<<4) & 0xF0) | RS1_EN0 | BackLight); 
  delay_us(50);
}
 
void LCD_CMD(char cmd) {
  RCK_0;
  Shift_595((cmd & 0xF0) | RS0_EN1 | BackLight); 
  Shift_595((cmd & 0xF0) | RS0_EN0 | BackLight); 
  delay_us(4);
 
  Shift_595(((cmd<<4) & 0xF0) | RS0_EN1 | BackLight); 
  Shift_595(((cmd<<4) & 0xF0) | RS0_EN0 | BackLight); 
  delay_us(50);
}
 
void LCD_CMD_4bit(char cmd) {
  RCK_0;
  Shift_595((cmd & 0xF0) | RS0_EN1 | BackLight); 
  Shift_595((cmd & 0xF0) | RS0_EN0 | BackLight); 
  delay_us(50);
}
 
void LCD_INIT(void) {
  delay_ms(100); 
  Shift_595(0x00); 
 
  LCD_CMD_4bit(0x30); delay_ms(5); 
  LCD_CMD_4bit(0x30); delay_us(100);
  LCD_CMD_4bit(0x30); delay_us(100);
  LCD_CMD_4bit(0x20); delay_us(100);
  LCD_CMD(0x28);  // 4 bits, 2 line, 5x8 font
  LCD_CMD(0x08);  // display off, cursor off, blink off 
  LCD_CMD(0x01);  // clear display
  delay_ms(3);
  LCD_CMD(0x06);  // cursor movint direction
  LCD_CMD(0x0C);  // display on, cursor off, blink off 
}
 
void LCD_XY(char x, char y) {
  if     (y==0) LCD_CMD(0x80 + x);
  else if(y==1) LCD_CMD(0xC0 + x);
  else if(y==2) LCD_CMD(0x94 + x);
  else if(y==3) LCD_CMD(0xD4 + x);
}
 
void LCD_CLEAR(void) { LCD_CMD(0x01); delay_ms(2);}
 
void LCD_PUTS(char *str) { while(*str) LCD_DATA(*str++); }
              


///----------------------------------------------------------------------------------///
///----------------------------------------------------------------------------------///
///------------------------------- BÀN PHÍM -----------------------------------------///
void BanPhim()   // Hàm bàn phím
{
  char temp = keypad.getKey();
 
  if ((int)keypad.getState() ==  PRESSED) 
  {
    if (temp != 0) 
    {
      key = temp;
      Status = 1;
    }
  }
  if ((int)keypad.getState() ==  HOLD) 
  {
    state++;
    state = constrain(state, 1, n-1);
    delay(holdDelay);
  }
 
  if ((int)keypad.getState() ==  RELEASED) 
  {
    key += state;
    state = 0;
    //Xuất lên Máy tính để xem kết quả
    Serial.print(key);
    Serial.print(" ");
    
  }
  delay(100);
}
//------------------------------------------------------------------------------------//
void NhapMK() // Hàm nhập mật khẩu
{

    i = 0;
    key = 0;
    Status = 0;
    do
    {
    
      if(key != '#')
      {
            //Serial.print("Status: ");
            //Serial.println(Status);
          if(Status == 1)
            {
          NhapMatKhau[i] = key;
                LCD_XY(i, 1); //Xác định đóng và vị trí bắt đầu in//  số đầu là vị trí in, số sau là dòng in
          LCD_PUTS((char *)"*"); //câu lệnh in
          i++;
            }
            
      }
        Status = 0;
      BanPhim();
       // Serial.println(i);
    }while(key != '#');
  key = 0;

}
//---------------------------------------------------------------------------------//
void DoiMK()  // Hàm đổi mật khẩu
{
  char XacMinhMK[32];
  Serial.println("DOI MAT KHAU");
  Serial.println("NHAP MAT KHAU CU:");
  
  LCD_XY(1,0);
  LCD_PUTS((char *) "DOI MAT KHAU:");
  delay(2000);
  LCD_CLEAR();
  LCD_XY(1,0);
  LCD_PUTS((char *) "MAT KHAU CU:");
  NhapMK();
  LCD_CLEAR();
  if(KT(LuuMatKhau))
  {
    Serial.println("MAT KHAU DUNG");
    Serial.print("\nNhap mat khau moi: ");
   
    
    LCD_XY(1,0);
    LCD_PUTS((char *) "MAT KHAU DUNG");
    delay(2000);
    LCD_CLEAR();
    LCD_XY(1,0);
    LCD_PUTS((char *) "MAT KHAU MOI:");
  
    NhapMK();
    LCD_CLEAR();
    for(int j = 0; j < i; j++)
      XacMinhMK[j] = NhapMatKhau[j];
    
    Serial.print("\nNhap xac minh mat khau moi: ");
    
    LCD_XY(1,0);
    LCD_PUTS((char *) "XAC MINH MK:");
    NhapMK();
    LCD_CLEAR();
    if(KT(XacMinhMK))
    {
      Serial.println("MAT KHAU XAC MINH CHINH XAC");
      
      LCD_XY(1,0);
      LCD_PUTS((char *) "MAT KHAU XAC");
      LCD_XY(0,1);
      LCD_PUTS((char *) "MINH CHINH XAC");
      
      
      for(int j = 0; j < i; j++)
        LuuMatKhau[j] = XacMinhMK[j];
    }
    else
    {
      Serial.println("MAT KHAU XAC MINH SAI");
      LCD_XY(1,0);
      LCD_PUTS((char *) "MAT KHAU XAC");
      LCD_XY(0,1);
      LCD_PUTS((char *) "MINH SAI");
    }
  
  
  }
  else
    Serial.println("SAI MAT KHAU");
  delay(1000);
  LCD_CLEAR();
  key = 0;
}


//---------------------------------------------------------------------------------
bool KT(char Luu[]) \\ hàm kiểm tra mật khẩu
{
  for(int j = 0; j < i; j++)
      if(NhapMatKhau[j] != Luu[j])
        return 0;
    return 1;
}
                       
//------------------------------------------------------------------//
//----------------------------- SERVO ------------------------------//

void DKservo() // hàm điều khiển servo
{
  Serial.println(servoPos);  int servoPos = map(value, 0, 1023, 0, 180);
        for(servoPos = 0; servoPos <= 180; servoPos++)
        {
          servo.write(servoPos);
        //  Serial.println(servoPos);
          delay(100);
        }
      
        delay(5000);
        
        for(servoPos; servoPos > 0; servoPos--)
        {
          servo.write(servoPos);
         // Serial.println(servoPos);
          delay(100);
        }
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

void Bluetooth()
{
  if(Serial.available() > 0)
  {
    state1 = Serial.read();
  } 
  else 
    state1 = 0;
 
  Serial.println(255-state1);
  if ((255-state1)==1)
  {
    Serial.println("MAT KHAU DUNG");
    LCD_XY(1,0);
    LCD_PUTS((char *) "DUNG MAT KHAU");
    delay(1000);
    DKservo();
    LCD_CLEAR();
  }
  delay(1000);
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
                       //------------------------//
void setup() 
{
  
  pinMode(SER_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  LCD_INIT();  // Khởi tạo LCD
  servo.attach(2);
  Serial.begin(9600);//bật serial, baudrate 9600

}
void loop() 
{
  
  BanPhim();
  if(key == '*')
  {
    LCD_XY(1,0);
    LCD_PUTS((char *) "NHAP MAT KHAU:");
    NhapMK();
    LCD_CLEAR();
    if(KT(LuuMatKhau) == 0)
    {
        Serial.println("SAI MAT KHAU");
        LCD_XY(1,0);
      LCD_PUTS((char *) "SAI MAT KHAU");
        delay(1000);
        LCD_CLEAR();
    }
    else
    {
        Serial.println("DUNG MAT KHAU");
        LCD_XY(1,0);
        LCD_PUTS((char *) "DUNG MAT KHAU");
        delay(1000);
        LCD_CLEAR();
       
      ///Kích hoạt Servo
       DKservo();
    }
      
  }
  if(key == '#')
    DoiMK();

  Bluetooth();
   
  Serial.println("Ngoai");
}


