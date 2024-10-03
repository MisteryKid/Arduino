
#include "LedControl.h" // 도트매트릭스 제어용 라이브러리
#include <SoftwareSerial.h>

// 도트매트릭스 사용 준비
LedControl lc = LedControl(8, 10, 9, 1); // (DIN,CLK,CS,number)
LedControl lc2 = LedControl(11, 13, 12,1);

SoftwareSerial btSerial(2,3);

int FSRsensor1 = A0; // 압력 센서 1
int FSRsensor2 = A1; // 압력 센서 2
int value1 =0; // 압력값 저장 
int value2 =0;

unsigned long previousMillis = 0;
const long interval = 500; // 500ms 간격으로 반짝임

byte down [] = 
{B00011000,
B00011000,
B00011000,
B00011000,
B11111111,
B01111110,
B00111100,
B00011000};

byte down_cross []=
{B00000011,
B00000111,
B10001110,
B11011100,
B11111000,
B11110000,
B11111000,
B11111100};

byte arrowLeft[] = {
  B11111100,
  B11111000,
  B11110000,
  B11111000,
  B11011100,
  B10001110,
  B00000111,
  B00000011
};

byte arrowRight[] =
{B00000011,
B00000111,
B10001110,
B11011100,
B11111000,
B11110000,
B11111000,
B11111100};

byte none []=
{B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B11111111};

byte none2[]=
{B11111111,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000};

void setup() {

  //모터 핀 설정 
  pinMode(5, OUTPUT);              // 5번핀을 출력모드로 설정합니다.
  pinMode(6, OUTPUT);              // 6번핀을 출력모드로 설정합니다.
  
  //첫 번째 도트 매트릭스 
  lc.shutdown(0, false);
  lc.setIntensity(0, 3); // LED 도트매트릭스 밝기
  lc.clearDisplay(0); // LED 도트매트릭스 모두 지우기
  
  //두 번째 도트 매트릭스 
  lc2.shutdown(0, false);
  lc2.setIntensity(0, 3); // 두 번째 LED 도트매트릭스 밝기
  lc2.clearDisplay(0); // 두 번째 LED 도트매트릭스 모두 지우기

  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {

  //센서 값 읽기 
  value1 = analogRead(FSRsensor1);
  value2 = analogRead(FSRsensor2);
  
  Serial.print("Sensor 1: ");
  Serial.println(value1);
  //btSerial.println(value1);
  delay(100);
  
  Serial.print("Sensor 2: ");
  Serial.println(value2);
  delay(100);

  // 현재 시간을 가져옴
  unsigned long currentMillis = millis();
  
  //첫 번째 도트 매트릭서와 첫 번째 압력 센서 
  if (value1<600){
    showLED(lc, arrowRight);
    delay(500);
    lc.clearDisplay(0);
    delay(500);
    btSerial.println("자리 비엇음");
  }
  else{
    btSerial.println("누가 앉음");
    
    showLED(lc, none);
  }

  
  // 두 번째 도트 매트릭스는 두 번째 압력 센서에 따라 동작 (노란색 테이프) 
  if (value2 < 600) { 
    if (currentMillis - previousMillis >= interval) {
      // 시간을 업데이트
      previousMillis = currentMillis;

      // 도트 매트릭스 반짝임
      lc2.clearDisplay(0); // 도트 매트릭스 지우기
      delay(500);         // 짧은 딜레이 후
      showLED(lc2, arrowRight); // 다시 표시
      delay(500);
      //btSerial.println("자리 비엇음");
    }

    // 진동 모터 동작 (100ms 간격으로 진동)
    analogWrite(5, 150); // 진동 모터 작동
  } else {
    //btSerial.println("자리에 사람이 있음");
    showLED(lc2, none2);
    //lc2.clearDisplay(0);
    analogWrite(5, 0); // 진동 멈춤
  }
    /*
    showLED(lc2, arrowLeft);
    delay(500);
    lc2.clearDisplay(0);
    delay(500);
  
    analogWrite(5, 0);                   // 5번핀에 0(0V)의 신호를 출력합니다.
    analogWrite(6, 150);              // 6번핀에 150(약 3V)의 신호를 출력합니다.
    delay(1000);                           // 3초간 대기
  
    analogWrite(5, 150);             // 5번핀에 150(약 3V)의 신호를 출력합니다.
    analogWrite(6, 0);                 // 6번핀에 0(0V)의 신호를 출력합니다.
    delay(1000);                         // 3초간 대기
  } 
  //아니라면 
  else {
    showLED(lc2, none2);
  }
  */
}


// LED 도트매트릭스 제어용 함수
void showLED(LedControl &lc, byte arr[]) {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, arr[i]);
  }
}
