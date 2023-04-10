
#include "AsyncUDP.h"
// ********************************************************************** //
// UDP
String devsListTemp =   "";               // Временный список устройств в сети
String * ipList =       new String [100]; // Массив найденных IP адресов устройств
bool flagHandlePacket = false;            // Чтение пакетов принимаемых по широковещательному запросу

AsyncUDP udp;                             // Инициализируем UDP
// ********************************************************************** //
// ИНИЦИАЛИЗАЦИЯ UDP, ОБРАБОТКА ОТВЕТОВ, ДОБАВЛЕНИЕ НОВЫХ УСТРОЙСТВ
bool udp_init() {
  //if (udp.listenMulticast(IPAddress(239, 255, 255, 250), 1900) && WiFi.status() == WL_CONNECTED) {
  if (udp.listenMulticast(IPAddress(239, 255, 255, 250), 1900)) {
    udp.onPacket([](AsyncUDPPacket packet) {                    // Прием пакета
      if (flagHandlePacket) {                                   // Если разрешен прием пакетов - обрабатываем
        const char * inPack = (char*)packet.data();             // Забираем принятый пакет в inPack
        if (strstr(inPack, "SERVER: SOI Tech")) {               // Если в ответе есть "SERVER: SOI Tech" - обрабатываем пакет
          for (int i = 0; i < 100; i++) {                       // Перебираем массив IP адресов,
            if (ipList[i] == NULL) {                            // если дошли до конца списка IP адресов и не нашли совпадений, то
              ipList[i] = packet.remoteIP().toString();         // добавляем IP адрес ответившего устройства в массив,
              char * temp = strstr(inPack, "\r\n\r\n,{") + 4;   // находим начало данных и
              char * temp2 = strstr(temp, "\r\n\r\n");          // конец строки
              temp2[0] = 0;                                     // затираем все после конца строки,
              devsListTemp += temp;                             // добавляем объект (полезные данные) к списку устройств
              return;
            }
            if (ipList[i] == packet.remoteIP().toString())      // Если IP ответившего устройства уже есть в списке, то
              return;                                           // игнорируем пакет
          }
        }
      }
      packet.printf(                                            // Ответ на широковещательный запрос
        "HTTP/1.1 200 OK\r\n"                                   // Формируем шаблон ответа
        "CACHE-CONTROL: max-age=1800\r\n"                       // и добавляем данные об устройстве
        "EXT: \r\n"                                             // после двойных переносов строк (\r\n\r\n)
        "LOCATION: http://%s:80/description.xml\r\n"            // для того, чтоб не делать
        "SERVER: SOI Tech/1.0 UPnP/1.1 %s\r\n"                  // дополнительный запрос на получение
        "ST: upnp:rootdevice\r\n"                               // данных об устройстве
        "USN: uuid:EDC3B25E-C2D0-411E-B04B-F9BD5E%02X::upnp:rootdevice\r\n\r\n"
        ",{\"ip\":\"%s\",\"ra\":\"%s\",\"DevID\":\"%s\",\"DevName\":\"%s\",\"DevColor\":\"%s\"}\r\n\r\n",  // JSON Объект с данными об устройстве
        WiFi.localIP().toString().c_str(),
        DEVICE_ID,
        configDev.RFAddr,
        WiFi.localIP().toString().c_str(), configDev.RFAddr, DEVICE_ID, configDev.DevName, configDev.DevColor
      );
    });
    return true;
  } else
    return false;
}
// ********************************************************************** //
// СКАНИРОВАНИЕ УСТРОЙСТВ В СЕТИ
void findDevices() {
  if (WiFi.status() == WL_CONNECTED) {
    // Добавляем данные о локальном устройстве в список устройств
    devsListTemp = "[{\"ip\":\""; devsListTemp += WiFi.localIP().toString(); devsListTemp += "\",";
    devsListTemp += "\"DevSN\":\""; devsListTemp += configDev.DevSN; devsListTemp += "\",";
    devsListTemp += "\"DevName\":\""; devsListTemp += configDev.DevName; devsListTemp += "\",";
    devsListTemp += "\"RFAddr\":\""; devsListTemp += configDev.RFAddr; devsListTemp += "\",";
    devsListTemp += "\"DevColor\":\""; devsListTemp += configDev.DevColor; devsListTemp += "\"}";
    ipList[0] = WiFi.localIP().toString();                      // а так же IP адрес локального устройства в список IP адресов
    udp.broadcastTo(                                            // Формируем широковещательный запрос
      "M-SEARCH * HTTP/1.1\r\n"                                 // Фрейма M-SEARCH
      "Host:239.255.255.250:1900\r\n"                           // IP-адрес и порт для мультивещания
      "ST:upnp:rootdevice\r\n"                                  // Поиск корневых устройств
      "Man:\"ssdp:discover\"\r\n"
      "MX:3\r\n\r\n", 1900
    );
    flagHandlePacket = true;                                    // Разрешаем обработку принятых пакетов
    waitScan.attach(3, udpScanStop);                            // Отключаем сканер через 3 секунды
  }
}
// ********************************************************************** //
// ОТКЛЮЧЕНИЕ СКАНЕРА
void udpScanStop() {
  flagHandlePacket = false;                                     // Запрещаем обработку принятых пакетов
  waitScan.detach();                                            // Выключаем диспетчер задержки
  devsListTemp += "]";                                          // Закрываем массив списка устройств
  devsList = devsListTemp;                                      // Формируем итоговый список устройств.
  devsListTemp = "";                                            // Чистим временный буфер списка устройств
  for (int i = 0; i < 100; i++) {                               // Чистим список IP адресов устройств
    ipList[i] = "";
  }
  //sendWS(NULL, "DevsList", devsList);                           // Возвращаем список найденных устройств
}
