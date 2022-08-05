/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: July, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example demonstrates how to generate a GPO interrupt when the RF field changes.
  
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

// Use a jumper cable to link the ST25DV64KC GPO1 pin to a digital pin
const uint8_t GPO_PIN = 2; // Change this to match the digital pin you have linked GPO1 to

static volatile bool interruptChanged = false;

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Wire.begin();

  Serial.println(F("ST25DV64KC example."));

  pinMode(GPO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(GPO_PIN), ISR, CHANGE);

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
  
  Serial.println(F("Opening I2C session with password."));
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  Serial.println(F("Configuring GPO1 to toggle on field change only."));
  tag.setGPO1Bit(BIT_GPO1_FIELD_CHANGE_EN, true);
  Serial.println(F("Disabling other bits."));
  tag.setGPO1Bit(BIT_GPO1_RF_USER_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_ACTIVITY_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_INTERRUPT_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_PUT_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_GET_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_WRITE_EN, false);
  Serial.println(F("Enabling GPO_EN bit."));
  tag.setGPO1Bit(BIT_GPO1_GPO_EN, true);
}

void ISR()
{
  interruptChanged = true;
}

void loop()
{
  static uint16_t counter = 0;

  if (interruptChanged == true)
  {
    interruptChanged = false;
    Serial.print(F("RF field change detected "));
    Serial.print(++counter);
    Serial.println(F(" times."));
  }
}
