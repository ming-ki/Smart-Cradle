#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "ubicomp";
const char* password = "ubicomp407";
const char* server = "203.253.128.177";
const int port = 7579;
const String ae = "CapstonDesign";
const String cnt = "LED";

const int LED_PIN = D7;
const int LED_COUNT = 36;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  strip.begin();
  strip.show(); // 초기 LED 상태를 설정
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    HTTPClient http2;

    // BabyTemp 데이터를 받아옴
    http.begin("http://" + String(server) + ":" + String(port) + "/Mobius/" + ae + "/" + cnt + "/latest");
    http.addHeader("Accept", "application/json");
    http.addHeader("X-M2M-RI", "12345");
    http.addHeader("X-M2M-Origin", ae);
    int httpCode = http.GET();
    String response = http.getString();
    http.end();

    if (httpCode == 200) {
      Serial.println("Data received from Mobius:");

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);

      String con = doc["m2m:cin"]["con"].as<String>();
      Serial.println("BabyTemp: " + con);

      // LED 제어
      if (con == "1") {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(255, 255, 255)); // 흰색으로 설정
        }
        strip.show();
      } else if (con == "0") {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 0)); // LED를 끔
        }
        strip.show();
      } else if (con == "2") {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(255, 0, 0)); // 빨강색으로 설정
        }
        strip.show();
      } else if (con == "3") {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(255, 255, 0)); // 노랑색으로 설정
        }
        strip.show();
      } else if (con == "4") {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 255)); // 파랑색으로 설정
        }
        strip.show();
      }
    } else {
      Serial.print("HTTP GET failed: ");
      Serial.println(httpCode);
      Serial.println(response);
    }
  } else {
    Serial.println("Wi-Fi disconnected");
  }

  delay(5000);
}
