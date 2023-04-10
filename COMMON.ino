
// ============================================================ //
// Name  : CRC-8/MAXIM
// Poly:  0x31  | Init:   0x00
// RefIn: true  | RefOut: true   | XorOut: 0x00
// Check: 0x75  | 0123456789
byte get_crc_8(const char * Data) {
  unsigned int len = strlen(Data);
  byte crc = 0x00;
  while (len--) {
    byte extract = *Data++;
    for (byte i = 8; i; i--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
        crc ^= 0x8C;
      extract >>= 1;
    }
  }
  return crc;
}
// ************************************************************ //
// Name  : CRC-16/AUG-CCITT
// Poly:  0x1021  | Init:   0x1D0F
// RefIn: false   | RefOut: false   | XorOut: 0x0000
// Check: 0x18A1  | 0123456789
unsigned short get_crc_16(const char * Data) {
  unsigned int len = strlen(Data);
  unsigned short crc = 0x1D0F;
  unsigned char i;
  while (len--) {
    crc ^= *Data++ << 8;
    for (i = 0; i < 8; i++)
      crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
  }
  return crc;
}
// ************************************************************ //
// Серийный номер, генерируется автоматически (пример 07C9E-6BD0E3C-ACA00)
char * get_device_SN() {
  char * buff = (char*)malloc(20);
  uint8_t bufMac[6];
  WiFi.macAddress(bufMac);
  sprintf (buff, "%s%02X%02X-6%02X%s%02XC-%02X%02X%s",
           DEVICE_CATEGORY, bufMac[0], bufMac[1],
           bufMac[2], DEVICE_TYPE, bufMac[3],
           bufMac[4], bufMac[5], DEVICE_MODEL);
  return buff;
}
// ************************************************************ //
// Валидация серийного номера устройства
bool validDeviceSN(const char * devSN) {
  if (devSN[6] == '6' && devSN[12] == 'C') {
    return true;
  } else return false;
}
// ************************************************************ //
