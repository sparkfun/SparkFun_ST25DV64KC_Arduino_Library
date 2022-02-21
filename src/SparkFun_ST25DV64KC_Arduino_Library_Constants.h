/*
  This is a library written for the ST25DV64KC Dynamic RFID Tag.
  SparkFun sells these at its website:
  https://www.sparkfun.com/products/

  Do you like this library? Help support open source hardware. Buy a board!

  Written by Ricardo Ramos  @ SparkFun Electronics, January 6th, 2021
  This file declares all constants used in the ST25DV64KC Dynamic RFID Tag Arduino Library.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SPARKFUN_ST25DV64KC_CONSTANTS_
#define _SPARKFUN_ST25DV64KC_CONSTANTS_

#include <Arduino.h>

// Macro for invoking the callback if the function pointer is valid
#define SAFE_CALLBACK(cb, code) \
    if (cb != nullptr)          \
    {                           \
        cb(code);               \
    }

// Registers definitions
static const uint16_t REG_GPO1              = 0x0;
static const uint16_t REG_GPO2              = 0x0001;
static const uint16_t REG_EH_MODE           = 0x0002;
static const uint16_t REG_RF_MNGMT          = 0x0003;
static const uint16_t REG_RFA1SS            = 0x0004;
static const uint16_t REG_ENDA1             = 0x0005;
static const uint16_t REG_RFA2SS            = 0x0006;
static const uint16_t REG_ENDA2             = 0x0007;
static const uint16_t REG_RFA3SS            = 0x0008;
static const uint16_t REG_ENDA3             = 0x0009;
static const uint16_t REG_RFA4SS            = 0x000a;
static const uint16_t REG_I2CSS             = 0x000b;
static const uint16_t REG_LOCK_CCFILE       = 0x000c;
static const uint16_t REG_FTM               = 0X000d;
static const uint16_t REG_I2C_CFG           = 0x000e;
static const uint16_t REG_LOCK_CFG          = 0x000f;
static const uint16_t REG_LOCK_DSFID        = 0x0010;
static const uint16_t REG_LOCK_AFI          = 0x0011;
static const uint16_t REG_DSFID             = 0x0012;
static const uint16_t REG_AFI               = 0x0013;
static const uint16_t REG_MEM_SIZE_BASE     = 0x0014;
static const uint16_t REG_BLOCK_SIZE        = 0x0016;
static const uint16_t REG_IC_REF            = 0x0017;
static const uint16_t REG_UID_BASE          = 0x0018;
static const uint16_t REG_IC_REV            = 0x0020;
static const uint16_t REG_I2C_PASSWD_BASE   = 0x0900;

// Multi-byte registers lengths
static const uint8_t  LEN_MEM_SIZE          = 0x02;
static const uint8_t  LEN_UID_SIZE          = 0x08;
static const uint8_t  LEN_I2C_PASSWD_SIZE   = 0x08;

// Dynamic registers
static const uint16_t DYN_REG_GPO_CTRL_DYN  = 0x2000;
static const uint16_t DYN_REG_EH_CTRL_DYN   = 0x2002;
static const uint16_t DYN_REG_RF_MNGT_DYN   = 0x2003;
static const uint16_t REG_I2C_SSO_DYN       = 0x2004;
static const uint16_t REG_IT_STS_DYN        = 0x2005;
static const uint16_t REG_MB_CTRL_DYN       = 0x2006;
static const uint16_t REG_MB_LEN_DYN        = 0x2007;

// Mailbox registers
static const uint16_t MAILBOX_BASE          = 0x2008;

// Mailbox length
static const uint8_t  LEN_MAILBOX           = 0xff;       

// EEPROM size
static const uint16_t EEPROM_SIZE           = 0x2000;

// Registers' bits definitions
#define BIT_FTM_MB_MODE                         (1 << 0)

#define BIT_MB_CTRL_DYN_MB_EN                   (1 << 0)
#define BIT_MB_CTRL_DYN_HOST_PUT_MSG            (1 << 1)
#define BIT_MB_CTRL_DYN_RF_PUT_MSG              (1 << 2)
#define BIT_MB_CTRL_DYN_HOST_MISS_MSG           (1 << 4)
#define BIT_MB_CTRL_DYN_RF_MISS_MSG             (1 << 5)
#define BIT_MB_CTRL_DYN_HOST_CURRENT_MSG        (1 << 6)
#define BIT_MB_CTRL_DYN_RF_CURRENT_MSG          (1 << 7)

#define BIT_RF_MNGT_RF_DISABLE                  (1 << 0)
#define BIT_RF_MNGT_RF_SLEEP                    (1 << 1)

#define BIT_RF_MNGT_DYN_RF_DISABLE              (1 << 0)
#define BIT_RF_MNGT_DYN_RF_SLEEP                (1 << 1)
#define BIT_RF_MNGT_DYN_RF_OFF                  (1 << 2)

#define BIT_GPO1_GPO_EN                         (1 << 0)
#define BIT_GPO1_RF_USER_EN                     (1 << 1)
#define BIT_GPO1_RF_ACTIVITY_EN                 (1 << 2)
#define BIT_GPO1_RF_INTERRUPT_EN                (1 << 3)
#define BIT_GPO1_FIELD_CHANGE_EN                (1 << 4)
#define BIT_GPO1_RF_PUT_MSG_EN                  (1 << 5)
#define BIT_GPO1_RF_GET_MSG_EN                  (1 << 6)
#define BIT_GPO1_RF_WRITE_EN                    (1 << 7)

#define BIT_GPO2_I2C_WRITE_EN                   (1 << 0)
#define BIT_GPO2_I2C_RF_OFF_EN                  (1 << 1)

#define BIT_GPO_CTRL_DYN_GPO_EN                 (1 << 0)

#define BIT_IT_STS_DYN_RF_USER                  (1 << 0)
#define BIT_IT_STS_DYN_RF_ACTIVITY              (1 << 1)
#define BIT_IT_STS_DYN_RF_INTERRUPT             (1 << 2)
#define BIT_IT_STS_DYN_FIELD_FALLING            (1 << 3)
#define BIT_IT_STS_DYN_FIELD_RISING             (1 << 4)
#define BIT_IT_STS_DYN_RF_PUT_MSG               (1 << 5)
#define BIT_IT_STS_DYN_RF_GET_MSG               (1 << 6)
#define BIT_IT_STS_DYN_RF_WRITE                 (1 << 7)

#define BIT_EH_MODE_EH_MODE                     (1 << 0)

#define BIT_EH_CTRL_DYN_EH_EN                   (1 << 0)
#define BIT_EH_CTRL_DYN_EH_ON                   (1 << 1)
#define BIT_EH_CTRL_DYN_FIELD_ON                (1 << 2)
#define BIT_EH_CTRL_DYN_VCC_ON                  (1 << 3)

#define BIT_LOCK_CCFILE_LCKBCK0                 (1 << 0)
#define BIT_LOCK_CCFILE_LCKBCK1                 (1 << 1)

#define BIT_LOCK_CFG_LCK_CFG                    (1 << 0)

#define BIT_I2C_SSO_DYN_I2C_SSO                 (1 << 0)

#define BIT_LOCK_DSFID_LOCK_DSFID               (1 << 0)

#define BIT_LOCK_AFI_LOCK_AFI                   (1 << 0)

#define BIT_I2CSS_MEM1_WRITE                    (1 << 0)
#define BIT_I2CSS_MEM1_READ                     (1 << 1)
#define BIT_I2CSS_MEM2_WRITE                    (1 << 2)
#define BIT_I2CSS_MEM2_READ                     (1 << 3)
#define BIT_I2CSS_MEM3_WRITE                    (1 << 4)
#define BIT_I2CSS_MEM3_READ                     (1 << 5)
#define BIT_I2CSS_MEM4_WRITE                    (1 << 6)
#define BIT_I2CSS_MEM4_READ                     (1 << 7)

enum class SF_ST25DV64KC_ADDRESS : uint8_t
{
    DATA    = 0x53,             // E2 = 0, E1 = 1
    SYSTEM  = 0x57,             // E2 = 1, E1 = 1
    RF_SWITCH_OFF = 0x51,       // E2 = 0, E1 = 0
    RF_SWITCH_ON = 0x55,        // E2 = 1, E1 = 0
};

enum class SF_ST25DV64KC_ERROR
{
    NONE,
    I2C_INITIALIZATION_ERROR,
    INVALID_DEVICE,
    I2C_SESSION_NOT_OPENED,
    I2CSS_MEMORY_AREA_INVALID,
    INVALID_WATCHDOG_VALUE,
    INVALID_MEMORY_AREA_PASSED,
    INVALID_MEMORY_AREA_SIZE,
};

enum class END_REGISTERS
{
    ENDA1,
    ENDA2,
    ENDA3,
    ENDA4
};

#endif