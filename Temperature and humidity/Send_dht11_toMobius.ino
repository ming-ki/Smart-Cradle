#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char* ssid = "ubicomp"; // Wi-Fi SSID
const char* password = "ubicomp407"; // Wi-Fi Password
const char* server = "203.253.128.161"; // 모비우스 서버 IP 주소
const int port = 7579; // 모비우스 서버 포트
const String cnt = "Temperature";
const String ae = "CapstonDesign"; // 모비우스에서 사용할 AE 이름

#define DHTPIN 5 // DHT11 데이터 핀
#define DHTTYPE DHT11 // DHT11 사용

DHT dht(DHTPIN, DHTTYPE); // DHT11 객체 생성

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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature(); // 온도 측정
    float hum = dht.readHumidity();
    if (isnan(temp)) {
      Serial.println("Failed to read temperature from DHT sensor");
      delay(1000);
      return;
    }

    String payload = "{\"m2m:cin\": {\"con\": \"" + String(temp) + "," + String(hum) + "\"}}"; // 전송할 데이터

    HTTPClient http;

    // HTTP POST 요청 설정
    http.begin("http://" + String(server) + ":" + String(port) + "/Mobius/" + ae + "/" + cnt);
    http.addHeader("Content-Type", "application/vnd.onem2m-res+json; ty=4");
    http.addHeader("Accept", "application/json");
    http.addHeader("X-M2M-RI", "12345");
    http.addHeader("X-M2M-Origin", ae);
    int httpCode = http.POST(payload); // HTTP POST 요청 전송
    String response = http.getString(); // 서버 응답 수신
    http.end(); // HTTP 연결 종료

    if (httpCode == 201) { // HTTP 응답 코드가 201인 경우
      Serial.println("Data sent to Mobius");
      Serial.println(response);
    } else {
      Serial.print("HTTP POST failed: ");
      Serial.println(httpCode);
      Serial.println(response);
    }
  } else {
    Serial.println("Wi-Fi disconnected");
  }

  delay(5000); // 5초마다 데이터 전송
}
