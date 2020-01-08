#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "Ambient.h"

DHT12 dht12;
Adafruit_BMP280 bme;

WiFiClient client;
Ambient ambient;

const char* ssid = "";
const char* password = "";

unsigned int channelId = 12345;
const char* writeKey = "";

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(0,26);

  M5.Axp.ScreenBreath(8);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);

  if (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected\r\nIP address: ");
  Serial.println(WiFi.localIP());

  ambient.begin(channelId, writeKey, &client);
}

void loop() {
  delay(2000);
    
  double vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
  float pressure = bme.readPressure();
  
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.printf("temp: %4.1f'C\r\n", tmp);
  M5.Lcd.printf("humid:%4.1f%%\r\n", hum);
  M5.Lcd.printf("press:%4.0fhPa\r\n", pressure / 100);
  M5.Lcd.printf("vbat: %4.2fV\r\n", vbat);
  
  ambient.set(1, tmp);
  ambient.set(2, hum);
  ambient.set(3, pressure / 100);
  ambient.set(4, vbat);
  ambient.send();

  delay(60000);
}
