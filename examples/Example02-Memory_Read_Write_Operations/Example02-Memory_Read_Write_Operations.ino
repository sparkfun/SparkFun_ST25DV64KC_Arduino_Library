/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates how to write data to Area 1 of the Tag's EEPROM User Memory.
  The user memory can be divided up into (up to) four areas.
  Area 1 is always readable - but can be write-protected.
  
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

  Serial.println(F("Opening I2C security session with default password."));
  uint8_t password[8] = {0x0}; // Default password is all zeros
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("EEPROM area 1 will always be readable (datasheet page 60)."));
  Serial.print(F("EEPROM area 1 write protection: "));
  Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

  Serial.println(F("Protecting area 1 for write operation."));
  tag.programEEPROMWriteProtectionBit(1, true);

  Serial.print(F("EEPROM area 1 read protection: "));
  Serial.println(tag.getEEPROMReadProtectionBit(1) ? "protected." : "opened.");
  bool area1WriteProtected = tag.getEEPROMWriteProtectionBit(1);
  Serial.print(F("EEPROM area 1 write protection: "));
  Serial.println(area1WriteProtected ? "protected." : "opened.");
  if (area1WriteProtected)
    Serial.println(F("Writing can only take place if I2C security session is opened."));

  uint8_t tagRead[16] = {0};
  Serial.print(F("Reading values starting on 0x0 with opened security session: "));
  tag.readEEPROM(0x0, tagRead, 16);
  for (auto value : tagRead)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  // Write 16 bytes from EEPROM location 0x0
  uint8_t tagWrite[16] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};

  Serial.print(F("Writing values starting on 0x0 with opened security session: "));
  for (auto value : tagWrite)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();
  tag.writeEEPROM(0x0, tagWrite, 16);

  memset(tagRead, 0, 16);
  Serial.print(F("Reading values starting on 0x0 with opened security session: "));
  tag.readEEPROM(0x0, tagRead, 16);
  for (auto value : tagRead)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  Serial.println(F("Closing I2C session - by writing the wrong password."));
  password[1] = {0x10}; // Change one byte of the password
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  // Try to write 16 random bytes from EEPROM location 0x0
  randomSeed(analogRead(A0));
  for (uint8_t i = 0; i < 16; i++)
    tagWrite[i] = (uint8_t)random(0, 0xff);

  Serial.print(F("Writing values starting on 0x0 with closed session: "));
  for (auto value : tagWrite)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();
  tag.writeEEPROM(0x0, tagWrite, 16);

  Serial.print(F("Reading values starting on 0x0 with closed session: "));
  memset(tagRead, 0, 16);
  tag.readEEPROM(0x0, tagRead, 16);
  for (auto value : tagRead)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  Serial.println(F("Opening I2C session."));
  password[1] = {0x0}; // Reset the password
  tag.openI2CSession(password);
  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Changing the I2C password."));
  uint8_t newPassword[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
  tag.writeI2CPassword(newPassword);
  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Entering the default password."));
  tag.openI2CSession(password);
  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Entering the new password."));
  tag.openI2CSession(newPassword);
  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Changing the I2C password back to the default."));
  tag.writeI2CPassword(password);
  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Unprotecting area 1 for write operation."));
  tag.programEEPROMWriteProtectionBit(1, false);

  Serial.print(F("EEPROM area 1 write protection: "));
  Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

  Serial.println(F("Closing I2C session - by writing the wrong password."));
  password[1] = {0x10}; // Change one byte of the password
  tag.openI2CSession(password);

  Serial.print(F("Writing zeros starting on 0x0 with closed session: "));
  memset(tagWrite, 0, 16);
  for (auto value : tagWrite)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();
  tag.writeEEPROM(0x0, tagWrite, 16);
  
  Serial.print(F("Reading values starting on 0x0 with closed session: "));
  memset(tagRead, 0, 16);
  tag.readEEPROM(0x0, tagRead, 16);
  for (auto value : tagRead)
  {
    Serial.print(F("0x"));
    if (value < 0x10)
      Serial.print(F("0"));
    Serial.print(value, HEX);
    Serial.print(F(" "));
  }
  Serial.println();
}

void loop()
{
  // Nothing to do here
}
