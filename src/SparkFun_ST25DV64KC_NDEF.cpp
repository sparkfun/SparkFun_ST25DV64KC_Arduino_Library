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

  *tagPtr++ = SFE_ST25DV_TYPE5_NDEF_MESSAGE_TLV; // Type5 Tag TLV-Format: T (Type field)

  uint16_t payloadLength = strlen(uri) + 1; // Payload length is strlen(uri) + Record Type

  // Total field length is strlen(uri) + Prefix Code + Record Type + Payload Length + Type Length + Record Header
  uint16_t fieldLength = strlen(uri) + 1 + 1 + ((payloadLength <= 0xFF) ? 1 : 4) + 1 + 1;

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
    *address = memLoc + numBytes;
  }

  return result;
}