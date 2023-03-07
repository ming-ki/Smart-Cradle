#include <Adafruit_NeoPixel.h>

#define LED_PIN 5        // LED가 연결된 핀 번호
#define LED_COUNT 36     // 연결된 LED 개수
#define PIR_PIN D4       // PIR 센서가 연결된 핀 번호

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pinMode(PIR_PIN, INPUT);  // PIR 센서의 핀을 입력으로 설정합니다.
}

void loop() {
  int pirValue = digitalRead(PIR_PIN);
  Serial.println(pirValue);
  if(pirValue == HIGH) {
    for(int i=0; i<LED_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // 모든 LED를 흰색으로 설정합니다.
    }
    pixels.show();                                     // 설정한 색으로 LED를 표시합니다.
  }else{
    for(int i=0; i<LED_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));   // 모든 LED를 끕니다.
  }
  pixels.show();
  }
  delay(100);    // 0.1초 대기
}
