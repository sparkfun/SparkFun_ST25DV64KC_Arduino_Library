/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example shows how to set up the ST25DV64KC's Capability Container (CC)
  and create a NDEF URI record for https://www.sparkfun.com

  Feel like supporting open source hardware?
  Buy a board from SparkFun!
  SparkFun Qwiic RFID Tag - ST25DV64KC : https://www.sparkfun.com/products/19035

  Hardware Connections:
  Plug a Qwiic cable into the Qwiic RFID Tag and a RedBoard
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/14425)
  Open the serial monitor at 115200 baud to see the output
*/

/*
  The following resources are very useful:
  
  ST Application Note AN4911:
  https://www.st.com/resource/en/application_note/an4911-ndef-management-with-st25dvi2c-series-and-st25tv16k-and-st25tv64k-products-stmicroelectronics.pdf
  
  ST25PC-NFC software:
  https://www.st.com/en/embedded-software/stsw-st25pc001.html
  
  ST25R3911B-DISCO:
  https://www.st.com/en/evaluation-tools/st25r3911b-disco.html

  NFC Data Exchange Format (NDEF):
  The official version is available from: https://nfc-forum.org/build/specifications#record-type-definition-technical-specifications
  Or search for "nfc data exchange format (ndef)"

  ST Application Note AN3408:
  https://www.st.com/resource/en/application_note/an3408-using-lrixx-lrisxx-m24lrxxr-and-m24lrxxer-products-as-nfc-vicinity-tags-stmicroelectronics.pdf
*/

/*
  In this example, the Capacity Container File is written to the first eight bytes of user memory:

  0xE2 0x40 0x00 0x01 0x00 0x00 0x03 0xFF

  Byte 0: Magic Number
          The ST25DV64KC has 8kBytes of user memory so we need to select 0xE2 to select two-byte addressing
  Byte 1: Version and Access Condition
          b7 b6 = 0b01 Major Version
          b5 b4 = 0b00 Minor Version
          b3 b2 = 0x00 Read Access: Always
          b1 b0 = 0x00 Write Access: Always
  Byte 2: 0x00
  Byte 3: Additional Feature Information
          b7 b6 b5 = 0b000 RFU (Reserved Future Use)
          b4 = 0b0 Special Frame
          b3 = 0b0 Lock Block
          b2 b1 = 0b00 RFU
          b0 = 0b1 MBREAD: Read Multiple Block is supported
  Byte 4: 0x00 RFU (Reserved Future Use)
  Byte 5: 0x00 RFU (Reserved Future Use)
  Byte 6 + Byte 7: MLEN Encoded Memory Length
          MLEN = T5T_Area / 8
          MLEN encoding for a ST25DV64K (8192 bytes memory size) and 8 bytes capability container (CC):
          If the entire user memory full, user memory is used to store NDEF, T5T_Area=8192-8
          MLEN = (8192 – 8) / 8 = 1023 (0x03FF)
*/

/*
  To create a single NDEF URI short record:

  0x03 0x11 0xD1 0x01 0x0D 0x55 0x02 sparkfun.com 0xFE

  Byte 0: Type5 Tag TLV-Format: T (Type field)
          0x03 = NDEF Message TLV
  Byte 1: Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
          0x11 = 17 Bytes
  Bytes 2-18: Type5 Tag TLV-Format: V (Value field)
          Byte 2: Record Header
                  b7 = 0b1 MB (Message Begin)
                  b6 = 0b1 ME (Message End)
                  b5 = 0b0 CF (Chunk Flag)
                  b4 = 0b1 SR (Short Record)
                  b3 = 0b0 IL (ID Length)
                  b2 b1 b0 = 0b001 TNF (Type Name Format): NFC Forum Well-Known Type
          Byte 3: Type Length
                  0x01 = 1 Byte
          Byte 4: Payload Length
                  0x0D = 13 bytes
          Byte 5: Record Type
                  0x55 = "U" URI Record
          Byte 6: URI Prefix Code
                  0x02 = https://www.
          Bytes 7-18: URI (12 bytes)
                  sparkfun.com
  Byte 19: Type5 Tag TLV-Format: T (Type field)
          0xFE = Terminator TLV
*/

#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC tag;

#define CC_Len 8
const uint8_t CC_File[CC_Len] = { 0xE2, 0x40, 0x00, 0x01, 0x00, 0x00, 0x03, 0xFF };

#define Type5_NDEF_Message_TLV  0x03
#define Type5_Terminator_TLV    0xFE
#define NDEF_MB                 (1 << 7)
#define NDEF_ME                 (1 << 6)
#define NDEF_CF                 (1 << 5)
#define NDEF_SR                 (1 << 4)
#define NDEF_TNF_WELL_KNOWN     0x01
#define NDEF_URI_RECORD         0x55
#define NDEF_PREFIX_HTTPS_WWW   0x02

const char URI[] = "sparkfun.com";

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

  // Write the Type 5 CC File - starting at address zero
  Serial.println(F("Writing CC_File"));
  tag.writeEEPROM(0x0, (uint8_t *)CC_File, CC_Len);

  // Write the Type 5 NDEF URI 
  uint8_t tagWrite[256];
  memset(tagWrite, 0, 256);
  tagWrite[0] = Type5_NDEF_Message_TLV; // Type5 Tag TLV-Format: T (Type field)
  tagWrite[1] = strlen(URI) + 5; // Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
  tagWrite[2] = NDEF_MB | NDEF_ME | NDEF_SR | NDEF_TNF_WELL_KNOWN; // NDEF Record Header
  tagWrite[3] = 0x01; // NDEF Type Length
  tagWrite[4] = strlen(URI) + 1; // NDEF Payload Length
  tagWrite[5] = NDEF_URI_RECORD; // NDEF Record Type
  tagWrite[6] = NDEF_PREFIX_HTTPS_WWW; // NDEF URI Prefix Code
  strcpy((char *)&tagWrite[7], URI); // Add the URI
  tagWrite[7 + strlen(URI)] = Type5_Terminator_TLV; // Type5 Tag TLV-Format: T (Type field)
  Serial.println(F("Writing the NDEF URI record"));
  tag.writeEEPROM(CC_Len, tagWrite, 256);

  // Read back the memory contents
  Serial.println(F("The first 256 bytes of user memory are:"));
  uint8_t tagRead[256];
  tag.readEEPROM(0x0, tagRead, 256);
  prettyPrintChars(tagRead, 256);
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
