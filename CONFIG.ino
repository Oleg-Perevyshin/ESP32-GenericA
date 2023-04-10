
struct Config {
  // Данные пользователя устройства
  char UserID[17];                // ID пользователя в глобальной БД
  // Данные об устройстве
  char DevSN[20];                 // SN устройства
  char DevID[4] = DEVICE_ID;      // ID устройства
  char DevName[17];               // Имя устройства
  char DevColor[4];               // Цветовая зона
  char DevFW[6];                  // Версия прошивки
  char TimeZone[33];              // Временная зона
  // Параметры RF
  char RFAddr[7];                 // Адрес устройства (для Радио сети)
  char RFCh[3];                   // Частотный канал (в Радио сети)
  // Параметры WEB
  char WebUser[17];               // Имя пользователя для доступа к закрытым областям
  char WebPsw[17];                // Пароль для доступа к закрытым областям
  char WebTheme[2];               // Тема визуального оформления интерфейса
  char WebLng[3];                 // Язык интерфейса
  char WebLngList[32];            // Поддерживаемые языки ["en","es","fr","it","pt","ru"], каждый язык +5 символов
  // Параметры STA
  char StaSSID[33];               // Точка доступа
  char StaPSK[65];                // Пароль Точки доступа
  char StaIPMode[5];              // Режим параметров сети, статические или динамисеские
  char StaIP[16];                 // IP адрес при статических параметрах сети
  char StaMS[16];                 // Маска подсети при статических параметрах сети
  char StaGW[16];                 // Адрес шлюза при статических параметрах сети
  // Парметры AP
  char ApSSID[17];                // Идентификатор устройства в режиме Точка доступа
  char ApPSK[17];                 // Пароль устройства в режиме Точка доступа
  char ApIP[16];                  // IP адрес устройства в режиме Точка доступа
  char ApMS[16];                  // Маска подсети устройства в режиме Точка доступа
  char ApGW[16];                  // Адрес шлюза в режиме Точка доступа
  // ... добавить пользовательские переменные (имена IO и т.п.)
  char u1[17];
  char u2[17];
};
struct Config configDev;
extern struct Config configDev;
// ********************************************************************** //
// ЗАГРУЖАЕМ НАСТРОЙКИ ПО УМОЛЧАНИЮ
bool config_default() {
  deleteFile(CONFIG_FILE);
//  if (deleteFile(CONFIG_FILE) == false) {
//    return false;
//  }
  strncpy(configDev.UserID, "000000000000", sizeof(configDev.UserID) - 1);
  strncpy(configDev.DevSN, get_device_SN(), sizeof(configDev.DevSN) - 1);
  strncpy(configDev.DevID, DEVICE_ID, sizeof(configDev.DevID) - 1);
  strncpy(configDev.DevName, "SOI-" DEVICE_ID, sizeof(configDev.DevName) - 1);
  strncpy(configDev.DevColor, "92f", sizeof(configDev.DevColor) - 1);
  strncpy(configDev.DevFW, DEVICE_FW_VERSION, sizeof(configDev.DevFW) - 1);
  strncpy(configDev.TimeZone, "Europe/Vilnius", sizeof(configDev.TimeZone) - 1);
  uint32_t chip_ID;
  for (uint8_t i = 0; i < 17; i = i + 8) {
    chip_ID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  snprintf(configDev.RFAddr, sizeof(configDev.RFAddr), "%02X", chip_ID);
  strncpy(configDev.RFCh, "10", sizeof(configDev.RFCh) - 1);
  strncpy(configDev.WebUser, "admin", sizeof(configDev.WebUser) - 1);
  strncpy(configDev.WebPsw, "", sizeof(configDev.WebPsw) - 1);
  strncpy(configDev.WebTheme, "0", sizeof(configDev.WebTheme) - 1);
  strncpy(configDev.WebLng, "ru", sizeof(configDev.WebLng) - 1);
  // Создание списка языков на основе сканирования папки dl
  DynamicJsonDocument doc(512);
  JsonArray langList = doc.to<JsonArray>();
  File dir = SPIFFS.open("/dl");
  if (!dir || !dir.isDirectory()) {
    uart_send("SYS", "Config", "[{\"LangList\":\"Error Create\"}]");
    return false;
  }
  File file = dir.openNextFile();
  while (file) {
    const char *fileName = file.name();
    char langName[3] = "";
    langName[0] = fileName[0];
    langName[1] = fileName[1];
    langName[2] = '\0';
    langList.add(langName);
    file = dir.openNextFile();
  }
  file.close(); dir.close();
  serializeJson(langList, configDev.WebLngList);
  
  strncpy(configDev.StaSSID, "POE-Net", sizeof(configDev.StaSSID) - 1);
  strncpy(configDev.StaPSK, "MCMega2005", sizeof(configDev.StaPSK) - 1);
  strncpy(configDev.StaIPMode, "DHCP", sizeof(configDev.StaIPMode) - 1);
  strncpy(configDev.StaIP, "", sizeof(configDev.StaIP) - 1);
  strncpy(configDev.StaMS, "", sizeof(configDev.StaMS) - 1);
  strncpy(configDev.StaGW, "", sizeof(configDev.StaGW) - 1);
  snprintf(configDev.ApSSID, sizeof(configDev.ApSSID), "SOI-%s", configDev.RFAddr);
  strncpy(configDev.ApPSK, "", sizeof(configDev.ApPSK) - 1);
  strncpy(configDev.ApIP, "192.168.108.1", sizeof(configDev.ApIP) - 1);
  strncpy(configDev.ApMS, "255.255.255.0", sizeof(configDev.ApMS) - 1);
  strncpy(configDev.ApGW, "192.168.108.1", sizeof(configDev.ApGW) - 1);
  // ... добавить пользовательские переменные
  strncpy(configDev.u1, "In-1", sizeof(configDev.u1) - 1);
  strncpy(configDev.u2, "In-2", sizeof(configDev.u2) - 1);
  // Если нет файла сценариев - создаем пустой
  if (!SPIFFS.exists(SCRIPTS_FILE)) {
    String scripts = "[]";
    File f = SPIFFS.open(SCRIPTS_FILE, "w", false);
    if (!f) {
      uart_send("ER!", "Config", "[{\"Scripts\":\"Create\"}]");
      return false;
    }
    f.print(scripts); f.flush(); f.close();
  } else {
    uart_send("SYS!", "Config", "[{\"Scripts\":\"Exists\"}]");
  }
  flagSaveConfig = true;
  return true;
}
// ********************************************************************** //
bool config_init(int target) {
  // Если нет файла config.json или запрошен Сброс настроек по умолчанию
  if (!SPIFFS.exists(CONFIG_FILE) || target == 1) {
    if (config_default() == true) {
      return true;
    } else {
      uart_send("SYS", "Config", "[{\"LoadDefault\":\"Error\"}]");
      return false;
    }
  } else {
    // ЗАГРУЖАЕМ НАСТРОЙКИ ИЗ ФАЙЛА config.json
    File f = SPIFFS.open(CONFIG_FILE, "r", false);
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, f);
    if (error) {
      return false;
    }
    JsonObject root = doc.as<JsonObject>();
    // ЗАГРУЖАЕМ НАСТРОЙКИ ИЗ ФАЙЛА
    strncpy(configDev.UserID, root["UserID"], sizeof(configDev.UserID) - 1);
    strncpy(configDev.DevSN, root["DevSN"], sizeof(configDev.DevSN) - 1);
    strncpy(configDev.DevID, root["DevID"], sizeof(configDev.DevID) - 1);
    strncpy(configDev.DevName, root["DevName"], sizeof(configDev.DevName) - 1);
    strncpy(configDev.DevColor, root["DevColor"], sizeof(configDev.DevColor) - 1);
    strncpy(configDev.DevFW, root["DevFW"], sizeof(configDev.DevFW) - 1);
    strncpy(configDev.RFAddr, root["RFAddr"], sizeof(configDev.RFAddr) - 1);
    strncpy(configDev.RFCh, root["RFCh"], sizeof(configDev.RFCh) - 1);
    strncpy(configDev.WebUser, root["WebUser"], sizeof(configDev.WebUser) - 1);
    strncpy(configDev.WebPsw, root["WebPsw"], sizeof(configDev.WebPsw) - 1);
    strncpy(configDev.WebTheme, root["WebTheme"], sizeof(configDev.WebTheme) - 1);
    strncpy(configDev.WebLng, root["WebLng"], sizeof(configDev.WebLng) - 1);
    strncpy(configDev.WebLngList, root["WebLngList"], sizeof(configDev.WebLngList) - 1);
    strncpy(configDev.StaSSID, root["StaSSID"], sizeof(configDev.StaSSID) - 1);
    strncpy(configDev.StaPSK, root["StaPSK"], sizeof(configDev.StaPSK) - 1);
    strncpy(configDev.StaIPMode, root["StaIPMode"], sizeof(configDev.StaIPMode) - 1);
    strncpy(configDev.StaIP, root["StaIP"], sizeof(configDev.StaIP) - 1);
    strncpy(configDev.StaMS, root["StaMS"], sizeof(configDev.StaMS) - 1);
    strncpy(configDev.StaGW, root["StaGW"], sizeof(configDev.StaGW) - 1);
    strncpy(configDev.ApSSID, root["ApSSID"], sizeof(configDev.ApSSID) - 1);
    strncpy(configDev.ApPSK, root["ApPSK"], sizeof(configDev.ApPSK) - 1);
    strncpy(configDev.ApIP, root["ApIP"], sizeof(configDev.ApIP) - 1);
    strncpy(configDev.ApMS, root["ApMS"], sizeof(configDev.ApMS) - 1);
    strncpy(configDev.ApGW, root["ApGW"], sizeof(configDev.ApGW) - 1);
    // ... добавить пользовательские переменные
    strncpy(configDev.u1, root["u1"], sizeof(configDev.u1) - 1 );
    strncpy(configDev.u2, root["u2"], sizeof(configDev.u2) - 1 );
    doc.clear();
    f.flush(); f.close();
    return true;
  }
}
// ********************************************************************** //
// Сохранение параметров в файл config.json
void saveConfig() {
  DynamicJsonDocument doc(2048);
  JsonObject root = doc.to<JsonObject>();
  root["UserID"] = configDev.UserID;
  root["DevSN"] = configDev.DevSN;
  root["DevID"] = configDev.DevID;
  root["DevName"] = configDev.DevName;
  root["DevColor"] = configDev.DevColor;
  root["DevFW"] = configDev.DevFW;
  root["RFAddr"] = configDev.RFAddr;
  root["RFCh"] = configDev.RFCh;
  root["WebUser"] = configDev.WebUser;
  root["WebPsw"] = configDev.WebPsw;
  root["WebTheme"] = configDev.WebTheme;
  root["WebLng"] = configDev.WebLng;
  root["WebLngList"] = configDev.WebLngList;
  root["StaSSID"] = configDev.StaSSID;
  root["StaPSK"] = configDev.StaPSK;
  root["StaIPMode"] = configDev.StaIPMode;
  root["StaIP"] = configDev.StaIP;
  root["StaMS"] = configDev.StaMS;
  root["StaGW"] = configDev.StaGW;
  root["ApSSID"] = configDev.ApSSID;
  root["ApPSK"] = configDev.ApPSK;
  root["ApIP"] = configDev.ApIP;
  root["ApMS"] = configDev.ApMS;
  root["ApGW"] = configDev.ApGW;
  // ... добавить пользовательские переменные
  root["u1"] = configDev.u1;
  root["u2"] = configDev.u2;

  File f = SPIFFS.open(CONFIG_FILE, "w", false);
  if (!f) {
    uart_send("SYS", "Config", "[{\"Save\":\"File not created\"}]");
  }
  serializeJson(doc, f);
  f.close();
  doc.clear();
  flagSaveConfig = false;
}
// ********************************************************************** //
void config_handler() {
  if (flagSaveConfig == true) {
    saveConfig();
  }
}
