unsigned long intervalsend = 0;
void websender() {
  if (millis() - intervalsend > 20000) {
    //th.Read();
    
    HTTPClient httpClient;
    httpClient.setUserAgent("ShinePhone 1.3 (iPhone; iOS 9.0.2; Scale/1.0)"); 

    int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
    sensors.requestTemperatures();

    String req = "cid=" + String(ESP.getChipId());

  if (th.measure()) {
   
    req += "&humidity_"+String(ESP.getChipId())+"=";
    req += (float)th.getTemperature();
    req += "&temperature_"+String(ESP.getChipId())+"=";
    req += (float)th.getHumidity();
  }
   
    for (int i = 0; i <= deviceCount - 1; i++) {
      DeviceAddress Address18b20;
      sensors.getAddress(Address18b20, i);
      String tmp = "";
      String t = "";
      for (int i = 0; i < 8; i++) {
        t = String(Address18b20[i], HEX);
        while ( t.length() < 2)  t = "0" +  t;
        tmp += t;
      }
      req += "&" +tmp+"=";
      req += sensors.getTempC(Address18b20);
    }
    Serial.println(req);
    httpClient.begin(websenderURI + "?" + req);
    httpClient.GET();
    httpClient.end();
    intervalsend = millis();
  }

}
