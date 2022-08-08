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

#ifndef _SPARKFUN_ST25DV64KC_NDEF_
#define _SPARKFUN_ST25DV64KC_NDEF_

#include "SparkFun_ST25DV64KC_Arduino_Library.h"
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

class SFE_ST25DV64KC_NDEF : public SFE_ST25DV64KC
{
private:
  uint16_t _ccFileLen = 8; // Record the length of the CC File - default to 8 bytes for the ST25DV64K

public:
  // Default constructor.
  SFE_ST25DV64KC_NDEF(){};

  // Default destructor.
  ~SFE_ST25DV64KC_NDEF(){};

  // Write a 4-byte CC File to user memory (e.g. ST25DV04K)
  // Returns true if successful, otherwise false
  bool writeCCFile4Byte(uint32_t val = 0xE1403F00);
  
  // Write an 8-byte CC File to user memory (ST25DV64K)
  // Returns true if successful, otherwise false
  bool writeCCFile8Byte(uint32_t val1 = 0xE2400001, uint32_t val2 = 0x000003FF);

  // Update _ccFileLen
  void setCCFileLen(uint16_t newLen) { _ccFileLen = newLen; }
  uint16_t getCCFileLen() { return _ccFileLen; }
  
  // Write an NDEF URI Record to user memory
  // If address is not NULL, start writing at *address, otherwise start at _ccFileLen
  // MB = Message Begin, ME = Message End
  // Default is a single message (MB=true, ME=true)
  // To add multiple URIs:
  //   First: MB=true, ME=false
  //   Intermediate: MB=false, ME=false
  //   Last: MB=false, ME=true
  // Returns true if successful, otherwise false
  bool writeNDEFURI(const char *uri, uint8_t idCode = SFE_ST25DV_NDEF_URI_ID_CODE_NONE, uint16_t *address = NULL, bool MB = true, bool ME = true);

  // Write an NDEF WiFi Record to user memory
  // If address is not NULL, start writing at *address, otherwise start at _ccFileLen
  // MB = Message Begin, ME = Message End
  // Default is a single message (MB=true, ME=true)
  // To add multiple URIs:
  //   First: MB=true, ME=false
  //   Intermediate: MB=false, ME=false
  //   Last: MB=false, ME=true
  // Returns true if successful, otherwise false
  bool writeNDEFWiFi(const char *ssid, const char *passwd, uint16_t *address = NULL, bool MB = true, bool ME = true,
                     const uint8_t auth[2] = SFE_ST25DV_WIFI_AUTH_WPA2_PERSONAL, const uint8_t encrypt[2] = SFE_ST25DV_WIFI_ENCRYPT_AES);

  // Read an NDEF WiFi Record from memory
  // Default is to read the first WiFi record (recordNo = 1). Increase recordNo to read later entries
  // maxSsidLen is the maximum number of chars which ssid can hold
  // maxPasswdLen is the maximum number of chars which passwd can hold
  // Returns true if successful, otherwise false
  bool readNDEFWiFi(char *ssid, uint8_t maxSsidLen, char *passwd, uint8_t maxPasswdLen, uint8_t recordNo = 1);

  // Write an NDEF UTF-8 Text Record to user memory
  // If address is not NULL, start writing at *address, otherwise start at _ccFileLen
  // MB = Message Begin, ME = Message End
  // Default is a single message (MB=true, ME=true)
  // To add multiple URIs:
  //   First: MB=true, ME=false
  //   Intermediate: MB=false, ME=false
  //   Last: MB=false, ME=true
  // If language is not NULL, language is copied into the Text Record, otherwise "en" is used
  // Returns true if successful, otherwise false
  bool writeNDEFText(const char *theText, uint16_t *address, bool MB, bool ME, const char *language = NULL);

  // Read an NDEF UTF-8 Text Record from memory
  // Default is to read the first Text record (recordNo = 1). Increase recordNo to read later entries
  // maxTextLen is the maximum number of chars which theText can hold
  // If language is not NULL, the Language Code will be copied into language
  // maxLanguageLen is the maximum number of chars which language can hold
  // Returns true if successful, otherwise false
  bool readNDEFText(char *theText, uint16_t maxTextLen, uint8_t recordNo = 1, char *language = NULL, uint16_t maxLanguageLen = 0);
};

#endif
