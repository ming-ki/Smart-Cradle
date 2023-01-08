#include <Wire.h>  
#include <BH1750.h>  
#include <Adafruit_NeoPixel.h> //WS2812 라이브러리
#include <SoftwareSerial.h>  
BH1750 lightMeter; //SCL -> A5 SDA -> A4

//Bluetooth
SoftwareSerial BTSerial(4,5);

//PIR
const int pinPIRM = 2;
int val = 0;

//WS2812 LED
#define LED_PIN 6 //핀 번호
#define LED_COUNT 60 // LED 개수
#define BRIGHTENESS 50 //LED 밝기(0~255) *RGB만

// RGB일 경우 NEO_GRBW 대신 NEO_GRBW 입력
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup(){
  BTSerial.begin(9600);
  pinMode(pinPIRM,INPUT);  
  Serial.begin(9600);  
  lightMeter.begin();
  strip.begin();
  strip.setBrightness(BRIGHTENESS);
  //strip.show();  
}  
  
  
void loop() {
  //BH1750  
  uint16_t lux = lightMeter.readLightLevel();  
  Serial.print("Light: ");  
  Serial.print(lux);  
  Serial.println(" lx");

  //PIR
  val = digitalRead(pinPIRM);
  Serial.println(val);
    
  delay(1000);

  if (val == 1){
    colorWipe(strip.Color(255,255,255),100);
  }
  else if(val == 0) {
    colorWipe(strip.Color(0,0,0,255),50);
  }

  //Bluetooth 
  if(BTSerial.available())//블루투스로 데이터를 받았다면
  {
    char data;
    data=(char)BTSerial.read();
    if(data=='1')
    {
      Serial.println("기저귀 교체 요망");
      theaterChase(strip.Color(127,0,0),50); // 빨간색 출력
    }
}

}

void colorWipe(uint32_t c, uint8_t wait){    //  loop에서 사용할 colorWipe 함수를 만든 공식

for(uint16_t i=0; i<strip.numPixels(); i++){

strip.setPixelColor(i,c);

strip.show();

delay(wait);

   }

}

//입력한 색으로 LED를 깜빡거리며 표현한다
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
