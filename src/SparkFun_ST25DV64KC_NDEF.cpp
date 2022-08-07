/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Paul CLark  @ SparkFun Electronics, August 5th, 2021
  This file declares the additional functions used to read/write NDEF 
  records from/to the ST25DV64KC Dynamic RFID Tag.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "SparkFun_ST25DV64KC_NDEF.h"

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

bool SFE_ST25DV64KC_NDEF::writeCCFile4Byte(uint32_t val)
{
  uint8_t CCFile[4];
  CCFile[0] = val >> 24;
  CCFile[1] = (val >> 16) & 0xFF;
  CCFile[2] = (val >> 8) & 0xFF;
  CCFile[3] = val & 0xFF;

  _ccFileLen = 4;

  return writeEEPROM(0x0, CCFile, 0x4);
}

/*
  The Capacity Container File is written to the first eight bytes of user memory:

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

bool SFE_ST25DV64KC_NDEF::writeCCFile8Byte(uint32_t val1, uint32_t val2)
{
  uint8_t CCFile[8];
  CCFile[0] = val1 >> 24;
  CCFile[1] = (val1 >> 16) & 0xFF;
  CCFile[2] = (val1 >> 8) & 0xFF;
  CCFile[3] = val1 & 0xFF;
  CCFile[4] = val2 >> 24;
  CCFile[5] = (val2 >> 16) & 0xFF;
  CCFile[6] = (val2 >> 8) & 0xFF;
  CCFile[7] = val2 & 0xFF;

  _ccFileLen = 8;

  return writeEEPROM(0x0, CCFile, 0x8);
}

/*
  To create a single NDEF URI short record:

  0x03 0x11 0xD1 0x01 0x0D 0x55 0x02 sparkfun.com 0xFE

  Byte 0: Type5 Tag TLV-Format: T (Type field)
          0x03 = NDEF Message TLV
  Byte 1: Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
          0x11 = 17 Bytes
  Bytes 2-18: Type5 Tag TLV-Format: V (Value field)
          Byte 2: Record Header = 0xD1
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

// Write an NDEF URI Record to user memory
// If address is not NULL, start writing at *address, otherwise start at _ccFileLen
// MB = Message Begin, ME = Message End
// Default is a single message (MB=true, ME=true)
// To add multiple URIs:
//   First: MB=true, ME=false
//   Intermediate: MB=false, ME=false
//   Last: MB=false, ME=true
bool SFE_ST25DV64KC_NDEF::writeNDEFURI(const char *uri, uint8_t idCode, uint16_t *address, bool MB, bool ME)
{
  // Total length could be: strlen(uri) + 8 (see above) + 2 (if L field > 0xFE) + 3 (if PAYLOAD LENGTH > 255)
  uint8_t tagWrite[strlen(uri) + 13];
  memset(tagWrite, 0, strlen(uri) + 13);

  uint8_t *tagPtr = &tagWrite[0];

  uint16_t payloadLength = strlen(uri) + 1; // Payload length is strlen(uri) + Record Type

  // Total field length is strlen(uri) + Prefix Code + Record Type + Payload Length + Type Length + Record Header
  uint16_t fieldLength = strlen(uri) + 1 + 1 + ((payloadLength <= 0xFF) ? 1 : 4) + 1 + 1;

  // Only write the Type 5 T & L fields if the Message Begin bit is set
  if (MB)
  {
    *tagPtr++ = SFE_ST25DV_TYPE5_NDEF_MESSAGE_TLV; // Type5 Tag TLV-Format: T (Type field)

    if (fieldLength > 0xFE) // Is the total L greater than 0xFE?
    {
      *tagPtr++ = 0xFF; // Type5 Tag TLV-Format: L (Length field) (3-Byte Format)
      *tagPtr++ = fieldLength >> 8;
      *tagPtr++ = fieldLength & 0xFF;
    }
    else
    {
      *tagPtr++ = fieldLength; // Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
    }
  }

  // NDEF Record Header
  *tagPtr++ = (MB ? SFE_ST25DV_NDEF_MB : 0) | (ME ? SFE_ST25DV_NDEF_ME : 0) | ((payloadLength <= 0xFF) ? SFE_ST25DV_NDEF_SR : 0) | SFE_ST25DV_NDEF_TNF_WELL_KNOWN;
  *tagPtr++ = 0x01; // NDEF Type Length
  if (payloadLength <= 0xFF)
  {
    *tagPtr++ = payloadLength; // NDEF Payload Length (1-Byte)
  }
  else
  {
    *tagPtr++ = payloadLength >> 24; // NDEF Payload Length (4-Byte)
    *tagPtr++ = (payloadLength >> 16) & 0xFF;
    *tagPtr++ = (payloadLength >> 8) & 0xFF;
    *tagPtr++ = payloadLength & 0xFF;
  }
  *tagPtr++ = SFE_ST25DV_NDEF_URI_RECORD; // NDEF Record Type
  *tagPtr++ = idCode; // NDEF URI Prefix Code

  strcpy((char *)tagPtr, uri); // Add the URI

  tagPtr += strlen(uri);

  if (ME)
  {
    *tagPtr++ = SFE_ST25DV_TYPE5_TERMINATOR_TLV; // Type5 Tag TLV-Format: T (Type field)
  }

  uint16_t memLoc = _ccFileLen; // Write to this memory location
  uint16_t numBytes = tagPtr - &tagWrite[0];

  if (address != NULL)
  {
    memLoc = *address;
  }

  bool result = writeEEPROM(memLoc, tagWrite, numBytes);

  if ((address != NULL) && (result))
  {
    *address = memLoc + numBytes; // Update address so the next writeNDEFURI can append to this one
  }

  // If Message Begin is not set, we need to go back and update the L field
  if (!MB)
  {
    uint16_t baseAddress = _ccFileLen + 1; // Skip the SFE_ST25DV_TYPE5_NDEF_MESSAGE_TLV
    uint8_t data[3];
    result &= readEEPROM(baseAddress, &data[0], 0x03); // Read the possible three length bytes
    if (!result)
      return false;
    if (data[0] == 0xFF) // Is the length already 3-byte?
    {
      uint16_t oldLen = ((uint16_t)data[1] << 8) | data[2];
      oldLen += (ME ? numBytes - 1 : numBytes);
      data[1] = oldLen >> 8;
      data[2] = oldLen & 0xFF;
      result &= writeEEPROM(baseAddress, &data[0], 0x03); // Update the existing 3-byte length
    }
    else
    {
      // Length is 1-byte
      uint16_t newLen = data[0];
      newLen += (ME ? numBytes - 1 : numBytes);
      if (newLen <= 0xFE) // Is the new length still 1-byte?
      {
        data[0] = newLen;
        result &= writeEEPROM(baseAddress, &data[0], 0x01); // Update the existing 1-byte length
      }
      else
      {
        // The length was 1-byte but needs to be changed to 3-byte
        //delete[] tagWrite; // Delete tagWrite to save memory
        uint8_t newTagWrite[newLen + 4];
        newTagWrite[0] = 0xFF; // Change length to 3-byte
        newTagWrite[1] = newLen >> 8;
        newTagWrite[2] = newLen & 0xFF;
        result &= readEEPROM(baseAddress + 1, &newTagWrite[3], (ME ? newLen + 1 : newLen)); // Copy in the old data
        if (!result)
          return false;
        result &= writeEEPROM(baseAddress, &newTagWrite[0], (ME ? newLen + 4 : newLen + 3));
        if (result)
          *address = *address + 2; // Update address too
      }
    }
  }

  return result;
}

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
          Byte 37:     0x01 (Value 1 is used for backwards compatibility)
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
          Bytes 100-102: 0x00 0x37 0x2A (Vendor ID)
          Byte 103: 0x02 (Network Key Shareable)
          Byte 104: 0x01 (Length)
          Byte 105: 0x01 (True)
          Bytes 106+107: 0x1049 (Vendor Extension)
          Bytes 108+109: 0x00 0x06 (Length = 6)
          Bytes 110-112: 0x00 0x37 0x2A (Vendor ID)
          Byte 113: 0x00 (Version2)
          Byte 114: 0x01 (Length)
          Byte 115: 0x20 (Version 2.0)
          End of Payload
  Byte 116: Type5 Tag TLV-Format: T (Type field)
          0xFE = Terminator TLV
*/

bool SFE_ST25DV64KC_NDEF::writeNDEFWiFi(const char *ssid, const char *passwd, uint16_t *address, bool MB, bool ME,
                                        const uint8_t authType[2], const uint8_t encryptType[2])
{
  // Total length could be: strlen(ssid) + strlen(passwd) + 89 (see above) + 2 (if L field > 0xFE) + 3 (if PAYLOAD LENGTH > 255)
  uint8_t tagWrite[strlen(ssid) + strlen(passwd) + 94];
  memset(tagWrite, 0, strlen(ssid) + strlen(passwd) + 94);

  uint8_t *tagPtr = &tagWrite[0];

  // WiFi Credential length is: 5 (Network Index) + 4 + strlen(ssid) + 6 (Auth) + 6 (Encrypt) + 4 + strlen(passwd) + 10 (MAC address) + 10 (Vendor Ext) + 10 (Vendor Ext)
  uint16_t credentialLength = 5 + 4 + strlen(ssid) + 6 + 6 + 4 + strlen(passwd) + 10 + 10 + 10;

  uint16_t payloadLength = credentialLength + 4;

  // Type Length is: strlen(SFE_ST25DV_WIFI_MIME_TYPE)
  uint8_t typeLength = strlen(SFE_ST25DV_WIFI_MIME_TYPE);

  // Total field length is: Payload Length + ID Length + Type Length + Record Header
  uint16_t fieldLength = payloadLength + ((uint16_t)typeLength) + 1 + ((payloadLength <= 0xFF) ? 1 : 4) + 1 + 1;

  // Only write the Type 5 T & L fields if the Message Begin bit is set
  if (MB)
  {
    *tagPtr++ = SFE_ST25DV_TYPE5_NDEF_MESSAGE_TLV; // Type5 Tag TLV-Format: T (Type field)

    if (fieldLength > 0xFE) // Is the total L greater than 0xFE?
    {
      *tagPtr++ = 0xFF; // Type5 Tag TLV-Format: L (Length field) (3-Byte Format)
      *tagPtr++ = fieldLength >> 8;
      *tagPtr++ = fieldLength & 0xFF;
    }
    else
    {
      *tagPtr++ = fieldLength; // Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
    }
  }

  // NDEF Record Header
  *tagPtr++ = (MB ? SFE_ST25DV_NDEF_MB : 0) | (ME ? SFE_ST25DV_NDEF_ME : 0) | ((payloadLength <= 0xFF) ? SFE_ST25DV_NDEF_SR : 0) | SFE_ST25DV_NDEF_IL | SFE_ST25DV_NDEF_TNF_MEDIA;
  
  // NDEF Type Length
  *tagPtr++ = typeLength;

  // NDEF Payload Length
  if (payloadLength <= 0xFF)
  {
    *tagPtr++ = payloadLength; // NDEF Payload Length (1-Byte)
  }
  else
  {
    *tagPtr++ = payloadLength >> 24; // NDEF Payload Length (4-Byte)
    *tagPtr++ = (payloadLength >> 16) & 0xFF;
    *tagPtr++ = (payloadLength >> 8) & 0xFF;
    *tagPtr++ = payloadLength & 0xFF;
  }

  // ID Length
  *tagPtr++ = 0x00;

  // Type
  strcpy((char *)tagPtr, SFE_ST25DV_WIFI_MIME_TYPE);
  tagPtr += typeLength;

  // WiFi Credential
  *tagPtr++ = SFE_ST25DV_WIFI_CREDENTIAL[0];
  *tagPtr++ = SFE_ST25DV_WIFI_CREDENTIAL[1];
  *tagPtr++ = credentialLength >> 8;
  *tagPtr++ = credentialLength & 0xFF;

  // Network Index
  *tagPtr++ = SFE_ST25DV_WIFI_NETWORK_IDX[0];
  *tagPtr++ = SFE_ST25DV_WIFI_NETWORK_IDX[1];
  *tagPtr++ = 0x00; // Network Index Length
  *tagPtr++ = 0x01; // Network Index Length
  *tagPtr++ = 0x01; // Network Index : Value 1 is used for backwards compatibility

  // SSID
  *tagPtr++ = SFE_ST25DV_WIFI_SSID[0];
  *tagPtr++ = SFE_ST25DV_WIFI_SSID[1];
  *tagPtr++ = ((uint16_t)strlen(ssid)) >> 8;
  *tagPtr++ = ((uint16_t)strlen(ssid)) & 0xFF;
  strcpy((char *)tagPtr, ssid);
  tagPtr += strlen(ssid);

  // Auth Type
  *tagPtr++ = SFE_ST25DV_WIFI_AUTH_TYPE[0];
  *tagPtr++ = SFE_ST25DV_WIFI_AUTH_TYPE[1];
  *tagPtr++ = 0x00; // Auth Type Length
  *tagPtr++ = 0x02; // Auth Type Length
  *tagPtr++ = authType[0]; // Auth Type
  *tagPtr++ = authType[1]; // Auth Type

  // Encrypt Type
  *tagPtr++ = SFE_ST25DV_WIFI_ENCRYPT_TYPE[0];
  *tagPtr++ = SFE_ST25DV_WIFI_ENCRYPT_TYPE[1];
  *tagPtr++ = 0x00; // Encrypt Type Length
  *tagPtr++ = 0x02; // Encrypt Type Length
  *tagPtr++ = encryptType[0]; // Encrypt Type
  *tagPtr++ = encryptType[1]; // Encrypt Type

  // Network Key
  *tagPtr++ = SFE_ST25DV_WIFI_NETWORK_KEY[0];
  *tagPtr++ = SFE_ST25DV_WIFI_NETWORK_KEY[1];
  *tagPtr++ = ((uint16_t)strlen(passwd)) >> 8;
  *tagPtr++ = ((uint16_t)strlen(passwd)) & 0xFF;
  strcpy((char *)tagPtr, passwd);
  tagPtr += strlen(passwd);

  // MAC Address
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDRESS[0];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDRESS[1];
  *tagPtr++ = 0x00; // MAC Address Length
  *tagPtr++ = 0x06; // MAC Address Length
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[0];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[1];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[2];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[3];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[4];
  *tagPtr++ = SFE_ST25DV_WIFI_MAC_ADDR[5];

  // Vendor Extension - Network Key Shareable
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_EXT[0];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_EXT[1];
  *tagPtr++ = 0x00; // Length
  *tagPtr++ = 0x06; // Length
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[0];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[1];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[2];
  *tagPtr++ = SFE_ST25DV_WIFI_KEY_SHAREABLE;
  *tagPtr++ = 0x01; // Length
  *tagPtr++ = SFE_ST25DV_WIFI_KEY_SHAREABLE_TRUE;

  // Vendor Extension - Version 2
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_EXT[0];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_EXT[1];
  *tagPtr++ = 0x00; // Length
  *tagPtr++ = 0x06; // Length
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[0];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[1];
  *tagPtr++ = SFE_ST25DV_WIFI_VENDOR_WFA[2];
  *tagPtr++ = SFE_ST25DV_WIFI_VERSION2;
  *tagPtr++ = 0x01; // Length
  *tagPtr++ = SFE_ST25DV_WIFI_VERSION2_V2_0;

  if (ME)
  {
    *tagPtr++ = SFE_ST25DV_TYPE5_TERMINATOR_TLV; // Type5 Tag TLV-Format: T (Type field)
  }

  uint16_t memLoc = _ccFileLen; // Write to this memory location
  uint16_t numBytes = tagPtr - &tagWrite[0];

  if (address != NULL)
  {
    memLoc = *address;
  }

  bool result = writeEEPROM(memLoc, tagWrite, numBytes);

  if ((address != NULL) && (result))
  {
    *address = memLoc + numBytes; // Update address so the next writeNDEFURI can append to this one
  }

  // If Message Begin is not set, we need to go back and update the L field
  if (!MB)
  {
    uint16_t baseAddress = _ccFileLen + 1; // Skip the SFE_ST25DV_TYPE5_NDEF_MESSAGE_TLV
    uint8_t data[3];
    result &= readEEPROM(baseAddress, &data[0], 0x03); // Read the possible three length bytes
    if (!result)
      return false;
    if (data[0] == 0xFF) // Is the length already 3-byte?
    {
      uint16_t oldLen = ((uint16_t)data[1] << 8) | data[2];
      oldLen += (ME ? numBytes - 1 : numBytes);
      data[1] = oldLen >> 8;
      data[2] = oldLen & 0xFF;
      result &= writeEEPROM(baseAddress, &data[0], 0x03); // Update the existing 3-byte length
    }
    else
    {
      // Length is 1-byte
      uint16_t newLen = data[0];
      newLen += (ME ? numBytes - 1 : numBytes);
      if (newLen <= 0xFE) // Is the new length still 1-byte?
      {
        data[0] = newLen;
        result &= writeEEPROM(baseAddress, &data[0], 0x01); // Update the existing 1-byte length
      }
      else
      {
        // The length was 1-byte but needs to be changed to 3-byte
        //delete[] tagWrite; // Delete tagWrite to save memory
        uint8_t newTagWrite[newLen + 4];
        newTagWrite[0] = 0xFF; // Change length to 3-byte
        newTagWrite[1] = newLen >> 8;
        newTagWrite[2] = newLen & 0xFF;
        result &= readEEPROM(baseAddress + 1, &newTagWrite[3], (ME ? newLen + 1 : newLen)); // Copy in the old data
        if (!result)
          return false;
        result &= writeEEPROM(baseAddress, &newTagWrite[0], (ME ? newLen + 4 : newLen + 3));
        if (result)
          *address = *address + 2; // Update address too
      }
    }
  }

  return result;
}
