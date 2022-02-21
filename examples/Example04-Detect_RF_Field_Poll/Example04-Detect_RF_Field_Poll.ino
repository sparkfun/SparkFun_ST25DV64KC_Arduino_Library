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
    }
}

void loop()
{
    bool value = tag.RFFieldDetected();
    if(value)
        Serial.println("RF field detected.");
    else
        Serial.println("RF field not detected.");

    delay(1000);
}