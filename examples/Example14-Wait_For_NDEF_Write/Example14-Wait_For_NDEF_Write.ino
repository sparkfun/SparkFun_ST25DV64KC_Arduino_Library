/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example:
  * Clears the tag memory
  * Waits for an NDEF URI, WiFi or Text record to be written via RF
  * Prints it

  We've noticed that it is possible to confuse the ST "NFC Tap" App (on iPhone) when adding new records to existing ones in tag memory.
  We think the App is doing a read-modify-write using separate RF transactions and gets confused if the tag
  becomes busy with an I2C transaction part way through.
  We've found that using the GPO pin to indicate RF activity and delay the next I2C transaction is a good work-around.
  Uncomment the "#define useGPOpin" below if you want to use the GPO pin to delay I2C activity.
  
  The ST25PC-NFC (Windows) software seems much more robust. But you need a separate reader (e.g. ST25R3911B-DISCO) to use that.
  And it has a bug where it adds an extra space to the SSID in WiFi records, which is a bit annoying...

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

//#define useGPOpin // Uncomment this line if you want to use the GPO pin to detect RF activity

#ifdef useGPOpin
// Use a jumper cable to link the ST25DV64KC GPO pin to a digital pin
const uint8_t GPO_PIN = 2; // Change this to match the digital pin you have linked GPO to
#endif

// Error handler: print the error code as readable text
// An I2C_TRANSMISSION_ERROR probably indicates that the tag is busy servicing an RF transaction
void errorHandler(SF_ST25DV64KC_ERROR errorCode)
{
  Serial.print(F("Error Callback: "));
  Serial.println(tag.errorCodeString(errorCode));
}

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Wire.begin();
  
#ifdef useGPOpin
  pinMode(GPO_PIN, INPUT);
#endif

  Serial.println(F("ST25DV64KC example."));

  //tag.setErrorCallback(&errorHandler); // Uncomment this line to enable the error callback

  if (!tag.begin(Wire))
  {
    Serial.println(F("ST25 not detected. Freezing..."));
    while (1) // Do nothing more
      ;
  }

  Serial.println(F("ST25 connected."));

  // -=-=-=-=-=-=-=-=-

#ifdef useGPOpin

  // The GPO registers can only be changed during an open security session
  Serial.println(F("Opening I2C session with password."));
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  // -=-=-=-=-=-=-=-=-

  Serial.println(F("Configuring GPO1 to indicate RF user & activity."));
  tag.setGPO1Bit(BIT_GPO1_RF_USER_EN, true);
  tag.setGPO1Bit(BIT_GPO1_RF_ACTIVITY_EN, true);
  
  Serial.println(F("Disabling other bits."));
  tag.setGPO1Bit(BIT_GPO1_RF_INTERRUPT_EN, false);
  tag.setGPO1Bit(BIT_GPO1_FIELD_CHANGE_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_PUT_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_GET_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_WRITE_EN, false);

  // -=-=-=-=-=-=-=-=-

  Serial.println(F("Enabling GPO_EN bit."));
  tag.setGPO1Bit(BIT_GPO1_GPO_EN, true);

#endif

  // -=-=-=-=-=-=-=-=-

  // Clear the first 256 bytes of user memory
  uint8_t tagMemory[256];
  memset(tagMemory, 0, 256);

  Serial.println("Writing 0x0 to the first 256 bytes of user memory.");
  tag.writeEEPROM(0x0, tagMemory, 256);

  // -=-=-=-=-=-=-=-=-

  // Write the Type 5 CC File - eight bytes - starting at address zero
  Serial.println(F("Writing CC_File"));
  tag.writeCCFile8Byte();

  // -=-=-=-=-=-=-=-=-

  // Add an empty record at the first memory location after the CC File
  Serial.println(F("Writing an empty (zero-length) TLV Record"));
  uint16_t memoryLocation = tag.getCCFileLen(); // Write to the memory location immediately after the CC File
  tag.writeNDEFEmpty(&memoryLocation);

  // -=-=-=-=-=-=-=-=-

  Serial.println(F("Please use the ST NFC Tap App to write an NDEF URI, WiFi or Text Record to the tag"));
}

void loop()
{
  static char thing1[50]; // Create storage for the URI, SSID, Text
  static char thing2[50]; // Create storage for the password
  static uint8_t uriRecord = 1; // Keep track of how many URIs have been read
  static uint8_t wifiRecord = 1; // Keep track of how many WiFi records have been read
  static uint8_t textRecord = 1; // Keep track of how many Text records have been read
  static unsigned long lastRead = 0; // Keep track of the last tag read
  static uint8_t loopCounter = 0; // Only attempt one read each time around the loop

#ifdef useGPOpin
  static unsigned long lastActivity = 0; // Keep track of the last RF activity

  if (digitalRead(GPO_PIN) == LOW) // GPO will be low when there is RF activity
  {
    lastActivity = millis(); // Record the last RF activity
  }

  // Only try to communicate with the tag when there has been no RF activity for at least two seconds
  if (millis() > (lastActivity + 2000))
#endif

  {
    if (millis() > (lastRead + 250)) // Don't pound the I2C bus too hard. Do a single read every 250ms
    {
      // Check for a new NDEF URI record
      if ((loopCounter % 3) == 0)
      {
        if (tag.readNDEFURI(thing1, 50, uriRecord))
        {
          Serial.println(F("URI Record Found!"));
          Serial.println(thing1);
          uriRecord++; // Increase the count
        }
      }
  
      // Check for a new NDEF WiFi record
      if ((loopCounter % 3) == 1)
      {
        if (tag.readNDEFWiFi(thing1, 50, thing2, 50, wifiRecord))
        {
          Serial.println(F("WiFi Record Found!"));
          Serial.print(F("    SSID:"));
          Serial.println(thing1);
          Serial.print(F("Password:"));
          Serial.println(thing2);
          wifiRecord++; // Increase the count
        }
      }
  
      // Check for a new NDEF Text record
      if ((loopCounter % 3) == 2)
      {
        if (tag.readNDEFText(thing1, 50, textRecord))
        {
          Serial.println(F("Text Record Found!"));
          Serial.println(thing1);
          textRecord++; // Increase the count
        }
      }
  
      loopCounter++;
      lastRead = millis(); // Record the last read
    }
  }
}
