/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Ricardo Ramos  @ SparkFun Electronics, January 6th, 2021
  This file implements all functions used in the ST25DV64KC Dynamic RFID Tag Arduino Library IO layer.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "SparkFun_ST25DV64KC_IO.h"
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

bool SFE_ST2525DV64KC_IO::begin(TwoWire &i2cPort)
{
  _i2cPort = &i2cPort;
  return isConnected();
}

bool SFE_ST2525DV64KC_IO::isConnected()
{
  _i2cPort->beginTransmission(static_cast<int>(SF_ST25DV64KC_ADDRESS::SYSTEM));
  return _i2cPort->endTransmission() == 0;
}

bool SFE_ST2525DV64KC_IO::writeMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, uint16_t const packetLength)
{
  // Split long writes up into multiple chunks
  uint16_t bytesWritten = 0;

  // If the IC is busy - e.g. completing a previous write - the I2C transmission is NACK'd and fails.
  // For each chunk: try up to maxRetries times, waiting retryDelay ms between tries.
  bool result = true;
  uint8_t maxTries = maxRetries;

  while ((bytesWritten < packetLength) && (result))
  {
    uint8_t bytesToWrite; // Write the data in chunks of readWriteChunkSize max
    if ((packetLength - bytesWritten) > ((uint16_t)readWriteChunkSize))
      bytesToWrite = readWriteChunkSize;
    else
      bytesToWrite = packetLength - bytesWritten;

    _i2cPort->beginTransmission(static_cast<int>(address));

    uint8_t temp = static_cast<uint16_t>(registerAddress + bytesWritten) >> 8;
    _i2cPort->write(temp);
    temp = (registerAddress + bytesWritten) & 0xff;
    _i2cPort->write(temp);

    for (uint16_t i = 0; i < bytesToWrite; i++)
      _i2cPort->write(buffer[i + bytesWritten]);

    bool success = (_i2cPort->endTransmission() == 0);

    if (success)
    {
      bytesWritten += bytesToWrite;
      maxTries = maxRetries;
    }
    else
    {
      maxTries--;
      if (maxTries == 0)
        result = false;
      else
        delay(retryDelay);
    }
  }

  return result;
}

bool SFE_ST2525DV64KC_IO::readMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength)
{
  bool success = true; // Return true if packetLength is zero

  // Split long reads up into multiple chunks
  uint16_t bytesRead = 0;

  // If the IC is busy - e.g. completing a previous write - the I2C transmission is NACK'd and fails.
  // For each chunk: try up to maxRetries times, waiting retryDelay ms between tries.
  uint8_t maxTries = maxRetries;

  while ((bytesRead < packetLength) && (maxTries > 0))
  {
    uint8_t bytesToRead; // Read the data in chunks of readWriteChunkSize max
    if ((packetLength - bytesRead) > ((uint16_t)readWriteChunkSize))
      bytesToRead = readWriteChunkSize;
    else
      bytesToRead = packetLength - bytesRead;

    _i2cPort->beginTransmission(static_cast<int>(address));

    uint8_t temp = static_cast<uint16_t>(registerAddress + bytesRead) >> 8;
    _i2cPort->write(temp);
    temp = (registerAddress + bytesRead) & 0xff;
    _i2cPort->write(temp);

    success = (_i2cPort->endTransmission() == 0);

    if (success)
    {
      success = (_i2cPort->requestFrom(static_cast<int>(address), static_cast<int>(bytesToRead)) == bytesToRead);

      if (success)
      {
        for (uint16_t i = 0; i < bytesToRead; i++)
          buffer[i + bytesRead] = _i2cPort->read();

        bytesRead += bytesToRead;
        maxTries = maxRetries;
      }
    }

    if (!success)
    {
      delay(retryDelay);
      maxTries--;
    }
  }

  return success;
}

bool SFE_ST2525DV64KC_IO::readSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *value)
{
  // If the IC is busy - e.g. completing a previous write - the I2C transmission is NACK'd and fails.
  // Try up to maxRetries times, waiting retryDelay ms between tries.
  uint8_t maxTries = maxRetries;
  bool success = false;

  while ((maxTries > 0) && (!success))
  {
    _i2cPort->beginTransmission(static_cast<int>(address));

    uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
    _i2cPort->write(temp);
    temp = (registerAddress) & 0xff;
    _i2cPort->write(temp);

    success = (_i2cPort->endTransmission() == 0);

    if (success)
    {
      success = (_i2cPort->requestFrom(static_cast<int>(address), 1U) == 1U);

      if (success)
        *value = _i2cPort->read();
    }

    if (!success)
    {
      delay(retryDelay);
      maxTries--;
    }
  }

  return success;
}

bool SFE_ST2525DV64KC_IO::writeSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t value)
{
  // If the IC is busy - e.g. completing a previous write - the I2C transmission is NACK'd and fails.
  // Try up to maxRetries times, waiting retryDelay ms between tries.
  uint8_t maxTries = maxRetries;
  bool success = false;

  while ((maxTries > 0) && (!success))
  {
    _i2cPort->beginTransmission(static_cast<int>(address));

    uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
    _i2cPort->write(temp);
    temp = registerAddress & 0xff;
    _i2cPort->write(temp);

    _i2cPort->write(value);

    success = (_i2cPort->endTransmission() == 0);

    if (!success)
    {
      delay(retryDelay);
      maxTries--;
    }
  }

  return success;
}

bool SFE_ST2525DV64KC_IO::setRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value;
  
  if (!readSingleByte(address, registerAddress, &value))
    return false;

  value |= bitMask;

  return writeSingleByte(address, registerAddress, value);
}

bool SFE_ST2525DV64KC_IO::clearRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value;
  
  if (!readSingleByte(address, registerAddress, &value))
    return false;

  value &= ~bitMask;

  return writeSingleByte(address, registerAddress, value);
}

bool SFE_ST2525DV64KC_IO::isBitSet(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value;
  
  if (!readSingleByte(address, registerAddress, &value))
    return false;

  return (value & bitMask);
}