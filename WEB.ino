
#include "lwip/inet.h"
// ********************************************************************** //
String api = "[[5,9],[2,5,3]]";  // API устройства (для примера)

// ГЕНЕРАТОР СТРАНИЦЫ index.html
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>SOI Tech | Device Manager</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA/plUAP2aWAD4pW0A9at5ACHchwAi3YgAJt2JADDcjgA/3JUAT9ycAFPdngBW3Z8Aat2oAHHdqwBz3awA8LSMAOu+oQCJ3LYAjt25AOrApADlyrgApNzDAKjdxQC93M4Ay9zUAM/d1gDR3NcA1tzZANnc2wDc3NwA4tLGAOPe2wDj3twA1ODbANnh3QDi4uIA4eTjAOXk4wDl5eUA6urqAO/v7wDx8fEA9fX1APr6+gD9/f0A0fv/AP///wAAAAAAAC8hAABQNwAAcEwAAJBjAACweQAAz48AAPCmABH/tAAx/74AUf/IAHH/0wCR/9wAsf/lANH/8AD///8AAAAAAAAvDgAAUBgAAHAiAACQLAAAsDYAAM9AAADwSgAR/1sAMf9xAFH/hwBx/50Akf+yALH/yQDR/98A////AAAAAAACLwAABFAAAAZwAAAIkAAACrAAAAvPAAAO8AAAIP8SAD3/MQBb/1EAef9xAJj/kQC1/7EA1P/RAP///wAAAAAAFC8AACJQAAAwcAAAPZAAAEywAABZzwAAZ/AAAHj/EQCK/zEAnP9RAK7/cQDA/5EA0v+xAOT/0QD///8AAAAAACYvAABAUAAAWnAAAHSQAACOsAAAqc8AAMLwAADR/xEA2P8xAN7/UQDj/3EA6f+RAO//sQD2/9EA////AAAAAAAvJgAAUEEAAHBbAACQdAAAsI4AAM+pAADwwwAA/9IRAP/YMQD/3VEA/+RxAP/qkQD/8LEA//bRAP///wAAAAAALxQAAFAiAABwMAAAkD4AALBNAADPWwAA8GkAAP95EQD/ijEA/51RAP+vcQD/wZEA/9KxAP/l0QD///8AAAAAAC8DAABQBAAAcAYAAJAJAACwCgAAzwwAAPAOAAD/IBIA/z4xAP9cUQD/enEA/5eRAP+2sQD/1NEA////AAAAAAAvAA4AUAAXAHAAIQCQACsAsAA2AM8AQADwAEkA/xFaAP8xcAD/UYYA/3GcAP+RsgD/scgA/9HfAP///wAAAAAALwAgAFAANgBwAEwAkABiALAAeADPAI4A8ACkAP8RswD/Mb4A/1HHAP9x0QD/kdwA/7HlAP/R8AD///8AAAAAACwALwBLAFAAaQBwAIcAkAClALAAxADPAOEA8ADwEf8A8jH/APRR/wD2cf8A95H/APmx/wD70f8A////AAAAAAAbAC8ALQBQAD8AcABSAJAAYwCwAHYAzwCIAPAAmRH/AKYx/wC0Uf8AwnH/AM+R/wDcsf8A69H/AP///wAAAAAACAAvAA4AUAAVAHAAGwCQACEAsAAmAM8ALADwAD4R/wBYMf8AcVH/AIxx/wCmkf8Av7H/ANrR/wD///8AAAAAAAAoIR8fISgAAAAAAAAAACgVBAICAQIEFSgAAAAAACcQAgECAQEBAQEQJwAAACgUAwMDAwMDAwMDAxEoAAAeIR4eHh4eHiQeHh4eHgAoKR4eHh4eHh4eHh4kHQ8oJS0oHh4eHh4eHh4eHhMHIyQtLR4eHh4eHh4eHhoIBhgkLS8pHh4eHh4eHh4OBgYYJC0vLSQeHh4eHh4YBwYHIygrLy8rHh4eHh4eDAYGCygAJy0vLSceHh4eFgYGBhcAACgqLy8sHh4eHAkGBg0oAAAAJyotLygeHhIFBg0lAAAAAAAoJystIRkIChYoAAAAAAAAAAAoJB4bIigAAAAAAPgfAADgBwAAwAMAAIABAACAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAEAAIABAADAAwAA4AcAAPgfAAA=" />
    <link type="text/css" rel="stylesheet" href="style.css">
  </head>
  <body class="loading">
    <div id="app"></div>
    <div id="loader"></div>
    <script src="main.js"></script>
  </body>
</html>
)rawliteral";
// ********************************************************************** //
// ГЕНЕРАТОР ИКОНКИ УСТРОЙСТВА
const char icon[] PROGMEM = R"rawliteral(<img style="height:4em;" src="data:image/svg+xml;utf8,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100' height='100' width='100'%3E%3Cdefs%3E%3Cmarker id='a' orient='auto' overflow='visible'%3E%3Cpath class='c e i' d='M.508 0l-3.46 2v-4z'/%3E%3C/marker%3E%3Cmarker id='b' orient='auto' overflow='visible'%3E%3Cpath class='c e i' d='M.508 0l-3.46 2v-4z'/%3E%3C/marker%3E%3C/defs%3E%3Cstyle%3Ecircle,path,rect%7Bstroke-linecap:round;stroke-linejoin:round%7D.a%7Bfill:none%7D.b%7Bfill:%23e66%7D.c%7Bfill:%23777%7D.d%7Bstroke:%23e66%7D.e%7Bstroke:%23777%7D.f%7Bpaint-order:stroke fill markers%7D.g%7Bcx:50;cy:50%7D.h%7Bstroke-width:2%7D.i%7Bstroke-width:.5%7D%3C/style%3E%3Ccircle class='a d f g' r='46.5' stroke-width='7'/%3E%3Ccircle class='a f g' r='49.5' stroke='%23555' stroke-width='1'/%3E%3Cpath class='a d' d='M42.5 44.535v20.93M62.464 44.5h-9.928M52.5 44.535v20.93M62.464 54.5h-9.928' stroke-width='5'/%3E%3Crect class='b e f' width='11' height='11' x='66' y='67' rx='3' ry='3' stroke-width='4'/%3E%3Crect class='a e f h' width='13' height='13' x='21' y='21' rx='7' ry='7'/%3E%3Cpath class='a e f h' d='M37.963 18.99a33.264 33.264 0 0 1 35.482 7.414 33.264 33.264 0 0 1 7.64 35.434' marker-end='url(%23a)'/%3E%3Cpath class='a e f h' d='M-61.835-82.277a33.264 33.264 0 0 1 35.274 7.67 33.264 33.264 0 0 1 7.512 35.306' transform='scale(-1)' marker-end='url(%23b)'/%3E%3Ccircle class='b f' cx='42.5' cy='37.5' r='2.5'/%3E%3C/svg%3E">)rawliteral";
// ********************************************************************** //
// ГЕНЕРАТОР ВСТАВКИ В БЛОК НАЙДЕННОГО УСТРОЙСТВА
static const char dev[] PROGMEM = R"rawliteral(
  <p class="bold">Режим работы</p>
)rawliteral";
// ********************************************************************** //
// ГЕНЕРАТОР ФАЙЛА description.xml
const char* ssdpTemplate =
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
    "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
    "</specVersion>"
    "<URLBase>http://%u.%u.%u.%u/</URLBase>"
    "<device>"
      "<deviceType>upnp:rootdevice</deviceType>"
      "<friendlyName>%s | %s</friendlyName>"
      "<manufacturer>%s</manufacturer>"
      "<manufacturerURL>%s</manufacturerURL>"
      "<modelDescription>UPnP Lighting Control</modelDescription>"
      "<modelName>%s</modelName>"
      "<modelNumber>%s</modelNumber>"
      "<modelURL>%s</modelURL>"
      "<serialNumber>%s</serialNumber>"
      "<UDN>uuid:edc3b25e-c2d0-411e-b04b-f9bd5e%02x%02x%02x</UDN>"
      "<presentationURL>/</presentationURL>"
    "</device>"
    "<iconList>"
      "<icon>"
        "<mimetype>image/png</mimetype>"
        "<height>48</height>"
        "<width>48</width>"
        "<depth>24</depth>"
        "<url>icon48.png</url>"
      "</icon>"
      "<icon>"
        "<mimetype>image/png</mimetype>"
        "<height>120</height>"
        "<width>120</width>"
        "<depth>24</depth>"
        "<url>icon120.png</url>"
      "</icon>"
    "</iconList>"
  "</root>\r\n"
  "\r\n";
// ********************************************************************** //
AsyncWebServer server(80);            // Инициализируем http сервер на порту 80
//static AsyncClient * aClient =  NULL; // Инициализируем асинхронный клиент
// ********************************************************************** //
void web_init() {

  //  server.on("/description.xml", HTTP_GET, [](AsyncWebServerRequest * request) {
  //    StreamString output;
  //    if (output.reserve(1024)) {
  //      if (!WiFi.getMode() == WIFI_STA)
  //        return;
  //      uint32_t ip = WiFi.localIP();
  //      uint32_t copChipID = chip_ID;
  //      output.printf(
  //        ssdpTemplate,
  //        IP2STR(&ip),                        // URLBase            (IP адрес устройства)
  //        DEVICE_MODEL, configDev.DevName,         // friendlyName       (SOI-WiFi | Пользовательское имя)
  //        "SOI Tech",                         // manufacturer
  //        "http://soi-tech.com",              // manufacturerURL
  //        DEVICE_MODEL,                       // modelName          (SOI-WiFi)
  //        DEVICE_ID,                          // modelNumber        (F9)
  //        DEVICE_URL,                         // modelURL           (http://soi-tech.com/production/F9)
  //        configDev.da,                       // serialNumber       (Адрес устройства в Радио сети)
  //        (uint8_t) ((copChipID >> 16) & 0xff),  // UDN
  //        (uint8_t) ((copChipID >>  8) & 0xff),  // UDN
  //        (uint8_t)   copChipID        & 0xff    // UDN EDC3B25E-C2D0-411E-B04B-F9BD5E%02x%02x%02x
  //      );
  //      request->send(200, "text/xml; charset=utf-8", (String)output);
  //    } else
  //      request->send(500);
  //  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(configDev.WebUser, configDev.WebPsw))
      return request->requestAuthentication();
    AsyncWebServerResponse *response;
    response = request->beginResponse_P(200, "text/html; charset=utf-8", index_html);
    response->addHeader("Server", "soi-tech");
    request->send(response);
  });

//    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
//      AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css; charset=utf-8", style_css_gz, style_css_gz_len);
//      response->addHeader("Content-Encoding", "gzip");
//      request->send(response);
//    });
//
//    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest * request) {
//      AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript; charset=utf-8", main_js_gz, main_js_gz_len);
//      response->addHeader("Content-Encoding", "gzip");
//      request->send(response);
//    });

  server.on("/icon", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/svg+xml; charset=utf-8", icon);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/dev", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html; charset=utf-8", dev);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  // ВЕРНУТЬ ДАННЫЕ КОНФИГУПАЦИИ УСТРОЙСТВА
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, CONFIG_FILE, "text/json; charset=utf-8");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  // УСТАНОВИТЬ ЯЗЫК ИНТЕРФЕЙСА И ВЕРНУТЬ СЛОВАРЬ
  server.on("/lang", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response;
    if (!request->hasArg("get") || !request->hasArg("set")) {
      return request->send(400, "text/plain; charset=utf-8", "Bad Request");
    }
    strncpy(configDev.WebLng, request->arg("get").c_str(), sizeof(configDev.WebLng) - 1);
    if (request->arg("set").equals("1")) {
      flagSaveConfig = true;
    }
    String path = "/dl/"; path += configDev.WebLng; path += ".json";
    response = request->beginResponse(SPIFFS, path, "text/json; charset=utf-8");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  // ПОЛУЧИТЬ API устройства
  server.on("/api", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/json; charset=utf-8", api);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  // ПОЛУЧИТЬ СЦЕНАРИИ
  server.on("/scr", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, SCRIPTS_FILE, "text/json; charset=utf-8");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  // Загрузка файла прошивки MCU в SPIFFS
//  server.on("/mcuFlash", HTTP_POST, [](AsyncWebServerRequest * request) {
//    request->send(200, "text/plain; charset=utf-8", "OK");
//  }, mcuFlashUpload);

  // Загрузка файла прошивки MCU в SPIFFS
//  server.on("/mcuEEPROM", HTTP_POST, [](AsyncWebServerRequest * request) {
//    request->send(200, "text/plain; charset=utf-8", "OK");
//  }, mcuEEPROMUpload);

  server.onNotFound([](AsyncWebServerRequest * request) {
    if (request->method() == HTTP_OPTIONS)
      request->send(200, "text/plain; charset=utf-8", "OK");
    else
      request->send(404, "text/plain; charset=utf-8", "Not Found");
  });
  server.serveStatic("/", SPIFFS, "/");   // Доступ к файловой системе
  server.begin();
}
// ********************************************************************** //
