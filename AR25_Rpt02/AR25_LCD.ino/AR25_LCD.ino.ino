// I2C 통신 라이브러리 설정
#include <Wire.h>
// I2C LCD 라리브러리 설정
#include <LiquidCrystal_I2C.h>

int blinkNumber=0;

// LCD I2C address 설정 PCF8574:0x27, PCF8574A:0x3F
LiquidCrystal_I2C lcd(0x27,16,2);  // LCD address:0x27, 16X2 LCD, 0x3F

void setup()
{
  // 9600 bps로 시리얼 통신 설정
  Serial.begin(9600);
  lcd.init(); // LCD 설정
  lcd.clear();      // LCD를 모두 지운다.
  lcd.backlight();  // 백라이트를 켠다.  
  // Arduino LCD, Welcome 표시  
  lcd.setCursor(0,0);
  lcd.print("Arduino LCD");
  delay(3000);
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  delay(250);  
lcd.noBacklight();
  delay(250);
  lcd.backlight();
  delay(250);

}

void loop()
{
  // 시리얼 통신 수신 값이 있을 때
  if (Serial.available()) {  
    delay(100);
    // 모두 삭제
    lcd.clear();
     // 커서를 좌측 상단으로
    lcd.setCursor(0,0);
    // "Message from PC" 출력
    lcd.print("Insert to Number");
    // 커서를 두 번째 줄로
    lcd.setCursor(0,1);
    

    // LCD에 PC에서 전송된 데이터를 출력
    while (Serial.available()>0) {
      char val = Serial.read();
      blinkNumber = (val - '0');
      if(isDigit(val))
      {
        lcd.print("Enter : ");
        lcd.print(val);
        for(int i = 0; i<blinkNumber; i++)
        {
          lcd.noBacklight();
          delay(100);
          lcd.backlight();
          delay(100);
        }
        delay(5000);
        lcd.clear();
  
      }
    }
  }
}
