
// UART
char inChar;                // Текущий считанный Байт из буфера UART
char uartData[1024] =  "";  // Буфер хранения данных для UART
// ********************************************************************** //
// СЛУЖЕБНЫЕ СИМВОЛЫ ФОРМИРОВАНИЯ ПАКЕТОВ
#define SOH           0x01  // Начало пакета
#define US            0x1F  // Разделитель заголовка и аргумента в пакете
#define STX           0x02  // Начало данных в пакете
#define ETX           0x03  // Конец данных в пакете
#define EOT           0x04  // Конец пакета
// ********************************************************************** //
// ПЕРЕДАЧА ПАКЕТА UART
void uart_send(const char * header, const char * argument, const char * data_packet) {
  Serial.printf( "%c%s%c%s%c%s%c%02X%c\r\n", SOH, header, US, argument, STX, data_packet, ETX, get_crc_8(data_packet), EOT );
  //  Serial.printf( "%c%s%c%s%c%s%c%02X%c\r\n", SOH, header, US, argument, STX, data_packet, ETX, get_crc_16(data_packet), EOT );
}
// ********************************************************************** //
// ОБРАБОТКА ПРИНЯТОГО ПАКЕТА ПО UART
void uart_handler() {
  if (Serial.available()) {
    /* Если есть непрочитанные данные в буфере, то читаем очередной символ */
    inChar = (char)Serial.read();
    int uartData_len = strlen(uartData);
    int uartData_max = sizeof(uartData) - 1;
    if (uartData_len < uartData_max) {
      /* Если буфер не переполнен, то добавляем очередной считанный байт */
      uartData[uartData_len] = inChar;
      uartData[uartData_len + 1] = '\0';
    } else {
      /* В противном случае возвращаем ошибку о переполнении буфера */
      uart_send("ER!", "Buffer", "[{\"Status\":\"Overflow\"}]");
      uartData[0] = '\0';
    }
    if (inChar == EOT) {
      /* Если пришел символ конца пакеты - обрабатываем */
      if (uartData[0] != SOH) {
        /* Если пакет начинается не с символа SOH - ошибка */
        uart_send("ER!", "Package", "[{\"Status\":\"Start SOH\"}]");
        uartData[0] = '\0';
        return;
      }
      /* Читаем заголовок и сохраняем в header */
      char header[strchr(uartData + 1, US) - uartData];
      strncpy(header, uartData + 1, sizeof(header) - 1);
      header[sizeof(header) - 1] = '\0';
      /* Читаем аргумент и копируем в argument */
      char argument[strchr(uartData + 1 + sizeof(header), STX) - uartData - sizeof(header)];
      strncpy(argument, uartData + sizeof(header) + 1, sizeof(argument) - 1);
      argument[sizeof(argument) - 1] = '\0';
      /* Читаем данные и копируем в body */
      int body_len = strchr(uartData, ETX) - strchr(uartData, STX) - 1;
      char * body = (char*)malloc(body_len + 1);
      strncpy(body, strchr(uartData, STX) + 1, body_len);
      body[body_len] = '\0';

      char crc = 0x00, hex[3];
      strncpy(hex, strchr(uartData, ETX) + 1, 2);
      hex[2] = '\0';
      crc = (char)strtol(hex, NULL, 16);
      /* Сравниваем полученную CRC с расчетной */
      if (get_crc_8(body) != crc) {
        uart_send("ER!", "CRC", "[{\"Status\":\"No match\"}]");
        free(body);
        uartData[0] = '\0';
        return;
      }
      /* Обрабатываем зоголовки */
      uart_send(header, argument, body);
      if (!strcmp(header, "SYS"))
        header_sys(argument, body);
      else if (!strcmp(header, "GET"))
        header_get(argument, body);
      else if (!strcmp(header, "SET"))
        header_set(argument, body);
      else if (!strcmp(header, "OK!"))
        header_ok(argument, body);
      else if (!strcmp(header, "ER!"))
        header_er(argument, body);
      else
        uart_send("ER!", "Header", "[{\"Status\":\"Doesn't exist\"}]");
      free(body);
      uartData[0] = '\0';
    }
  }
}

void header_sys(const char * argument, const char * data_packet) {

}

void header_get(const char * argument, const char * data_packet) {
  if (!strcmp(argument, "devsList")) {
    uart_send("GET", argument, devsList.c_str());
  }
}

void header_set(const char * argument, const char * data_packet) {

}

void header_ok(const char * argument, const char * data_packet) {

}

void header_er(const char * argument, const char * data_packet) {

}
