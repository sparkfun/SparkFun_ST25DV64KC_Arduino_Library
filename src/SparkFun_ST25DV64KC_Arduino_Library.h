/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Ricardo Ramos  @ SparkFun Electronics, January 6th, 2021
  This file declares all functions used in the ST25DV64KC Dynamic RFID Tag Arduino Library.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SPARKFUN_ST25DV64KC_
#define _SPARKFUN_ST25DV64KC_

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_ST25DV64KC_IO.h"
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

class SFE_ST25DV64KC
{
public:
  // Error callback function pointer.
  // Function must accept a SF_ST25DV64KC_ERROR as errorCode.
  void (*_errorCallback)(SF_ST25DV64KC_ERROR errorCode) = nullptr;

  // Convert errorCode to text
  const char *errorCodeString(SF_ST25DV64KC_ERROR errorCode);

  // I2C communication object instance - can be used to access the
  // ST25 registers through the IO layer functions.
  SFE_ST2525DV64KC_IO st25_io;

  // Default constructor.
  SFE_ST25DV64KC(){};

  // Default destructor.
  ~SFE_ST25DV64KC(){};

  // Sets the error callback function.
  void setErrorCallback(void (*errorCallback)(SF_ST25DV64KC_ERROR errorCode));

  // Initializes ST25DV64KC.
  bool begin(TwoWire &wirePort = Wire);

  // Checks if ST25DK64KC is connected and that chip ID matches the expected result.
  bool isConnected();

  // Reads single register value
  bool readRegisterValue(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *value);

  // Reads multiple values from register
  bool readRegisterValues(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *data, const uint16_t dataLength);

  // Gets device UID (8 bytes).
  bool getDeviceUID(uint8_t *values);

  // Gets device revision.
  bool getDeviceRevision(uint8_t *value);

  // Open I2C security session.
  bool openI2CSession(uint8_t *password);

  // Checks if I2C security session is open.
  bool isI2CSessionOpen();

  // Writes new I2C password. A session must be opened before calling this.
  // CAUTION: you will loose most of I2C functionality if you forget the password
  // since it's used to open a session to allow writing to some registers.
  // There's NO WAY it can be reset to factory defaults!
  // Returns true on success, false otherwise.
  bool writeI2CPassword(uint8_t *password);

  // Programs I2CSS read protection bits (Datasheet page 53)
  // Memory area ranges from 1 to 4. Values outside this range are ignored and function
  // calls the error callback if function pointer is set.
  // readSecured set will demand that I2C session is opened to read memory area
  void programEEPROMReadProtectionBit(uint8_t memoryArea, bool readSecured);

  // Programs I2CSS write protection bits (Datasheet page 53)
  // Memory area ranges from 1 to 4. Values outside this range are ignored and function
  // calls the error callback if function pointer is set.
  // writeSecured set will demand that I2C session is opened to read memory area
  void programEEPROMWriteProtectionBit(uint8_t memoryArea, bool writeSecured);

  // Gets IC2SS read protection bits (Datasheet page 53)
  // Memory area ranges from 1 to 4. Values outside this range are ignored and function
  // calls the error callback if function pointer is set, returning false.
  bool getEEPROMReadProtectionBit(uint8_t memoryArea);

  // Gets IC2SS write protection bits (Datasheet page 53)
  // Memory area ranges from 1 to 4. Values outside this range are ignored and function
  // calls the error callback if function pointer is set, returning false.
  bool getEEPROMWriteProtectionBit(uint8_t memoryArea);

  // Reads block of data from EEPROM.
  bool readEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength);

  // Writes block of data to EEPROM.
  bool writeEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength);

  // Sets memory area boundary. memoryNumber ranges from 1 to 4.
  // endAddressValue must comply with datasheet's area size specifications (page 14).
  // Returns true if memory was correctly programmed and passed all checks, false otherwise.
  // Calls the error callback if the function pointer is set and the returned value is false.
  bool setMemoryAreaEndAddress(uint8_t memoryNumber, uint8_t endAddressValue);

  // Returns memory area end address in bytes. Memory area values range from 1 to 3.
  // Calls the error callback if the function pointer is set and the memory area value is invalid.
  uint16_t getMemoryAreaEndAddress(uint8_t memoryArea);

  // Returns true if there's RF field on the sensor.
  bool RFFieldDetected();

  // Sets a specific GPO1 register bit
  void setGPO1Bit(uint8_t bitMask, bool enabled);

  // Gets a specific GPO1 register bit
  bool getGPO1Bit(uint8_t bitMask);

  // Sets a specific GPO2 register bit
  void setGPO2Bit(uint8_t bitMask, bool enabled);

  // Gets a specific GPO2 register bit
  bool getGPO2Bit(uint8_t bitMask);

  // Sets GPO_CTRL dynamic register bit
  void setGPO_CTRL_DynBit(bool enabled);

  // Gets GPO_CTRL dynamic register bit
  bool getGPO_CTRL_DynBit();

  // Gets IT_STS dynamic register bit value
  bool getIT_STS_DynBit(uint8_t bitMask);

  // Sets EH_MODE bit
  void setEH_MODEBit(bool value);

  // Gets EH_MODE bit
  bool getEH_MODEBit();

  // Sets a specific EH_CTRL_DYN dynamic register bit
  void setEH_CTRL_DYNBit(uint8_t bitMask, bool value);

  // Gets a specific EH_CTRL_DYN dynamic register bit
  bool getEH_CTRL_DYNBit(uint8_t bitMask);
};

#include "SparkFun_ST25DV64KC_NDEF.h"

#endif