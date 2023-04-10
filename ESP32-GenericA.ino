
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Ticker.h>
#include "WiFi.h"
#include "FS.h"
#include "SPIFFS.h"
// ********************************************************************** //
#define DEVICE_CATEGORY "F"  // Категория
#define DEVICE_TYPE "F"      // Тип
#define DEVICE_MODEL "5"     // Модель
#define DEVICE_ID DEVICE_CATEGORY DEVICE_TYPE DEVICE_MODEL
#define DEVICE_FW_VERSION "00.01"  // Версия прошивки
#define DEVICE_URL "http://..../" DEVICE_CATEGORY DEVICE_TYPE DEVICE_MODEL
// ********************************************************************** //
#define CONFIG_FILE "/config.json"               // Путь к файлу настроек
#define SCRIPTS_FILE "/scripts.json"             // Путь к файлу сценариев
bool flagSaveConfig = false;                     // Флаг сохранения параметров в файл
#define OTA_HOST "updates.soi-tech.com"          // Адрес сервера обновлений
#define OTA_PATH "/" DEVICE_ID ".txt"            // Путь id.txt на сервере (версия и MD5 файлов прошивки)
#define OTA_PATH_FLASH "/" DEVICE_ID "-MF.bin"   // Путь id-MF.bin на сервере (прошивка Flash MCU)
#define OTA_PATH_EEPROM "/" DEVICE_ID "-EF.bin"  // Путь id-ME.bin на сервере (прошивка EEPROM MCU)
// ********************************************************************** //
#define STATUS_LED_PIN 5
//#define MCU_RESET_PIN 6
//#define MCU_BOOT_PIN 7
#define LED_FRESH_TIME 10
bool flagWConnect = false;  // Проверка состояния подключения

String devsList = "";  // Итоговый список устройств в сети

// ДИСПЕТЧЕР ЗАДАЧ
Ticker wifiTicker;  // Проверка подключения к сети
Ticker udpTicker;   // Сканирования устройств в сети
Ticker waitScan;    // Задержки при сканировании сети (между широковещательным запросом и завершением прослушивания)

void setup() {
  delay(2500);
  /* Инициализация UART */
  Serial.begin(115200);
  uart_send("SYS", "UART", "[{\"Status\":\"OK!\"}]");

  /* Монтаж файловой системы */
  fs_init() ? uart_send("SYS", "FS", "[{\"Status\":\"OK!\"}]") : uart_send("SYS", "FS", "[{\"Status\":\"nOK!\"}]");

  /* Загрузка параметров */
  config_init(0) ? uart_send("SYS", "Config", "[{\"Status\":\"OK!\"}]") : uart_send("SYS", "Config", "[{\"Status\":\"nOK!\"}]");

  /* Инициализация WiFi */
  wifi_init() ? uart_send("SYS", "WiFi", "[{\"Status\":\"OK!\"}]") : uart_send("SYS", "WiFi", "[{\"Status\":\"nOK!\"}]");

  /* Инициализация UDP */
  udp_init() ? uart_send("SYS", "UDP", "[{\"Status\":\"OK!\"}]") : uart_send("SYS", "UDP", "[{\"Status\":\"nOK!\"}]");

  // Инициализация Web сервера
  //  web_init();

  // Инициализация WS сервера
  //  ws_init() ? uart_send("SYS", "WS", "[{\"Status\":\"OK!\"}]") : uart_send("SYS", "WS", "[{\"Status\":\"nOK!\"}]");

  pinMode(STATUS_LED_PIN, OUTPUT);  // Пин светодиода СТАТУС на Выход
  //  pinMode(MCU_RESET_PIN, OUTPUT);               // Пин сброса MCU на Выход
  //  pinMode(MCU_BOOT_PIN, OUTPUT);               // Пин режима загрузки MCU на Выход

  //  xTaskCreatePinnedToCore(RND, "RND", 2048, NULL, 1, NULL, tskNO_AFFINITY);

  wifiTicker.attach(15, checkConnectFlag);  // Проверка статуса подключения каждые 15 сек.
}

void loop() {
  uart_handler();
  config_handler();

  checkConnect_handler();
}

void RND(void *pvParameters) {
  int randNumber;
  char number[15];
  while (1) {
    randNumber = random(0, 1000000000);
    itoa(randNumber, number, 10);
    uart_send("SYS", "Rnd", number);
    vTaskDelay(10000);
  }
}
