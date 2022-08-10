/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates how to enable and detect RF field energy harvesting.
  
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

  Serial.println(F("Opening I2C session with password."));
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");
  Serial.println();
  
  Serial.println(F("Connect a digital multimeter between VEH(+) and GROUND(-) pins."));
  Serial.println(F("Whenever there's RF field a voltage will show up between VEH and GROUND."));
  Serial.println(F("Warning! Voltage will get higher than 3.3 volts so you may damage a 3.3V-only microcontroller!"));
  Serial.println();
  Serial.println(F("Enabling energy harvesting now."));
  
  tag.setEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_EN, true);
  tag.setEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_ON, true);
  
  Serial.print(F("Energy harvesting bit is "));
  Serial.println(tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_ON) ? "enabled." : "disabled.");
  Serial.print(F("VCC is "));
  Serial.println(tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_VCC_ON) ? "detected." : "not detected.");
  delay(1000);
}

void loop()
{
  if (tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_FIELD_ON))
    Serial.println(F("RF field detected."));
  else
    Serial.println(F("RF field not detected."));

  delay(250);
}
