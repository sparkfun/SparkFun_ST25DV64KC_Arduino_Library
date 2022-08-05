/*
  ST25DV64KC Example
  By: Ricardo Ramos and Paul Clark
  SparkFun Electronics
  Date: August, 2022
  License: MIT. Please see the license file for more information but you can
  basically do whatever you want with this code.

  This example shows how to set up the ST25DV64KC's Capability Container (CC)
  and create a NDEF WiFi record

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

  Wi-Fi Simple Configuration Technical Specification v2.0.5
  Available from: https://www.wi-fi.org/downloads-registered-guest/Wi-Fi_Simple_Configuration_Technical_Specification_v2.0.5.pdf/29560
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
  To create a single NDEF WiFi short record:

  00000 0x0000 E2 40 00 01 00 00 03 FF 03 72 DA 17 57 00 61 70 .@.......r..W.ap
  00016 0x0010 70 6C 69 63 61 74 69 6F 6E 2F 76 6E 64 2E 77 66 plication/vnd.wf
  00032 0x0020 61 2E 77 73 63 10 0E 00 53 10 26 00 01 01 10 45 a.wsc...S.&....E
  00048 0x0030 00 0C 67 75 65 73 74 4E 65 74 77 6F 72 6B 10 03 ..guestNetwork..
  00064 0x0040 00 02 00 20 10 0F 00 02 00 08 10 27 00 10 67 75 ... .......'..gu
  00080 0x0050 65 73 74 50 61 73 73 77 6F 72 64 31 32 33 10 20 estPassword123.
  00096 0x0060 00 06 00 00 00 00 00 00 10 49 00 06 00 37 2A 02 .........I...7*.
  00112 0x0070 01 01 10 49 00 06 00 37 2A 00 01 20 FE

  Starting at memory location 0008:

  Byte 0: Type5 Tag TLV-Format: T (Type field)
          0x03 = NDEF Message TLV
  Byte 1: Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
          0x72 = 114 Bytes
  Bytes 2-115: Type5 Tag TLV-Format: V (Value field)
          Byte 2: Record Header = 0xDA
                  b7 = 0b1 MB (Message Begin)
                  b6 = 0b1 ME (Message End)
                  b5 = 0b0 CF (Chunk Flag)
                  b4 = 0b1 SR (Short Record)
                  b3 = 0b1 IL (ID Length)
                  b2 b1 b0 = 0b010 TNF (Type Name Format): Media-type as defined in RFC 2046
          Byte 3: Type Length
                  0x17 = 23 Bytes
          Byte 4: Payload Length
                  0x57 = 87 Bytes
          Byte 5: ID Length
                  0x00 = 0 Bytes
          Bytes 6-28: Type (23 bytes)
                  "application/vnd.wfa.wsc"
          Start of Payload:
          Bytes 29+30: 0x100E (WiFi Credential)
          Bytes 31+32: 0x00 0x53 (Length = 83)
          Bytes 33+34: 0x1026 (Network Index)
          Bytes 35+36: 0x00 0x01 (Length = 1)
          Byte 37:     0x01
          Bytes 38+39: 0x1045 (SSID)
          Bytes 40+41: 0x00 0x0C (length = 12)
          Bytes 42-53: "guestNetwork"
          Bytes 54+55: 0x1003(Authentication Type)
          Bytes 56+57: 0x00 0x02 (Length = 2)
          Bytes 58+59: 0x00 0x20 (WPA2 Personal)
          Bytes 60+61: 0x100F (Encryption Type)
          Bytes 62+63: 0x00 0x02 (Length = 2)
          Bytes 64+65: 0x00 0x08 (AES)
          Bytes 66+67: 0x1027 (Network Key)
          Bytes 68+69: 0x00 0x10 (Length = 16)
          Bytes 70-85: "guestPassword123"
          Bytes 86+87: 0x1020 (MAC Address)
          Bytes 88+89: 0x00 0x06 (Length = 6)
          Bytes 90-95: 0x00 0x00 0x00 0x00 0x00 0x00
          Bytes 96+97: 0x1049 (Vendor Extension)
          Bytes 98+99: 0x00 0x06 (Length = 6)
          Bytes 100-105: 0x00 0x37 0x2A (Vendor ID) 0x02 0x01 0x01 (Vendor Data)
          Bytes 106+107: 0x1049 (Vendor Extension)
          Bytes 108+109: 0x00 0x06 (Length = 6)
          Bytes 110-115: 0x00 0x37 0x2A (Vendor ID) 0x00 0x01 0x20 (Vendor Data)
          End of Payload
  Byte 116: Type5 Tag TLV-Format: T (Type field)
          0xFE = Terminator TLV
*/

#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC tag;

#define CC_Len 8
const uint8_t CC_File[CC_Len] = {0xE2, 0x40, 0x00, 0x01, 0x00, 0x00, 0x03, 0xFF};

#define Type5_NDEF_Message_TLV 0x03
#define Type5_Terminator_TLV 0xFE

#define NDEF_MB (1 << 7)
#define NDEF_ME (1 << 6)
#define NDEF_CF (1 << 5)
#define NDEF_SR (1 << 4)
#define NDEF_IL (1 << 3)
#define NDEF_TNF_MEDIA 0x02

const uint8_t VERSION[2] = {0x10, 0x4A};
const uint8_t CREDENTIAL[2] = {0x10, 0x0e};
const uint8_t AUTH_TYPE[2] = {0x10, 0x03};
const uint8_t CRYPT_TYPE[2] = {0x10, 0x0F};
const uint8_t MAC_ADDRESS[2] = {0x10, 0x20};
const uint8_t NETWORK_IDX[2] = {0x10, 0x26};
const uint8_t NETWORK_KEY[2] = {0x10, 0x27};
const uint8_t NETWORK_NAME[2] = {0x10, 0x45};
const uint8_t OOB_PASSWORD[2] = {0x10, 0x2C};
const uint8_t VENDOR_EXT[2] = {0x10, 0x49};
const uint8_t VENDOR_WFA[3] = {0x00, 0x37, 0x2A};
const uint8_t VERSION2 = {0x00};
const uint8_t KEY_SHAREABLE = {0x02};

const uint8_t AUTH_OPEN[2] = {0x00, 0x01};
const uint8_t AUTH_WPA_PERSONAL[2] = {0x00, 0x02};
const uint8_t AUTH_SHARED[2] = {0x00, 0x04};
const uint8_t AUTH_WPA_ENTERPRISE[2] = {0x00, 0x08};
const uint8_t AUTH_WPA2_ENTERPRISE[2] = {0x00, 0x10};
const uint8_t AUTH_WPA2_PERSONAL[2] = {0x00, 0x20};
const uint8_t AUTH_WPA_WPA2_PERSONAL[2] = {0x00, 0x22};

const uint8_t CRYPT_NONE[2] = {0x00, 0x01};
const uint8_t CRYPT_WEP[2] = {0x00, 0x02};
const uint8_t CRYPT_TKIP[2] = {0x00, 0x04};
const uint8_t CRYPT_AES[2] = {0x00, 0x08};
const uint8_t CRYPT_AES_TKIP[2] = {0x00, 0x0C};

const char MIME_Type[] = "application/vnd.wfa.wsc";

const uint8_t MAC_Address[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t Vendor_Data_1[3] = {0x02, 0x01, 0x01};
const uint8_t Vendor_Data_2[3] = {0x00, 0x01, 0x20};

const char WiFi_SSID[] = "guestNetwork";
const char WiFi_Password[] = "guestPassword123";

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

  // Write the Type 5 NDEF WiFi Record
  uint8_t tagWrite[256];
  memset(tagWrite, 0, 256);
  tagWrite[0] = Type5_NDEF_Message_TLV;                                 // Type5 Tag TLV-Format: T (Type field)
  tagWrite[2] = NDEF_MB | NDEF_ME | NDEF_SR | NDEF_IL | NDEF_TNF_MEDIA; // NDEF Record Header
  uint8_t mimeLen = strlen(MIME_Type);
  tagWrite[3] = mimeLen;                              // NDEF Type Length
  tagWrite[5] = 0x00;                                 // NDEF ID Length
  strcpy((char *)&tagWrite[6], MIME_Type);            // Add MIME type
  uint8_t *payloadStart = &tagWrite[6 + mimeLen];     // Record the start of the payload
  uint8_t *target = payloadStart;                     // Point target at the start of the payload
  target = addField(target, CREDENTIAL, 2);           // Add WiFi Credentials
  target += 2;                                        // Skip over the credentials length for now
  target = addField(target, NETWORK_IDX, 2);          // Add Network Index
  target = addUint16t(target, 0x0001);                // Network Index Length
  target = addUint8t(target, 0x01);                   // Network Index
  target = addField(target, NETWORK_NAME, 2);         // Add SSID (Network Name)
  target = addUint16t(target, strlen(WiFi_SSID));     // SSID Length
  strcpy((char *)target, WiFi_SSID);                  // Copy the SSID
  target += strlen(WiFi_SSID);                        // Increment target
  target = addField(target, AUTH_TYPE, 2);            // Add the Authentication Type
  target = addUint16t(target, 0x0002);                // Authentication Type Length
  target = addField(target, AUTH_WPA2_PERSONAL, 2);   // Authentication Type
  target = addField(target, CRYPT_TYPE, 2);           // Add the Encryption Type
  target = addUint16t(target, 0x0002);                // Encryption Type Length
  target = addField(target, CRYPT_AES, 2);            // Encryption Type
  target = addField(target, NETWORK_KEY, 2);          // Add the Network Key (Password)
  target = addUint16t(target, strlen(WiFi_Password)); // Password Length
  strcpy((char *)target, WiFi_Password);              // Copy the password
  target += strlen(WiFi_Password);                    // Increment target
  target = addField(target, MAC_ADDRESS, 2);          // Add the MAC Address
  target = addUint16t(target, 0x0006);                // MAC Address Length
  memcpy(target, MAC_Address, 6);                     // Copy the MAC Address (could be full of zeros)
  target += 6;                                        // Increment target
  target = addField(target, VENDOR_EXT, 2);           // Add the first vendor extension
  target = addUint16t(target, 0x0006);                // Vendor Extension Length
  target = addField(target, VENDOR_WFA, 3);
  target = addField(target, Vendor_Data_1, 3);
  target = addField(target, VENDOR_EXT, 2); // Add the second vendor extension
  target = addUint16t(target, 0x0006);      // Vendor Extension Length
  target = addField(target, VENDOR_WFA, 3);
  target = addField(target, Vendor_Data_2, 3);
  addUint8t(target, Type5_Terminator_TLV); // Type5 Tag TLV-Format: T (Type field)
  uint16_t payloadLen = target - payloadStart;
  addUint16t((uint8_t *)(payloadStart + 2), payloadLen - 4); // WiFi Credentials Length
  tagWrite[4] = (uint8_t)payloadLen;                         // NDEF Payload Length
  tagWrite[1] = (uint8_t)payloadLen + strlen(MIME_Type) + 4; // Type5 Tag TLV-Format: L (Length field) (1-Byte Format)

  Serial.println(F("Writing the NDEF WiFi record"));
  tag.writeEEPROM(CC_Len, tagWrite, 256);

  // Read back the memory contents
  Serial.println(F("The first 256 bytes of user memory are:"));
  uint8_t tagRead[256];
  tag.readEEPROM(0x0, tagRead, 256);
  prettyPrintChars(tagRead, 256);
}

uint8_t *addField(uint8_t *target, const uint8_t *source, uint8_t len)
{
  for (uint8_t i = 0; i < len; i++)
    *target++ = *source++;

  return target;
}

uint8_t *addUint16t(uint8_t *target, const uint16_t source)
{
  *target++ = source >> 8;
  *target++ = source & 0xFF;

  return target;
}

uint8_t *addUint8t(uint8_t *target, const uint8_t source)
{
  *target++ = source;

  return target;
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
