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

bool SFE_ST25DV64KC_NDEF::writeCCFile4Byte(uint32_t val)
{
  uint8_t CCFile[4];
  CCFile[0] = val >> 24;
  CCFile[1] = (val >> 16) & 0xFF;
  CCFile[2] = (val >> 8) & 0xFF;
  CCFile[3] = val & 0xFF;

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

  return writeEEPROM(0x0, CCFile, 0x8);
}
