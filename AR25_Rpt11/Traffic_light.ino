#include "LedControlMS.h"

#define NBR_MTX 1  //number of matrices attached is one

LedControl lc=LedControl(4,3,2, NBR_MTX);

byte stophuman[] =
{
   B00011000,  // First frame of invader #1
   B00011000,
   B01111110,
   B01011010,
   B01011010,
   B00100100,
   B00100100,
   B00100100
};

byte movehuman1[] =
{
  B00011000, // Second frame of invader #1
  B00011000,
  B01111110,
  B01011010, 
  B11011011,
  B00111100,
  B01000110,
  B11000011
};

byte movehuman2[]=
{
  B00011000, // Second frame of invader #2
  B00011000,
  B01111110,
  B01011010, 
  B01111110,
  B00111100,
  B01100110,
  B01001100
};

//7-segment
byte data[6]={
B00111111, //0
B00000110, //1
B01011011, //2
B01001111, //3
B01100110, //4
B01101101, //5
};

const byte latchPin = 11; //STCP Pin
const byte clockPin = 10; //SHCP Pin
const byte dataPin = 9; //DS Pin
int SegPower = 0;
const byte TransversboardLight[2] = {12,13};
int cnt = 5;
boolean state = false;

const byte trafficLight[3] = {5,6,7}; //신호등 핀
const byte lightTime[3] = {5,2,7};
unsigned long timeVal = 0;
unsigned long timeSeg = 0;
int indexVal = 0;


void setup()
{
  
  lc.shutdown(0,false); //Wake up displays
  lc.setIntensity(0,5);  // Set intensity levels
  //lc.setIntensity(1,5);
  lc.clearDisplay(0);   // Clear Displays
  //lc.clearDisplay(1);

  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);;
  
  pinMode(TransversboardLight[0], OUTPUT);
  pinMode(TransversboardLight[1], OUTPUT);
  
  for(int i=0;i<3;i++){
    pinMode(trafficLight[i], OUTPUT); 
  }  //초기상태
  digitalWrite(trafficLight[indexVal], HIGH); //초록색 LED On
  digitalWrite(TransversboardLight[1], HIGH); //빨간색 LED On
}
  
//  Take values in Arrays and Display them
void sstophuman()
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,stophuman[i]);
  }
}

void smovehuman1()   
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,movehuman1[i]);
  }
}

void smovehuman2()
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,movehuman2[i]);
  }
}

void loop()
{
  
  if(SegPower == 0)
    shiftOut(dataPin, clockPin, MSBFIRST, data[4]); // if(SegPower == 1) / data[2] -> data[4]; 

  if(millis()-timeVal>=lightTime[indexVal]*1000){ //신호등 동작 trafficLight[3]순서대로
    digitalWrite(trafficLight[indexVal], LOW);  //이전등 끄기
    indexVal++; //신호등위치 증가
    if(indexVal==3)indexVal=0; // 신호등위치가 3이 되면 다시 0으로 처음위치로 돌아감
    digitalWrite(trafficLight[indexVal], HIGH); //새로운등 켜기
    timeVal=millis();   
    
    if(indexVal==2){
      state=true;
      timeSeg=millis();
      digitalWrite(TransversboardLight[1], LOW);  //빨간색 LED Off
      digitalWrite(TransversboardLight[0], HIGH); //초록색 LED On
      
      SegPower = 1;//7 세그먼트 작동      
      digitalWrite(latchPin, LOW);  //열림
      shiftOut(dataPin, clockPin, MSBFIRST, data[cnt]); //출력
      digitalWrite(latchPin, HIGH); //닫힘
    }
  }
  if(state==true){
    smovehuman1();
    delay(155);
    smovehuman2(); 
    delay(140);
    if(cnt>0 && millis()-timeSeg>=1000){ //"5-4-3-2-1-0"출력함
    cnt--; 
    Serial.println(cnt);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data[cnt]); //숫자 출력
    digitalWrite(latchPin, HIGH);                       
    timeSeg=millis();  //이전시간변수에 현재시간 저장
 }
 if(cnt==0 && millis()-timeSeg>=1000) //0을 출력하고 1초동안은 0인 상태를 유지하기 위함
 {
    cnt=5; //숫자패턴 5으로 초기화
    state = false; //횡단보도신호등 동작 중단
    sstophuman();
         
     if(SegPower == 1)
     {
       digitalWrite(TransversboardLight[0], LOW); //초록색 LED Off
       digitalWrite(TransversboardLight[1], HIGH); //빨간색 LED On
       SegPower = 0;//7-Segment Display 전원 공급 중단
     }
    }
  } 
}
