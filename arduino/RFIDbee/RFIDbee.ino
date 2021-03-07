// RFIDbee by William H. Grover - wgrover@engr.ucr.edu

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "RTClib.h"
//#include <SD.h>
#include <SdFat.h>

const uint8_t sdChipSelect = 10;

SdFat sd;
SdFile file;

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

RTC_DS3231 rtc;

int incomingByte = 0;

void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);

  nfc.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("Real-time clock needs to be set.");
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    Serial.println("Set year in two digits, like 21 for 2021:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    year = Serial.parseInt();
    Serial.println("Set month in two digits 1-12:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    month = Serial.parseInt();
    Serial.println("Set day in two digits 1-31:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    day = Serial.parseInt();
    Serial.println("Set hour in two digits 0-23:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    hour = Serial.parseInt();
    Serial.println("Set minute in two digits 0-59:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    min = Serial.parseInt();
    Serial.println("Set second in two digits 0-59:");
    Serial.read();
    while (!Serial.available()) { delay(10); }
    sec = Serial.parseInt();
    rtc.adjust(DateTime(year, month, day, hour, min, sec));
    Serial.println("Time successfully set.");
  }

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  nfc.SAMConfig();

  // Initialize the SD and create or open the data file for append.
  if (!sd.begin(sdChipSelect) || !file.open("LOGFILE.TXT", O_CREAT | O_WRITE | O_APPEND)) {
    // Replace this with somthing for your app.
    Serial.println(F("SD problem"));
    while(1);
  }

  // Record startup in log file and console:
  DateTime now = rtc.now();
  char t[20] = "";  // 19 characters + null terminator = 20
  sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());
  Serial.print(t);
  Serial.println("\tStartup");
  file.print(t);
  file.print("\t");
  file.println("Startup");
  file.sync();

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
}

void loop(void) {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // no need for null terminator here
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {

    digitalWrite(3, HIGH);

    DateTime now = rtc.now();
    char t[20] = "";  // 19 characters + null terminator = 20
    sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());

    char id[15] = ""; // max 7 nibbles = 14 characters + null terminator = 15
    array_to_string(uid, uidLength, id);

    Serial.print(t);
    Serial.print("\t");
    Serial.println(id);

    file.print(t);
    file.print("\t");
    file.println(id);
    file.sync();

    digitalWrite(3, LOW);

  }
}
