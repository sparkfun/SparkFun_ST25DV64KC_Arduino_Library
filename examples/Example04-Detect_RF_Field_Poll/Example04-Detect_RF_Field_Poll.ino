/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates how to detect when an RF field is present.
  
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
  }
}

void loop()
{
  bool value = tag.RFFieldDetected();
  if (value)
    Serial.println("RF field detected.");
  else
    Serial.println("RF field not detected.");

  delay(250);
}
