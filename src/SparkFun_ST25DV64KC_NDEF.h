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

class SFE_ST25DV64KC_NDEF : public SFE_ST25DV64KC
{
public:
  // Default constructor.
  SFE_ST25DV64KC_NDEF(){};

  // Default destructor.
  ~SFE_ST25DV64KC_NDEF(){};

  // Write a 4-byte CC File to user memory (e.g. ST25DV04K)
  bool writeCCFile4Byte(uint32_t val = 0xE1403F00);
  
  // Write an 8-byte CC File to user memory (ST25DV64K)
  bool writeCCFile8Byte(uint32_t val1 = 0xE2400001, uint32_t val2 = 0x000003FF);
  
};

#endif
