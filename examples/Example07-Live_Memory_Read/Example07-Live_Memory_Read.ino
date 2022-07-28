/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example reads and pretty-prints the first 256 bytes of the tags.
  You can use the ST "NFC Tap" App to write to the memory - and see the changes live!

  The example begins by setting the first 256 bytes to zero.
  
  Feel like supporting open source hardware?
  Buy a board from SparkFun!
  SparkFun Qwiic RFID Tag - ST25DV64KC : https://www.sparkfun.com/products/19035

  Hardware Connections:
  Plug a Qwiic cable into the Qwiic RFID Tag and a RedBoard
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 115200 baud to see the output
*/

#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC tag;

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Wire.begin();

  Serial.println("ST25DV64KC example.");

  if (tag.begin(Wire))
  {
    uint8_t values[8] = {0};
    Serial.println("ST25 connected.");
    Serial.print("Device UID: ");
    tag.getDeviceUID(values);
    for (uint8_t i = 0; i < 8; i++)
    {
      if (values[i] < 0x0a)
        Serial.print("0");
      Serial.print(values[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Revision: ");
    Serial.println(tag.getDeviceRevision());

    Serial.println("Opening I2C session with password.");
    uint8_t password[8] = {0x0};
    tag.openI2CSession(password);

    Serial.print("I2C session is ");
    Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

    // Allow writing to User Memory Area 1
    Serial.println("Unprotecting area 1 for write operation.");
    tag.programEEPROMWriteProtectionBit(1, false);

    Serial.print("EEPROM area 1 write protection: ");
    Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

    // Clear the first 256 bytes of user memory
    uint8_t tagWrite[256];
    memset(tagWrite, 0, 256);

    Serial.println("Writing 0x0 to the first 256 bytes of user memory using the opened session.");
    tag.writeEEPROM(0x0, tagWrite, 256);
  }
  else
  {
    Serial.println("ST25 not detected. Freezing...");
    while (1) // Do nothing more
      ;
  }
}

void loop()
{
  static uint8_t tagRead[256];
  
  tag.readEEPROM(0x0, tagRead, 256);

  prettyPrintChars(tagRead, 256);

  delay(1000);
}

void prettyPrintChars(uint8_t *theData, int theLength) // Pretty-print char data in HEX and ASCII format
{
  Serial.println();

  for (int i = 0; i < theLength; i += 16)
  {
    if (i < 10000) Serial.print(F("0"));
    if (i < 1000) Serial.print(F("0"));
    if (i < 100) Serial.print(F("0"));
    if (i < 10) Serial.print(F("0"));
    Serial.print(i);

    Serial.print(F(" 0x"));

    if (i < 0x1000) Serial.print(F("0"));
    if (i < 0x100) Serial.print(F("0"));
    if (i < 0x10) Serial.print(F("0"));
    Serial.print(i, HEX);

    Serial.print(F(" "));

    int j;
    for (j = 0; ((i + j) < theLength) && (j < 16); j++)
    {
      if (theData[i + j] < 0x10) Serial.print(F("0"));
      Serial.print(theData[i + j], HEX);
      Serial.print(F(" "));
    }

    if (((i + j) == theLength) && (j < 16))
    {
      for (int k = 0; k < (16 - (theLength % 16)); k++)
      {
        Serial.print(F("   "));
      }
    }
      
    for (j = 0; ((i + j) < theLength) && (j < 16); j++)
    {
      if ((theData[i + j] >= 0x20) && (theData[i + j] <= 0x7E))
        Serial.write(theData[i + j]);
      else
        Serial.print(F("."));
    }

    Serial.println();
  }

  Serial.println();
}
