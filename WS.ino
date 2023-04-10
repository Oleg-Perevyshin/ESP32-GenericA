
AsyncWebSocket ws("/ws");           // Инициализируем WebSocket Server (ws://ip/ws)
AsyncWebSocketClient * wsClient;    // Инициализируем WebSocket Client

bool ws_init() {
  server.addHandler(&ws);       // Асинхронный WebSocket
  ws.onEvent(onWsEvent);
  return true;
}
// ********************************************************************** //
// ОБРАБОТЧИК СОБЫТИЙ WEBSOCKET
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {                       // КЛИЕНТ ПОДКЛЮЧЕН
    wsClient = client;
    //    client->ping();                                   // Поддержание соединения (если закомм. предыдущий сокет отключается)
    //sendWS(NULL, "infoMessage", "WS - Connect");
  } else if (type == WS_EVT_DISCONNECT) {             // КЛИЕНТ ОТКЛЮЧЕН
    wsClient = nullptr;
  } else if (type == WS_EVT_ERROR) {                  // ОБРАБОТКА ОШИБОК
    void();
  } else if (type == WS_EVT_PONG) {                   // ПОЛУЧЕНО СООБЩЕНИЕ PONG (возможно, в ответ на запрос ping)
    void();
  } else if (type == WS_EVT_DATA) {                   // ОБРАБОТКА ВХОДЯЩИХ ПАКЕТОВ WEBSOCKET
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, (char*)data);   // Парсим пакет
    if (error) {
      uart_send("ER!", "WS-Data", "Parser");
      doc.clear();
      return;
    }
    JsonObject root = doc.as<JsonObject>();
//    if (root.containsKey("ActualInfo")) {         // АКТУАЛЬНЫЕ ПАРАМЕТРЫ УСТРОЙСТВА
//      DynamicJsonDocument doc(4096);
//      JsonObject json = doc.to<JsonObject>();
//      // Общие параметры
//      json["actID"] = DEVICE_ID;
//
//      String actualInfo;
//      serializeJson(json, actualInfo);
//      //sendWS(NULL, "ActualInfo", actualInfo);
//
//    } else if (root.containsKey("NetList")) {     // СПИСОК ДОСТУПНЫХ ТОЧЕК ДОСТУПА
//      int n = WiFi.scanNetworks();
//      printListAPs(n);
//    } else {                                      // НЕВЕРНЫЕ ДАННЫЕ (ЗАГОЛОВОК ПАКЕТА НЕ ОПРЕДЕЛЕН)
//      uart_send("ER!", "WS", "Unknown Data");
//      //sendWS(NULL, "infoMessage", "WS - Unknown Data");
//    }
    doc.clear();
  }
}
// ********************************************************************** //
// ПЕРЕДАЧА ДАННЫХ В WEBSOCKET
// Передаем объект: { argument: data }
void sendWS(AsyncWebSocketClient *client, const char *argument, String value) {
  DynamicJsonDocument doc(2048);
  JsonObject root = doc.to<JsonObject>();
  root[argument] = value;
  size_t len = measureJson(doc);
//  AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len);
//  if (buffer) {
//    serializeJson(root, (char*)buffer->get(), len + 1);
//    ws.textAll(buffer);
//  }
//  wsClient->text(buffer);
  doc.clear();
}
