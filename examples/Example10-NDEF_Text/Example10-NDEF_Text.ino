/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example shows how to set up the ST25DV64KC's Capability Container (CC)
  and create a NDEF UTF-8 Text record

  Feel like supporting open source hardware?
  Buy a board from SparkFun!
  SparkFun Qwiic RFID Tag - ST25DV64KC : https://www.sparkfun.com/products/19035

  Hardware Connections:
  Plug a Qwiic cable into the Qwiic RFID Tag and a RedBoard
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 115200 baud to see the output
*/

#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC_NDEF tag;

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Wire.begin();

  Serial.println(F("ST25DV64KC example."));

  if (!tag.begin(Wire))
  {
    Serial.println(F("ST25 not detected. Freezing..."));
    while (1) // Do nothing more
      ;
  }

  Serial.println(F("ST25 connected."));

  uint8_t values[8] = {0};
  if (tag.getDeviceUID(values))
  {
    Serial.print(F("Device UID: "));
    for (uint8_t i = 0; i < 8; i++)
    {
      if (values[i] < 0x0a)
        Serial.print(F("0"));
      Serial.print(values[i], HEX);
      Serial.print(F(" "));
    }
    Serial.println();
  }
  else
    Serial.println(F("Could not read device UID!"));

  uint8_t rev;
  if (tag.getDeviceRevision(&rev))
  {
    Serial.print(F("Revision: "));
    Serial.println(rev);
  }
  else
    Serial.println(F("Could not read device revision!"));

  Serial.println("Opening I2C session with password.");
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  // Allow writing to User Memory Area 1
  Serial.println(F("Unprotecting area 1 for write operation."));
  tag.programEEPROMWriteProtectionBit(1, false);

  Serial.print(F("EEPROM area 1 write protection: "));
  Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

  // -=-=-=-=-=-=-=-=-

  // Clear the first 512 bytes of user memory
  uint8_t tagWrite[512];
  memset(tagWrite, 0, 512);

  Serial.println("Writing 0x0 to the first 512 bytes of user memory using the opened session.");
  tag.writeEEPROM(0x0, tagWrite, 512);

  // -=-=-=-=-=-=-=-=-

  // Write the Type 5 CC File - starting at address zero
  Serial.println(F("Writing CC_File"));
  tag.writeCCFile8Byte();

  // -=-=-=-=-=-=-=-=-

  // Write two NDEF UTF-8 Text records
  uint16_t memLoc = tag.getCCFileLen();

  Serial.println(F("Writing the first NDEF Text record"));
  tag.writeNDEFText("Hello, World!", &memLoc, true, false); // MB=1, ME=0
  
  Serial.println(F("Writing the second NDEF Text record"));
  tag.writeNDEFText("All your base are belong to us", &memLoc, false, true); // MB=0, ME=1

  // -=-=-=-=-=-=-=-=-

  // Read back the second NDEF UTF-8 Text record
  Serial.println(F("Reading the second NDEF Text record:"));
  char theText[64];
  if (tag.readNDEFText(theText, 64, 2))
    Serial.println(theText);
  else
    Serial.println(F("Read failed!"));

  // -=-=-=-=-=-=-=-=-

  // Read back the memory contents
  Serial.println(F("The first 512 bytes of user memory are:"));
  uint8_t tagRead[512];
  tag.readEEPROM(0x0, tagRead, 512);
  prettyPrintChars(tagRead, 512);
}

void loop()
{
  // Nothing to do here
}

void prettyPrintChars(uint8_t *theData, int theLength) // Pretty-print char data in HEX and ASCII format
{
  Serial.println();

  for (int i = 0; i < theLength; i += 16)
  {
    if (i < 10000)
      Serial.print(F("0"));
    if (i < 1000)
      Serial.print(F("0"));
    if (i < 100)
      Serial.print(F("0"));
    if (i < 10)
      Serial.print(F("0"));
    Serial.print(i);

    Serial.print(F(" 0x"));

    if (i < 0x1000)
      Serial.print(F("0"));
    if (i < 0x100)
      Serial.print(F("0"));
    if (i < 0x10)
      Serial.print(F("0"));
    Serial.print(i, HEX);

    Serial.print(F(" "));

    int j;
    for (j = 0; ((i + j) < theLength) && (j < 16); j++)
    {
      if (theData[i + j] < 0x10)
        Serial.print(F("0"));
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
