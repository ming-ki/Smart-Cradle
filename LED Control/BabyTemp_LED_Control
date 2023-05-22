#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "ubicomp";
const char* password = "ubicomp407";
const char* server = "203.253.128.177";
const int port = 7579;
const String cnt = "BabyTemp";
const String ae = "CapstonDesign";

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
      Serial.println(con);

      // con 값이 36.5 이상인 경우 모든 LED를 켬
      if (con.toFloat() > 36.5) {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(255, 0, 0)); // LED를 빨간색으로 설정
        }
        strip.show();
      }
      else {
        for (int i = 0; i < LED_COUNT; i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 0)); // LED를 끔
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
