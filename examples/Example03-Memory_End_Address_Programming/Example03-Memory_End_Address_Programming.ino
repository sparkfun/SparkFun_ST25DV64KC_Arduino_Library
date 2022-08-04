/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates how to change the size of the Tag's EEPROM User Memory partitions.
  The user memory can be divided up into (up to) four areas.
  Area 1 is always readable - but can be write-protected.
  This example shows how to re-size Area 1.
  
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

  Serial.println(F("Opening I2C session with password."));
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  char endAddress[5] = {0};

  Serial.println();

  Serial.print(F("Area 1 ending address before: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
  Serial.println(endAddress);

  // This function will succeed if ENDA1 value is less than or equal to ENDA2 and ENDA3.
  // Further information can be found on the datasheet on page 14.
  // Memory address will be: 0x20 * ENDAi + 0x1f which in this case results
  // in  0x20 * 0x10 + 0x1f = 0x021f
  if (tag.setMemoryAreaEndAddress(1, 0x10))
  {
    Serial.println(F("Success setting Area 1 end memory address."));
  }
  else
  {
    Serial.println(F("Error setting Area 1 end memory address."));
  }
  memset(endAddress, 0, 5);
  Serial.print(F("Area 1 ending address after: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
  Serial.println(endAddress);

  Serial.println();

  memset(endAddress, 0, 5);
  Serial.print(F("Area 2 ending address before: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(2));
  Serial.println(endAddress);

  // This function will fail since ENDA2 value is less than if ENDA1 programmed above.
  // Further information can be found on the datasheet on page 14.
  // Memory address will be: 0x20 * ENDAi + 0x1f which in this case results
  // in 0x20 * 0x03 + 0x1f = 0x007f
  if (tag.setMemoryAreaEndAddress(2, 0x03))
  {
    Serial.println(F("Success setting Area 2 end memory address."));
  }
  else
  {
    Serial.println(F("Error setting Area 2 end memory address."));
  }

  memset(endAddress, 0, 5);
  Serial.print(F("Area 2 ending address after: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(2));
  Serial.println(endAddress);

  Serial.println();

  memset(endAddress, 0, 5);
  Serial.print(F("Area 1 ending address before: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
  Serial.println(endAddress);

  // Restore ENDA1
  // This function will succeed if ENDA1 value is less than or equal to ENDA2 and ENDA3.
  // Further information can be found on the datasheet on page 14.
  // Memory address will be: 0x20 * ENDAi + 0x1f which in this case results
  // in  0x20 * 0xff + 0x1f = 0x1fff
  if (tag.setMemoryAreaEndAddress(1, 0xFF))
  {
    Serial.println(F("Success setting Area 1 end memory address."));
  }
  else
  {
    Serial.println(F("Error setting Area 1 end memory address."));
  }
  memset(endAddress, 0, 5);
  Serial.print(F("Area 1 ending address after: 0x"));
  sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
  Serial.println(endAddress);
}

void loop()
{
  // Nothing to do here
}
