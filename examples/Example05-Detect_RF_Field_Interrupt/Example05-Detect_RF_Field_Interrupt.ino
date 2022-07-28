#include <SparkFun_ST25DV64KC_Arduino_Library.h> // Click here to get the library:  http://librarymanager/All#SparkFun_ST25DV64KC

SFE_ST25DV64KC tag;

// Use a jumper cable to link the ST25DV64KC GPO1 pin to a digital pin
const uint8_t GPO_PIN = 2; // Change this to match the digital pin you have linked GPO1 to

static volatile bool interruptChanged = false;

void setup()
{
    delay(1000);
    
    Serial.begin(115200);
    Wire.begin();

    Serial.println("ST25DV64KC example.");
    
    pinMode(GPO_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(GPO_PIN), ISR, CHANGE);

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

        Serial.println("Configuring GPO1 to toggle on field change only.");
        tag.setGPO1Bit(BIT_GPO1_FIELD_CHANGE_EN, true);
        Serial.println("Disabling other bits.");
        tag.setGPO1Bit(BIT_GPO1_RF_USER_EN, false);
        tag.setGPO1Bit(BIT_GPO1_RF_ACTIVITY_EN, false);
        tag.setGPO1Bit(BIT_GPO1_RF_INTERRUPT_EN, false);
        tag.setGPO1Bit(BIT_GPO1_RF_PUT_MSG_EN, false);
        tag.setGPO1Bit(BIT_GPO1_RF_GET_MSG_EN, false);
        tag.setGPO1Bit(BIT_GPO1_RF_WRITE_EN, false);
        Serial.println("Enabling GPO_EN bit.");
        tag.setGPO1Bit(BIT_GPO1_GPO_EN, true);
    }
}

void ISR()
{
    interruptChanged = true;
}

void loop()
{
    static uint16_t counter = 0;

    if(interruptChanged == true)
    {
        interruptChanged = false;
        Serial.print("RF field change detected ");
        Serial.print(++counter);
        Serial.println(" times.");
    }
}
