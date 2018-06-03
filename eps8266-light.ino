#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <AM2320.h>

AM2320 th;
#define DS18B20_PIN 5
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

const String websenderURI = "http://192.168.1.141/update.php";

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);

WiFiClient wclient;
PubSubClient client(wclient);
// Для файловой системы
File fsUploadFile;



void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);

  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");

  Wire.begin();
   sensors.begin();
  sensors.setResolution(12);
  for (int i = 0; i < PORT; i++) {
    pinMode(GpioList[i], OUTPUT);
    digitalWrite(GpioList[i], HIGH);
  }

  FS_init();
  loadConfig();

  //WiFi.hostname(Hostname);

  HTTP_init();


}


unsigned long timer = 0;


void loop() {

  HTTP.handleClient();
  delay(1);
  if (client.connected()) {
    client.loop();
    refreshData();
  } else {
    mqttConnect();
  }


}
