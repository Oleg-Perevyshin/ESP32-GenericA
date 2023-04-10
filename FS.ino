
//void readFile(const char * path) {
//  File file = SPIFFS.open(path);
//  if (!file || file.isDirectory()) {
//    return; // failed to open file for reading
//  }
//  while (file.available()) {
//    Serial.write(file.read());
//  }
//  file.close();
//}

// Прочитать файл, вернуть содержимое или NULL
char * readFile(const char * path) {
  size_t capacity = 16, length = 0;
  int ch;
  char * buff;
  File file = SPIFFS.open(path, FILE_READ);
  if (file != NULL) {
    buff = (char*)malloc(capacity);
    if (buff == NULL) {
      uart_send("SYS", "FS", "[{\"ReadFile\":\"Unable to Allocate Buffer\"}]");
      return buff = NULL;
    }
    while ((ch = file.read()) != EOF) {
      if (length >= capacity) {
        buff = (char*)realloc(buff, capacity += capacity);
        if (buff == NULL) {
          uart_send("SYS", "FS", "[{\"ReadFile\":\"Unable to Reallocate Buffer\"}]");
          break;
        }
      }
      buff[length++] = ch;
    }
    buff[length] = 0;
    file.close();
    return buff;
  } else {
    return buff = NULL;
  }
}

bool writeFile(const char * path, const char * message) {
  File file = SPIFFS.open(path, FILE_WRITE);
  if (file && file.print(message)) {
    file.close();
    return true;
  } else {
    uart_send("SYS", "FS", "[{\"WriteFile\":\"ER!\"}]");
    return false;
  }
}

bool appendFile(const char * path, const char * message) {
  File file = SPIFFS.open(path, FILE_APPEND);
  if (file && file.print(message)) {
    file.close();
    return true;
  } else {
    uart_send("SYS", "FS", "[{\"AppendFile\":\"ER!\"}]");
    return false;
  }
}

bool renameFile(const char * path1, const char * path2) {
  if (SPIFFS.rename(path1, path2)) {
    return true;
  } else {
    uart_send("SYS", "FS", "[{\"RenameFile\":\"ER!\"}]");
    return false;
  }
}

bool deleteFile(const char * path) {
  if (SPIFFS.remove(path)) {
    return true;
  } else {
    uart_send("SYS", "FS", "[{\"DeleteFile\":\"ER!\"}]");
    return false;
  }
}

bool fs_init() {
  if (SPIFFS.begin()) {
    return true;
  } else if (SPIFFS.format()) {
    return true;
  } else {
    return false;
  }
}
