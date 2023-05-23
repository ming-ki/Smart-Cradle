#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

const char* ssid = "ubicomp"; // Wi-Fi SSID
const char* password = "ubicomp407"; // Wi-Fi Password
const char* server = "203.253.128.177"; // 모비우스 서버 IP 주소
const int port = 7579; // 모비우스 서버 포트
const String cnt = "Temperature";
const String ae = "CapstonDesign"; // 모비우스에서 사용할 AE 이름
const String cnt2 = "State";

#define DHTPIN 5 // DHT11 데이터 핀
#define DHTTYPE DHT11 // DHT11 사용

DHT dht(DHTPIN, DHTTYPE); // DHT11 객체 생성

int hum_pin = 0; // 가습기 핀
float t, h;
String con = "0"; // 초기 가습기 상태 값

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

  dht.begin(); // DHT11 초기화
  pinMode(hum_pin, OUTPUT); // 가습기 핀을 출력 모드로 설정
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int waterSensorVal = analogRead(A0);
    float temp = dht.readTemperature(); // 온도 측정
    float hum = dht.readHumidity();
    Serial.println(temp);
    Serial.println(hum);
    if (isnan(temp)) {
      Serial.println("Failed to read temperature from DHT sensor");
      delay(1000);
      return;
    }

    // JSON 데이터 생성
    DynamicJsonDocument doc(512); // JSON 문서 크기를 512바이트로 설정
    doc["m2m:cin"]["con"] = String(temp) + "," + String(hum) + "," + String(waterSensorVal);
    String payload;
    serializeJson(doc, payload); // JSON 데이터를 문자열로 직렬화

    HTTPClient http;
    HTTPClient http2;

    // HTTP POST 요청 설정
    http.begin("http://" + String(server) + ":" + String(port) + "/Mobius/" + ae + "/" + cnt);
    http.addHeader("Content-Type", "application/vnd.onem2m-res+json; ty=4");
    http.addHeader("Accept", "application/json");
    http.addHeader("X-M2M-RI", "12345");
    http.addHeader("X-M2M-Origin", ae);
    int httpCode = http.POST(payload); // HTTP POST 요청 전송
    String response = http.getString(); // 서버 응답 수신
    http.end(); // HTTP 연결 종료

    // State 데이터를 받아옴
    http2.begin("http://" + String(server) + ":" + String(port) + "/Mobius/" + ae + "/" + cnt2 + "/latest");
    http2.addHeader("Accept", "application/json");
    http2.addHeader("X-M2M-RI", "12345");
    http2.addHeader("X-M2M-Origin", ae);
    int httpCode2 = http2.GET();
    String response2 = http2.getString();
    http2.end();

    if (httpCode == 201) { // HTTP 응답 코드가 201인 경우
      Serial.println("Data sent to Mobius");
      Serial.println(response);
    } else {
      Serial.print("HTTP POST failed: ");
      Serial.println(httpCode);
      Serial.println(response);
    }

    if (httpCode2 == 200) {
      Serial.println("Data received from Mobius:");

      DynamicJsonDocument doc2(512); // JSON 문서 크기를 512바이트로 설정
      deserializeJson(doc2, response2);

      con = doc2["m2m:cin"]["con"].as<String>();
      Serial.println("State: " + con);
    }

    // 습도가 36% 미만이거나 con 값이 "1"이면 가습기를 작동시킴
    if (hum <= 36 || con == "1") {
      digitalWrite(hum_pin, HIGH); // 가습기 켬
    } else {
      digitalWrite(hum_pin, LOW); // 가습기 끔
    }
  } else {
    Serial.println("Wi-Fi disconnected");
  }

  delay(5000); // 5초마다 데이터 전송 및 가습기 제어
}
