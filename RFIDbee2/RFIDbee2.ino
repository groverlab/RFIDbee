// RFIDbee2.ino v1 by William H. Grover | wgrover@engr.ucr.edu | groverlab.org

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>  # from "Adafruit PN532" by Adafruit
#include "RTClib.h"  # from "RTClib" by Adafruit
#include <SdFat.h>  # from "SdFat" by Bill Greiman

#define STATUS_LED (7)   // was 3 in first board, keep it 7 from now on

#define sdChipSelect (10)

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

#define PN532_SS1 (4)
#define PN532_SS2 (6)

Adafruit_PN532 nfc1(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS1);
Adafruit_PN532 nfc2(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS2);

RTC_DS3231 rtc;

// Error codes:
// 1 blink:  reserved for OK startup
// 2 blinks:  RTC needs to be set
// 3 blinks:  Problem with RTC
// 4 blinks:  Problem with SD card

void blink(unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }
  delay(800);
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
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
  Serial.println("\nStarting up");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while(1) { blink(3); }
  }

  if (rtc.lostPower()) {
    Serial.println("Real-time clock needs to be set.");

    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    
    while (!Serial.available()) { blink(2); }

    // this is expecting a string like "21 12 31 8 0 0"

    year = Serial.parseInt();
    month = Serial.parseInt();
    day = Serial.parseInt();
    hour = Serial.parseInt();
    min = Serial.parseInt();
    sec = Serial.parseInt();

    rtc.adjust(DateTime(year, month, day, hour, min, sec));
    Serial.println("Time successfully set.");
  }

  Serial.println("RTC OK");
  
  nfc1.begin();
  nfc1.setPassiveActivationRetries(0x1);
  nfc1.SAMConfig();
  Serial.println("PN532 board 1 OK");

  nfc2.begin();
  nfc2.setPassiveActivationRetries(0x1);
  nfc2.SAMConfig();
  Serial.println("PN532 board 2 OK");

  // Initialize the SD and create or open the data file for append.
  if (!sd.begin(sdChipSelect) || !file.open("LOGFILE.TXT", O_CREAT | O_WRITE | O_APPEND)) {
    Serial.println(F("SD problem"));
    while(1) {
      blink(4);
    }
  }
  Serial.println("SD card OK");

  // Record startup in log file and console:
  DateTime now = rtc.now();
  char t[20] = "";  // 19 characters + null terminator = 20
  sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());
  Serial.print("The RTC thinks the GMT time is now ");
  Serial.print(t);
  Serial.println("\tStartup\tStartup");
  file.print(t);
  file.println("\tStartup\tStartup");
  file.sync();

   // If we make it this far, startup is successful; turn off Status LED:
  digitalWrite(STATUS_LED, LOW);
}

void loop(void) {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // no need for null terminator here
  uint8_t uidLength;


  // READER 1
  success = nfc1.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {

    digitalWrite(STATUS_LED, HIGH);

    DateTime now = rtc.now();
    char t[20] = "";  // 19 characters + null terminator = 20
    sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());

    char id[15] = ""; // max 7 nibbles = 14 characters + null terminator = 15
    array_to_string(uid, uidLength, id);

    Serial.print(t);
    Serial.print("\t1\t");
    Serial.println(id);

    file.print(t);
    file.print("\t1\t");
    file.println(id);
    file.sync();

    digitalWrite(STATUS_LED, LOW);

  }

  // READER 2
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
    Serial.print("\t2\t");
    Serial.println(id);

    file.print(t);
    file.print("\t2\t");
    file.println(id);
    file.sync();

    digitalWrite(STATUS_LED, LOW);

  }

}
