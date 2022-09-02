# Example 14 - Wait For NDEF Write

An example showing how to wait for a new NDEF record to be written to the tag, using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Writing the tag's Capability Container (CC)
- Writing an Empty NDEF Record
- (Optionally) Configuring the GPO pin to indicate RF activity
- Checking for the writing of new URI, WiFi or Text records

## Using the GPO pin to indicate RF activity - and delay the next I<sup>2</sup>C transaction

The ST25DV tag has a clever "interface arbitration" feature. If an I<sup>2</sup>C transaction is in progress, RF transactions are 'blocked'. And vice versa.

This is obviously a good thing! But there are some complications to be aware of. It looks like the ST "NFC Tap" App uses a read-modify-write
approach when adding new records to tag memory. We think it does this using separate RF transactions or 'sessions'. The App gets very
confused if the tag is busy servicing a _new_ I<sup>2</sup>C transaction (memory read) when it tries to do the RF 'write' after the 'modify'...

Note: this is not an issue with the tag itself. The tag is very robust. It is an issue of how the App - and this example - have been written and interact.

A work-around is to use the GPO pin to indicate when RF activity is taking place and to use that to delay the next I<sup>2</sup>C transaction for a few seconds.

This feature is disabled by default. You can enable it by uncommenting this line:

```C++
//#define useGPOpin // Uncomment this line if you want to use the GPO pin to detect RF activity
```

With `useGPOpin` enabled, the GPO pin is configured to indicate an RF User session and/or RF Activity:

```C++
#ifdef useGPOpin

  // The GPO registers can only be changed during an open security session
  Serial.println(F("Opening I2C session with password."));
  uint8_t password[8] = {0x0};
  tag.openI2CSession(password);

  Serial.print(F("I2C session is "));
  Serial.println(tag.isI2CSessionOpen() ? "opened." : "closed.");

  // -=-=-=-=-=-=-=-=-

  Serial.println(F("Configuring GPO1 to indicate RF user & activity."));
  tag.setGPO1Bit(BIT_GPO1_RF_USER_EN, true);
  tag.setGPO1Bit(BIT_GPO1_RF_ACTIVITY_EN, true);
  
  Serial.println(F("Disabling other bits."));
  tag.setGPO1Bit(BIT_GPO1_RF_INTERRUPT_EN, false);
  tag.setGPO1Bit(BIT_GPO1_FIELD_CHANGE_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_PUT_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_GET_MSG_EN, false);
  tag.setGPO1Bit(BIT_GPO1_RF_WRITE_EN, false);

  // -=-=-=-=-=-=-=-=-

  Serial.println(F("Enabling GPO_EN bit."));
  tag.setGPO1Bit(BIT_GPO1_GPO_EN, true);

#endif
```

A digital pin is used to read the status of the GPO pin. The GPO pin goes low during RF activity and the code uses that to reset a timer.
Reading the tag's EEPROM memory - to see if a new record has been added - is delayed for at least two seconds from the last RF activity.
This gives the App time to complete its read-modify-write once it has started.

```C++
#ifdef useGPOpin
  static unsigned long lastActivity = 0; // Keep track of the last RF activity

  if (digitalRead(GPO_PIN) == LOW) // GPO will be low when there is RF activity
  {
    lastActivity = millis(); // Record the last RF activity
  }

  // Only try to communicate with the tag when there has been no RF activity for at least two seconds
  if (millis() > (lastActivity + 2000))
#endif
```

This prevents _most_ App confusion. You may still see occasions where the App thinks there is no NDEF record present. But this only happens when
a new RF transaction is attempted while the tag is already performing an I<sup>2</sup>C transaction (memory read). Once the RF read-modify-write is underway, the two second
timer prevents the next I<sup>2</sup>C transaction from interfering with it.

**It also helps a lot if you bring your phone near the tag first _and then_ press the "Write to Tag" button.** Bringing your phone near the tag generates a field
which causes the tag to go into RF mode early. This locks out I<sup>2</sup>C, ahead of you pushing the button to start the actual RF write.

## Writing an Empty NDEF Record

Please see the previous example for details.

## Checking for new NDEF Records

Please see the previous example for details.

