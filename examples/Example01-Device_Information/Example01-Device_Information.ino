#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

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
    }
}

void loop()
{
}
