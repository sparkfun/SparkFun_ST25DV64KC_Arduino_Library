/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Ricardo Ramos  @ SparkFun Electronics, January 6th, 2021
  This file declares all functions used in the ST25DV64KC Dynamic RFID Tag Arduino Library IO layer.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SPARKFUN_ST25DV64KC_IO_
#define _SPARKFUN_ST25DV64KC_IO_

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

class SFE_ST2525DV64KC_IO
{
private:
  TwoWire *_i2cPort;

public:
  // Default constructor.
  SFE_ST2525DV64KC_IO(){};

  // Default destructor
  ~SFE_ST2525DV64KC_IO(){};

  // Define the I2C chunk size (the maximum number of bytes to be read/written in one transmission)
  uint8_t readWriteChunkSize = 32;
  const uint8_t maxRetries = 6;
  const uint8_t retryDelay = 5;

  // Starts two wire interface.
  bool begin(TwoWire &wirePort);

  // Returns true if we get a reply from the I2C device.
  bool isConnected();

  // Since ST25DV64KC has two possible I2C addresses, the correct address must be passed to each corresponding
  // IO function so the proper area is addressed.

  // Read a single uint8_t from a register.
  bool readSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *value);

  // Writes a single uint8_t into a register.
  bool writeSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t value);

  // Reads multiple bytes from a register into buffer uint8_t array.
  bool readMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength);

  // Writes multiple bytes to register from buffer uint8_t array.
  bool writeMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength);

  // Sets a single bit in a specific register. Bit position ranges from 0 (lsb) to 7 (msb).
  bool setRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask);

  // Clears a single bit in a specific register. Bit position ranges from 0 (lsb) to 7 (msb).
  bool clearRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask);

  // Returns true if a specific bit is set in a register. Bit position ranges from 0 (lsb) to 7 (msb).
  bool isBitSet(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask);
};

#endif
