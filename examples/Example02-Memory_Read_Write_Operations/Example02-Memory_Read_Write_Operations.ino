#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC tag;

void setup()
{
    delay(1000);
    
    Serial.begin(115200);
    Wire.begin();

    Serial.println("ST25DV64KC example.");
    
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

        Serial.println("EEPROM area 1 will always be readable (datasheet page 60).");
        Serial.print("EEPROM area 1 write protection: ");
        Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

        Serial.println("Protecting area 1 for write operation.");
        tag.programEEPROMWriteProtectionBit(1, true);

        Serial.print("EEPROM area 1 read protection: ");
        Serial.println(tag.getEEPROMReadProtectionBit(1) ? "protected." : "opened.");
        Serial.print("EEPROM area 1 write protection: ");
        Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

        uint8_t tagRead[16] = {0};
        Serial.print("Reading values starting on 0x0 with opened session: ");
        tag.readEEPROM(0x0, tagRead, 16);
        for (auto value : tagRead)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();

        // Write 16 bytes from EEPROM location 0x0
        uint8_t tagWrite[16] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};

        Serial.print("Writing values starting on 0x0 with opened session: ");
        for (auto value : tagWrite)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();
        tag.writeEEPROM(0x0, tagWrite, 16);
        
        memset(tagRead, 0, 16);
        Serial.print("Reading values starting on 0x0 with opened session: ");
        tag.readEEPROM(0x0, tagRead, 16);
        for (auto value : tagRead)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();

        Serial.println("Closing I2C session.");
        password[1] = {0x10};
        tag.openI2CSession(password);

        Serial.print("I2C session is ");
        Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

        // Try to write 16 random bytes from EEPROM location 0x0
        randomSeed(analogRead(A0));
        for (uint8_t i = 0; i < 16; i++)
            tagWrite[i] = (uint8_t)random(0, 0xff);

        Serial.print("Writing values starting on 0x0 with closed session: ");
        for (auto value : tagWrite)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();
        tag.writeEEPROM(0x0, tagWrite, 16);

        Serial.print("Reading values starting on 0x0 with closed session: ");
        memset(tagRead, 0, 16);
        tag.readEEPROM(0x0, tagRead, 16);
        for (auto value : tagRead)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();

        Serial.println("Opening I2C session.");
        password[1] = {0x0};
        tag.openI2CSession(password);
        Serial.print("I2C session is ");
        Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

        Serial.println("Unprotecting area 1 for write operation.");
        tag.programEEPROMWriteProtectionBit(1, false);

        Serial.print("EEPROM area 1 write protection: ");
        Serial.println(tag.getEEPROMWriteProtectionBit(1) ? "protected." : "opened.");

        Serial.print("Writing zeros starting on 0x0 with opened session: ");
        memset(tagWrite, 0, 16);
        for (auto value : tagWrite)
        {
            Serial.print("0x");
            if (value < 0x10)
                Serial.print("0");
            Serial.print(value, HEX);
            Serial.print(" ");
        }
        Serial.println();
        tag.writeEEPROM(0x0, tagWrite, 16);
    }
}

void loop()
{
}
