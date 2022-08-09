/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This is the example we use to test boards at SparkFun.

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

  Serial.println(F("ST25DV64KC Production Test"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  delay(250); // Try every 0.25 seconds

  Wire.begin();
  
  //Initialize tag
  if (tag.begin() == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: could not detect tag"));
    return;
  }

  uint8_t password[8] = {0x0}; // Default password is all zeros
  tag.openI2CSession(password);

  if (tag.isI2CSessionOpen() == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: could not open I2C session"));
    return;
  }

  // Write the Type 5 CC File - eight bytes - starting at address zero
  if (tag.writeCCFile8Byte() == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: could not write CC File"));
    return;
  }

  // Write a single Type 5 NDEF URI 
  if (tag.writeNDEFURI("sparkfun.com", SFE_ST25DV_NDEF_URI_ID_CODE_HTTPS_WWW) == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: could not write NDEF URI"));
    return;
  }

  bool stillConnected = true;

  while (stillConnected)
  {
    // All good!
    Serial.println(F("Test passed!"));
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);    
    stillConnected = tag.isConnected(); // Detect ST25DV64KC on address 0x53
  }

  digitalWrite(LED_BUILTIN, LOW);
  Wire.end();
  Serial.println(F("Tag disconnected..."));
}
