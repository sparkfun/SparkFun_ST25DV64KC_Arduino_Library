/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Ricardo Ramos  @ SparkFun Electronics, January 6th, 2021
  This file implements all functions used in the ST25DV64KC Dynamic RFID Tag Arduino Library.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "SparkFun_ST25DV64KC_Arduino_Library.h"
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

bool SFE_ST25DV64KC::begin(TwoWire &i2cPort)
{
  st25_io.begin(i2cPort);
  return isConnected();
}

bool SFE_ST25DV64KC::isConnected()
{
  bool connected = st25_io.isConnected();
  return connected;
}

bool SFE_ST25DV64KC::readRegisterValue(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *value)
{
  return st25_io.readSingleByte(addressType, registerAddress, value);
}

bool SFE_ST25DV64KC::readRegisterValues(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *data, uint16_t dataLength)
{
  return st25_io.readMultipleBytes(addressType, registerAddress, data, dataLength);
}

bool SFE_ST25DV64KC::getDeviceUID(uint8_t *values)
{
  uint8_t tempBuffer[8] = {0};

  // Get UID into tempBuffer and return it from back to front
  bool success = st25_io.readMultipleBytes(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_UID_BASE, tempBuffer, 8);

  if (success)
  {
    for (uint8_t i = 0; i < 8; i++)
      values[i] = tempBuffer[7 - i];
  }

  return success;
}

bool SFE_ST25DV64KC::getDeviceRevision(uint8_t *value)
{
  return st25_io.readSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_IC_REV, value);
}

bool SFE_ST25DV64KC::openI2CSession(uint8_t *password)
{
  // Passwords are written MSB first and need to be sent twice with 0x09 sent after the first
  // set of 8 bytes.
  uint8_t tempBuffer[17] = {0};

  // First 8 bytes
  for (uint8_t i = 0; i < 8; i++)
    tempBuffer[i] = password[7 - i];

  // 9th byte - verification code
  tempBuffer[8] = 0x09;

  // remaining 8 bytes
  for (uint8_t i = 0; i < 8; i++)
    tempBuffer[i + 9] = tempBuffer[i];

  return st25_io.writeMultipleBytes(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2C_PASSWD_BASE, tempBuffer, 17);
}

bool SFE_ST25DV64KC::isI2CSessionOpen()
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, REG_I2C_SSO_DYN, BIT_I2C_SSO_DYN_I2C_SSO);
}

bool SFE_ST25DV64KC::writeI2CPassword(uint8_t *password)
{
  if (!isI2CSessionOpen())
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::I2C_SESSION_NOT_OPENED);
    return false;
  }

  // Disable Fast Transfer Mode (datasheet page 75)
  bool ftmIsSet = st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_FTM, BIT_FTM_MB_MODE);
  st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_FTM, BIT_FTM_MB_MODE);

  // Passwords are written MSB first and need to be sent twice with 0x07 sent after the first
  // set of 8 bytes.
  uint8_t tempBuffer[17] = {0};

  // First 8 bytes
  for (uint8_t i = 0; i < 8; i++)
    tempBuffer[i] = password[7 - i];

  // 9th byte - verification code
  tempBuffer[8] = 0x07;

  // remaining 8 bytes
  for (uint8_t i = 0; i < 8; i++)
    tempBuffer[i + 9] = tempBuffer[i];

  st25_io.writeMultipleBytes(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2C_PASSWD_BASE, tempBuffer, 17);

  if (ftmIsSet)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_FTM, BIT_FTM_MB_MODE);

  return true;
}

void SFE_ST25DV64KC::programEEPROMReadProtectionBit(uint8_t memoryArea, bool readSecured)
{
  if (memoryArea < 1 || memoryArea > 4)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::I2CSS_MEMORY_AREA_INVALID);
    return;
  }

  switch (memoryArea)
  {
  case 1:
  {
    if (readSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_READ);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_READ);
  }
  break;

  case 2:
  {
    if (readSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_READ);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_READ);
  }
  break;

  case 3:
  {
    if (readSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_READ);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_READ);
  }
  break;

  case 4:
  {
    if (readSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_READ);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_READ);
  }
  break;

  default:
    break;
  }
}

void SFE_ST25DV64KC::programEEPROMWriteProtectionBit(uint8_t memoryArea, bool writeSecured)
{
  if (memoryArea < 1 || memoryArea > 4)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::I2CSS_MEMORY_AREA_INVALID);
    return;
  }

  switch (memoryArea)
  {
  case 1:
  {
    if (writeSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_WRITE);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_WRITE);
  }
  break;

  case 2:
  {
    if (writeSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_WRITE);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_WRITE);
  }
  break;

  case 3:
  {
    if (writeSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_WRITE);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_WRITE);
  }
  break;

  case 4:
  {
    if (writeSecured)
      st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_WRITE);
    else
      st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_WRITE);
  }
  break;

  default:
    break;
  }
}

bool SFE_ST25DV64KC::getEEPROMReadProtectionBit(uint8_t memoryArea)
{
  if (memoryArea < 1 || memoryArea > 4)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::I2CSS_MEMORY_AREA_INVALID);
    return false;
  }

  switch (memoryArea)
  {
  case 1:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_READ);
  }
  break;

  case 2:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_READ);
  }
  break;

  case 3:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_READ);
  }
  break;

  case 4:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_READ);
  }
  break;

  default:
    break;
  }
  return false;
}

bool SFE_ST25DV64KC::getEEPROMWriteProtectionBit(uint8_t memoryArea)
{
  if (memoryArea < 1 || memoryArea > 4)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::I2CSS_MEMORY_AREA_INVALID);
    return false;
  }

  switch (memoryArea)
  {
  case 1:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM1_WRITE);
  }
  break;

  case 2:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM2_WRITE);
  }
  break;

  case 3:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM3_WRITE);
  }
  break;

  case 4:
  {
    return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_I2CSS, BIT_I2CSS_MEM4_WRITE);
  }
  break;

  default:
    break;
  }
  return false;
}

bool SFE_ST25DV64KC::writeEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength)
{
  // Disable FTM temporarily if enabled
  bool ftmEnabled = st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, REG_MB_CTRL_DYN, BIT_FTM_MB_MODE);

  bool success = true;

  if (ftmEnabled)
    success &= st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_FTM, BIT_FTM_MB_MODE);

  success &= st25_io.writeMultipleBytes(SF_ST25DV64KC_ADDRESS::DATA, baseAddress, data, dataLength);

  // Restore FTM if previously enabled
  if (ftmEnabled)
    success &= st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_FTM, BIT_FTM_MB_MODE);

  return success;
}

bool SFE_ST25DV64KC::readEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength)
{
  return st25_io.readMultipleBytes(SF_ST25DV64KC_ADDRESS::DATA, baseAddress, data, dataLength);
}

bool SFE_ST25DV64KC::setMemoryAreaEndAddress(uint8_t memoryArea, uint8_t endAddressValue)
{
  if (memoryArea < 1 || memoryArea > 3)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::INVALID_MEMORY_AREA_PASSED);
    return false;
  }

  bool success = true;
  switch (memoryArea)
  {
  case 1:
    success = st25_io.writeSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA1, endAddressValue);
    break;

  case 2:
    success = st25_io.writeSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA2, endAddressValue);
    break;

  case 3:
    success = st25_io.writeSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA3, endAddressValue);
    break;

  default:
    break;
  }

  if (!success)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::INVALID_MEMORY_AREA_SIZE);
    return false;
  }

  return success;
}

uint16_t SFE_ST25DV64KC::getMemoryAreaEndAddress(uint8_t memoryArea)
{
  if (memoryArea < 1 || memoryArea > 3)
  {
    SAFE_CALLBACK(errorCallback, SF_ST25DV64KC_ERROR::INVALID_MEMORY_AREA_PASSED);
    return 0;
  }

  uint8_t value = 0;

  switch (memoryArea)
  {
  case 1:
    st25_io.readSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA1, &value);
    break;

  case 2:
    st25_io.readSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA2, &value);
    break;

  case 3:
    st25_io.readSingleByte(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_ENDA3, &value);
    break;

  default:
    break;
  }

  return ((uint16_t)value * 32 + 31);
}

bool SFE_ST25DV64KC::RFFieldDetected()
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_EH_CTRL_DYN, BIT_EH_CTRL_DYN_FIELD_ON);
}

void SFE_ST25DV64KC::setGPO1Bit(uint8_t bitMask, bool enabled)
{
  if (enabled)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO1, bitMask);
  else
    st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO1, bitMask);
}

bool SFE_ST25DV64KC::getGPO1Bit(uint8_t bitMask)
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO1, bitMask);
}

void SFE_ST25DV64KC::setGPO2Bit(uint8_t bitMask, bool enabled)
{
  if (enabled)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO2, bitMask);
  else
    st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO2, bitMask);
}

bool SFE_ST25DV64KC::getGPO2Bit(uint8_t bitMask)
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_GPO2, bitMask);
}

void SFE_ST25DV64KC::setGPO_CTRL_DynBit(bool enabled)
{
  if (enabled)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_GPO_CTRL_DYN, BIT_GPO_CTRL_DYN_GPO_EN);
  else
    st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_GPO_CTRL_DYN, BIT_GPO_CTRL_DYN_GPO_EN);
}

bool SFE_ST25DV64KC::getGPO_CTRL_DynBit()
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_GPO_CTRL_DYN, BIT_GPO_CTRL_DYN_GPO_EN);
}

bool SFE_ST25DV64KC::getIT_STS_DynBit(uint8_t bitMask)
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, REG_IT_STS_DYN, bitMask);
}

void SFE_ST25DV64KC::setEH_MODEBit(bool value)
{
  if (value)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_EH_MODE, BIT_EH_MODE_EH_MODE);
  else
    st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_EH_MODE, BIT_EH_MODE_EH_MODE);
}

bool SFE_ST25DV64KC::getEH_MODEBit()
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::SYSTEM, REG_EH_MODE, BIT_EH_MODE_EH_MODE);
}

void SFE_ST25DV64KC::setEH_CTRL_DYNBit(uint8_t bitMask, bool value)
{
  if (value)
    st25_io.setRegisterBit(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_EH_CTRL_DYN, bitMask);
  else
    st25_io.clearRegisterBit(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_EH_CTRL_DYN, bitMask);
}

bool SFE_ST25DV64KC::getEH_CTRL_DYNBit(uint8_t bitMask)
{
  return st25_io.isBitSet(SF_ST25DV64KC_ADDRESS::DATA, DYN_REG_EH_CTRL_DYN, bitMask);
}
