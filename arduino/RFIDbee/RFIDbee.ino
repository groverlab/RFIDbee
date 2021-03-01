#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "RTClib.h"
#include <SD.h>

File myFile;

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

RTC_DS3231 rtc;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10); // for Leonardo/Micro/Zero

  Serial.println("Hello!");

  nfc.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }


  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);


  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {

    digitalWrite(3, HIGH);

    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    myFile = SD.open("log.txt", FILE_WRITE);
    myFile.println("I read a chip!");
    Serial.println("I wrote to the file!");
    myFile.close();
    
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
//    if (uidLength == 4)
//    {
//      // We probably have a Mifare Classic card ... 
//      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
//    
//      // Now we need to try to authenticate it for read/write access
//      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
//      Serial.println("Trying to authenticate block 4 with default KEYA value");
//      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
//    
//    // Start with block 4 (the first block of sector 1) since sector 0
//    // contains the manufacturer data and it's probably better just
//    // to leave it alone unless you know what you're doing
//      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
//    
//      if (success)
//      {
//        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
//        uint8_t data[16];
//    
//        // If you want to write something to block 4 to test with, uncomment
//    // the following line and this text should be read back in a minute
//        //memcpy(data, (const uint8_t[]){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
//        // success = nfc.mifareclassic_WriteDataBlock (4, data);
//
//        // Try to read the contents of block 4
//        success = nfc.mifareclassic_ReadDataBlock(4, data);
//    
//        if (success)
//        {
//          // Data seems to have been read ... spit it out
//          Serial.println("Reading Block 4:");
//          nfc.PrintHexChar(data, 16);
//          Serial.println("");
//      
//          // Wait a bit before reading the card again
//          delay(1000);
//        }
//        else
//        {
//          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
//        }
//      }
//      else
//      {
//        Serial.println("Ooops ... authentication failed: Try another key?");
//      }
//    }
//    
//    if (uidLength == 7)
//    {
//      // We probably have a Mifare Ultralight card ...
//      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
//    
//      // Try to read the first general-purpose user page (#4)
//      Serial.println("Reading page 4");
//      uint8_t data[32];
//      success = nfc.mifareultralight_ReadPage (4, data);
//      if (success)
//      {
//        // Data seems to have been read ... spit it out
//        nfc.PrintHexChar(data, 4);
//        Serial.println("");
//    
//        // Wait a bit before reading the card again
//        delay(1000);
//      }
//      else
//      {
//        Serial.println("Ooops ... unable to read the requested page!?");
//      }
//    }

    digitalWrite(3, LOW);

  }
}
