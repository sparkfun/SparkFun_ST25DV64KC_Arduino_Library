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
    Serial.println();
    Serial.println("Connect a digital multimeter between VEH(+) and GROUND(-) pins.");
    Serial.println("Whenever there's RF field a voltage will show up between VEH and GROUND.");
    Serial.println("Warning! Voltage will get higher than 3.3 volts so you may damage a 3.3V-only microcontroller!");
    Serial.println();
    Serial.println("Enabling energy harvesting now.");
    tag.setEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_EN, true);
    tag.setEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_ON, true);
    Serial.print("Energy harvesting bit is ");
    Serial.println(tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_EH_ON) ? "enabled." : "disabled.");
    Serial.print("VCC is ");
    Serial.println(tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_VCC_ON) ? "detected." : "not detected.");
    delay(1000);
  }
}

void loop()
{
  if (tag.getEH_CTRL_DYNBit(BIT_EH_CTRL_DYN_FIELD_ON))
    Serial.println("RF field detected.");
  else
    Serial.println("RF field not detected.");

  delay(250);
}
