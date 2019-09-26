/*
 AR25_number.ino
 */

int blinkNumber=0;

// 문자열 두가지를 설정한다.
String stringValue[]={"Binary:", "Octal: "}; // array

void setup() {
  // 9600bps로 시리얼 통신 설정
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    char val =Serial.read();
    if(isDigit(val)){
      blinkNumber=(val-'0');
    }
  }
  if(blinkNumber != 0)
  {
   // 'char Value'를 출력하고 문자열과 숫자를 변수 유형별로 출력한다.
  Serial.print("Number = ");
  Serial.print(blinkNumber);
  Serial.print(", ");
  Serial.print(stringValue[0]);  // stringValue 중 첫 번째 문자열 출력
  Serial.print(blinkNumber,BIN); // 2진수 형태로 출력
  Serial.print(", ");
  Serial.print(stringValue[1]);  // stringValue 중 첫 번째 문자열 출력
  Serial.print(blinkNumber,OCT); // 16진수 형태로 출력
  // 줄바꿈
  Serial.println();
  }

  //점멸 횟수를 리셋함
  blinkNumber=0;
}
