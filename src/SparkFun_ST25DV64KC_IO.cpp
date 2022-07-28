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
  _i2cPort->beginTransmission(static_cast<int>(address));

  uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
  _i2cPort->write(temp);
  temp = registerAddress & 0xff;
  _i2cPort->write(temp);

  for (uint16_t i = 0; i < packetLength; i++)
    _i2cPort->write(buffer[i]);

  bool success = (_i2cPort->endTransmission() == 0);

  // IC needs approx. 5ms to complete its writing phase.
  delay(10);

  return success;
}

void SFE_ST2525DV64KC_IO::readMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength)
{
  _i2cPort->beginTransmission(static_cast<int>(address));

  uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
  _i2cPort->write(temp);
  temp = registerAddress & 0xff;
  _i2cPort->write(temp);

  _i2cPort->endTransmission();

  _i2cPort->requestFrom(static_cast<int>(address), static_cast<int>(packetLength));
  for (uint16_t i = 0; (i < packetLength) && _i2cPort->available(); i++)
    buffer[i] = _i2cPort->read();
}

uint8_t SFE_ST2525DV64KC_IO::readSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress)
{
  uint8_t result;
  _i2cPort->beginTransmission(static_cast<int>(address));

  uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
  _i2cPort->write(temp);
  temp = registerAddress & 0xff;
  _i2cPort->write(temp);
  _i2cPort->endTransmission();

  _i2cPort->requestFrom(static_cast<int>(address), 1U);
  result = _i2cPort->read();
  return result;
}

bool SFE_ST2525DV64KC_IO::writeSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t value)
{
  _i2cPort->beginTransmission(static_cast<int>(address));

  uint8_t temp = static_cast<uint16_t>(registerAddress) >> 8;
  _i2cPort->write(temp);
  temp = registerAddress & 0xff;
  _i2cPort->write(temp);

  _i2cPort->write(value);

  bool success = (_i2cPort->endTransmission() == 0);

  // IC needs approx. 5ms to complete its writing phase.
  delay(10);

  return success;
}

void SFE_ST2525DV64KC_IO::setRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value = readSingleByte(address, registerAddress);
  value |= bitMask;
  writeSingleByte(address, registerAddress, value);
}

void SFE_ST2525DV64KC_IO::clearRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value = readSingleByte(address, registerAddress);
  value &= ~bitMask;
  writeSingleByte(address, registerAddress, value);
}

bool SFE_ST2525DV64KC_IO::isBitSet(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
{
  uint8_t value = readSingleByte(address, registerAddress);
  return (value & bitMask);
}