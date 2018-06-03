unsigned long intervalsend = 0;
void websender() {
  if (millis() - intervalsend > 20000) {
    th.Read();
    
    HTTPClient httpClient;


    int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
    sensors.requestTemperatures();

    String req = "cid=" + String(ESP.getChipId());

    req += "&humidity_"+String(ESP.getChipId())+"=";
    req += (float)th.h;
    req += "&temperature_"+String(ESP.getChipId())+"=";
    req += (float)th.t;
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
    httpClient.begin(websenderURI + "?" + req);
    httpClient.GET();
    httpClient.end();
    intervalsend = millis();
  }

  if (millis() - intervalHistory > 300000) {

    history();
    intervalHistory = millis();
  }
}
