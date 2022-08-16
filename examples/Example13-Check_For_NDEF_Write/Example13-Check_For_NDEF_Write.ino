/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example:
  * Clears the tag memory
  * Waits for the user to press a key
  * Checks for new NDEF URI, WiFi or Text records
  * Prints them

  We've noticed that it is possible to confuse the ST "NFC Tap" App (on iPhone) when adding new records to existing ones in tag memory.
  We think the App is doing a read-modify-write using separate RF transactions and gets confused if the tag
  becomes busy with an I2C transaction part way through.
  In this example, the user has to press a key (send any character to the Serial Monitor) to trigger a read.
  That way, the tag will never be busy (on I2C) when you do a write via RF.
  
  We've found that using the GPO pin to indicate RF activity and delay the next I2C transaction can be a good work-around.
  The next example shows how to do that.

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

  Serial.println();
  Serial.println(F("Please use the ST NFC Tap App to write NDEF URI, WiFi or Text Records to the tag"));
  Serial.println();
}

void loop()
{
  static char thing1[50]; // Create storage for the URI, SSID, Text
  static char thing2[50]; // Create storage for the password
  static uint8_t uriRecord = 1; // Keep track of how many URIs have been read
  static uint8_t wifiRecord = 1; // Keep track of how many WiFi records have been read
  static uint8_t textRecord = 1; // Keep track of how many Text records have been read

  while (Serial.available()) // Empty the Serial buffer
    Serial.read();
    
  Serial.println(F("Please press any key to check for new records"));
  Serial.println();

  while (!Serial.available())
    ; // Wait for the user to press a key (send any character)
  
  bool keepGoing = true;

  while (keepGoing)
  {
    keepGoing = false;
    
    // Check for a new NDEF URI record
    if (tag.readNDEFURI(thing1, 50, uriRecord))
    {
      Serial.println(F("New URI Record Found:"));
      Serial.println(thing1);
      Serial.println();
      uriRecord++; // Increase the count
      keepGoing = true;
    }
  
    // Check for a new NDEF WiFi record
    if (tag.readNDEFWiFi(thing1, 50, thing2, 50, wifiRecord))
    {
      Serial.println(F("New WiFi Record Found:"));
      Serial.print(F("    SSID:"));
      Serial.println(thing1);
      Serial.print(F("Password:"));
      Serial.println(thing2);
      Serial.println();
      wifiRecord++; // Increase the count
      keepGoing = true;
    }
  
    // Check for a new NDEF Text record
    if (tag.readNDEFText(thing1, 50, textRecord))
    {
      Serial.println(F("New Text Record Found:"));
      Serial.println(thing1);
      Serial.println();
      textRecord++; // Increase the count
      keepGoing = true;
    }
  }
}
