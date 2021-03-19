// RFIDbee2 by William H. Grover - wgrover@engr.ucr.edu

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "RTClib.h"
#include <SdFat.h>

#define STATUS_LED (7)   // was 3 in earlier version, keep it 7 for new version;

const uint8_t sdChipSelect = 10;
SdFat sd;
SdFile file;



// ORIGINAL ADAFRUIT CODE FOR RFID READERS:
#define PN532_SCK  (2)  
#define PN532_MOSI (3)  
#define PN532_MISO (5)  

// STANDARD ARDUINO SPI PINS:
//#define PN532_SCK  (13) 
//#define PN532_MOSI (11)  
//#define PN532_MISO (12)  



#define PN532_SS   (4)
#define PN532_SS2  (6)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS2);

RTC_DS3231 rtc;

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

  pinMode(STATUS_LED, OUTPUT);

  // Turn on Status LED to show that startup is underway:
  digitalWrite(STATUS_LED, HIGH);
  
  Serial.begin(9600);
  while (!Serial) delay(10);



//  pinMode(PN532_SS, OUTPUT);
//  digitalWrite(PN532_SS, HIGH);
//  pinMode(PN532_SS2, OUTPUT);
//  digitalWrite(PN532_SS2, HIGH);
//  pinMode(sdChipSelect, OUTPUT);
//  digitalWrite(sdChipSelect, HIGH);









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
    Serial.print("Didn't find PN53x board A");
    while (1);
  }
  Serial.println("Found chip PN5 board A ");
  nfc.setPassiveActivationRetries(0x1);
  nfc.SAMConfig();

  delay(1000);

  nfc2.begin();
  uint32_t versiondata2 = nfc2.getFirmwareVersion();
  if (! versiondata2) {
    Serial.println("Didn't find PN53x board B");
    while (1);
  }
  Serial.println("Found chip PN5 board B ");
  nfc2.setPassiveActivationRetries(0x1);
  nfc2.SAMConfig();

  delay(1000);






  // Initialize the SD and create or open the data file for append.
  if (!sd.begin(sdChipSelect) || !file.open("LOGFILE.TXT", O_CREAT | O_WRITE | O_APPEND)) {
    Serial.println(F("SD problem"));
    while(1);
  }
  Serial.println("CARD OK");

  delay(1000);





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

   // If we make it this far, startup is successful; turn off Status LED:
  digitalWrite(STATUS_LED, LOW);
}

void loop(void) {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // no need for null terminator here
  uint8_t uidLength;


  // READER A
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {

    digitalWrite(STATUS_LED, HIGH);

    DateTime now = rtc.now();
    char t[20] = "";  // 19 characters + null terminator = 20
    sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());

    char id[15] = ""; // max 7 nibbles = 14 characters + null terminator = 15
    array_to_string(uid, uidLength, id);

    Serial.print(t);
    Serial.print("\t");
    Serial.print("A: ");
    Serial.println(id);

    file.print(t);
    file.print("\t");
    file.print("A\t");
    file.println(id);
    file.sync();

    digitalWrite(STATUS_LED, LOW);

  }


  // READER B
  success = nfc2.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {

    digitalWrite(STATUS_LED, HIGH);

    DateTime now = rtc.now();
    char t[20] = "";  // 19 characters + null terminator = 20
    sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());

    char id[15] = ""; // max 7 nibbles = 14 characters + null terminator = 15
    array_to_string(uid, uidLength, id);

    Serial.print(t);
    Serial.print("\t");
    Serial.print("B:    ");
    Serial.println(id);

    file.print(t);
    file.print("\t");
    file.print("B\t");
    file.println(id);
    file.sync();

    digitalWrite(STATUS_LED, LOW);

  }

  
}
