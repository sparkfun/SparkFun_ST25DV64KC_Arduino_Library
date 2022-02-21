#include "SparkFun_ST25DV64KC_Arduino_Library.h"
#include "SparkFun_ST25DV64KC_Arduino_Library_Constants.h"

SFE_ST25DV64KC tag;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    if (tag.begin(Wire))
    {
        uint8_t values[8] = {0};
        Serial.println("ST25 connected.");
        Serial.print("Device UID: ");
        tag.getDeviceUID(values);
        for (uint8_t i = 0; i < 8; i++)
        {
            if (values[i] < 0x0a)
                Serial.print("0");
            Serial.print(values[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        Serial.print("Revision: ");
        Serial.println(tag.getDeviceRevision());

        Serial.println("Opening I2C session with password.");
        uint8_t password[8] = {0x0};
        tag.openI2CSession(password);

        Serial.print("I2C session is ");
        Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

        char endAddress[5] = {0};

        Serial.println();

        Serial.print("Area 1 ending address before: 0x");
        sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
        Serial.println(endAddress);

        // This function will succeed if ENDA1 value is less than ENDA2 or ENDA3.
        // Further information can be found on the datasheet on page 14.
        // Memory address will be: 0x20 * ENDAi + 0x1f which in this case results
        // in  0x20 * 0x10 + 0x1f = 0x021f
        if (tag.setMemoryAreaEndAddress(1, 0x10))
        {
            Serial.println("Success setting Area 1 end memory address.");
        }
        else
        {
            Serial.println("Error setting Area 1 end memory address.");
        }
        memset(endAddress, 0, 5);
        Serial.print("Area 1 ending address after: 0x");
        sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(1));
        Serial.println(endAddress);

        Serial.println();

        memset(endAddress, 0, 5);
        Serial.print("Area 2 ending address before: 0x");
        sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(2));
        Serial.println(endAddress);

        // This function will fail since ENDA2 value is less than if ENDA1 programmed above.
        // Further information can be found on the datasheet on page 14.
        // Memory address will be: 0x03 * ENDAi + 0x1f which in this case results
        // in 0x03 * 0x10 + 0x1f = 0x004f
        if (tag.setMemoryAreaEndAddress(2, 0x03))
        {
            Serial.println("Success setting Area 2 end memory address.");
        }
        else
        {
            Serial.println("Error setting Area 2 end memory address.");
        }

        memset(endAddress, 0, 5);
        Serial.print("Area 2 ending address after: 0x");
        sprintf(endAddress, "%04x", tag.getMemoryAreaEndAddress(2));
        Serial.println(endAddress);
    }
}

void loop()
{
}