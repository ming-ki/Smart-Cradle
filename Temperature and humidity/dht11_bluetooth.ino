#include <SoftwareSerial.h>
#include <dht11.h>

const int pinDHT11 = 2;
int ret;
dht11 DHT11;
SoftwareSerial BTSerial(4,5);

void setup() {
  Serial.begin(9600);
  //Serial.println("Hello");
  BTSerial.begin(9600);
}

void loop() {
  ret = DHT11.read(pinDHT11);
  switch (ret) {
  case DHTLIB_OK: Serial.println("OK"); break;
  case DHTLIB_ERROR_CHECKSUM: 
  Serial.println("Checksum error"); break;
  case DHTLIB_ERROR_TIMEOUT: 
  Serial.println("Time out error"); break;
  default: Serial.println("Unknown error"); break;
}
BTSerial.print("Humidity (%): ");
BTSerial.println((float)DHT11.humidity, 2);
BTSerial.print("Temperature (oC): ");
BTSerial.println((float)DHT11.temperature, 2);
delay(2000);
}
