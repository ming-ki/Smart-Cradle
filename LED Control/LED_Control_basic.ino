#include <Adafruit_NeoPixel.h>

const int pinPIRM = 2;
int val = 0;
bool motionDetected = false;
bool ledsOn = false;

#define LED_PIN     6
#define LED_COUNT   36

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(pinPIRM, INPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show(); // 초기 LED 상태를 설정
}

void loop() {
  val = digitalRead(pinPIRM);
  Serial.println(val);
  
  if (val == HIGH && !motionDetected) {
    motionDetected = true;
    
    if (!ledsOn) {
      // 모션 감지 시 모든 LED를 켬
      for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, 255, 255, 255);
      }
      strip.show();
      ledsOn = true;
    }
  }
  
  if (val == LOW && motionDetected) {
    motionDetected = false;
    
    if (ledsOn) {
      // 모션 감지가 해제되면 모든 LED를 끔
      for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, 0, 0, 0);
      }
      strip.show();
      ledsOn = false;
    }
  }
  
  delay(100);
}
