
// ********************************************************************** //
// ИНИЦИАЛИЗАЦИЯ WIFI
bool wifi_init () {
  WiFi.persistent(false);               // Запись во Flash только при изменении параметров подключения
  WiFi.mode(WIFI_OFF);
  if (startSTA()) {
    //    flagCheckUpdate = true;                         // Запуск проверки наличия обновления на сервере
    WiFi.setAutoReconnect(true);        // Активация переподключения при потере связи
  } else {
    startAPSTA();
  }
  WiFi.setAutoConnect(true);            // Автоматическое подключение к последней точке доступа при вкл. питания
  findDevices();                        // Запуск поиска устройств в сети
  WiFi.setHostname(configDev.ApSSID);   // Указание имени хоста
  return true;
}
// ********************************************************************** //
// ЗАПУСК РЕЖИМА STA
bool startSTA() {
  WiFi.mode(WIFI_STA);
  //  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.begin(configDev.StaSSID, configDev.StaPSK);
  if (strcmp(configDev.StaIPMode, "DHCP")) {
    IPAddress address, netmask, gateway;
    address.fromString(configDev.StaIP);
    netmask.fromString(configDev.StaMS);
    gateway.fromString(configDev.StaGW);
    WiFi.config(address, gateway, netmask);
  }
  int i = 10;
  while (i) {
    if (WiFi.status() == WL_CONNECTED) {
      uart_send("SYS", "WiFi", "[{\"Status\":\"STA\"}]");
      return true;
    } else {
      delay(1000);
      Serial.print(".");
      i--;
    }
  }
  return false;
}
// ********************************************************************** //
// ЗАПУСК РЕЖИМА AP+STA
void startAPSTA() {
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP_STA);
  config_init(0);                                   // Обновление настроек
  IPAddress address, netmask, gateway;
  address.fromString(configDev.ApIP);
  netmask.fromString(configDev.ApMS);
  gateway.fromString(configDev.ApGW);
  WiFi.softAPConfig(address, gateway, netmask);
  WiFi.softAP(configDev.ApSSID, configDev.ApPSK);
  uart_send("SYS", "WiFi", "[{\"Status\":\"AP+STA\"}]");
}
// ********************************************************************** //
// Установка флага проверки подключения к сети
void checkConnectFlag() {
  flagWConnect = true;
}
// ********************************************************************** //
// ПРОВЕРКА ПОДКЛЮЧЕНИЯ К СЕТИ
void checkConnect_handler() {
  if (flagWConnect == true) {
    flagWConnect = false;

    if (WiFi.getMode() == WIFI_STA && WiFi.status() != WL_CONNECTED)  {
      // Запуск режима AP+STA при отсутствии соединения в режиме STA
      wifi_init();
      return;
    }
    if (WiFi.getMode() == WIFI_AP_STA && WiFi.softAPgetStationNum() == 0) {
      // Подключение к сети, если нет подключенных клиентов
      int n = WiFi.scanComplete();
      if (n == -2) {
        WiFi.scanNetworks(true);
      } else if (n) {
        for (int i = 0; i < n; ++i) {
          if (WiFi.SSID(i) == configDev.StaSSID) {
            WiFi.scanDelete();
            wifi_init();
            return;
          }
        }
        WiFi.scanDelete();
        if (WiFi.scanComplete() == -2)
          WiFi.scanNetworks(true);
      }
    }
  }
}
// ********************************************************************** //
// Сканирование точек доступа
void printListAPs(int networksFound) {
  DynamicJsonDocument doc(2048);
  JsonArray apList = doc.to<JsonArray>();
  for (int i = 0; i < networksFound; ++i) {
    JsonObject net = apList.createNestedObject();
    net["ssid"] = WiFi.SSID(i);
    net["rssi"] = WiFi.RSSI(i);
    net["enc"] = WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Close";
    net["ch"] = WiFi.channel(i);
  }
  String netList;
  serializeJson(apList, netList);
  uart_send("OK!", "Net List", netList.c_str());
  sendWS(NULL, "NetList", netList);
}
// Использование
// int n = WiFi.scanNetworks();
// printListAPs(n);
