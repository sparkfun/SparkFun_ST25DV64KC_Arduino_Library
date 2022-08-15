# Example 13 - Wait For NDEF Write

An example showing how to check if a new NDEF record has been written to the tag, using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Writing the tag's Capability Container (CC)
- Writing an Empty NDEF Record
- (Optionally) Configuring the GPO pin to indicate RF activity
- Checking for the writing of new URI, WiFi or Text records

## Using the GPO pin to indicate RF activity - and delay the next I2C transaction

The ST25DV tag has a clever "interface arbitration" feature. If an I2C transaction is in progress, RF transactions are 'blocked'. And vice versa.

This is obviously a good thing! But there are some complications to be aware of. It looks like the ST "NFC Tap" App uses a read-modify-write
approach when adding new records to tag memory. We think it does this using separate RF transactions or 'sessions'. The App gets very
confused if the tag is busy servicing a new I2C transaction (memory read) when it tries to do the RF 'write' after the 'modify'...

Note: this is not an issue with the tag itself. The tag is very robust. It is an issue of how the App - and this example - have been written.

A work-around is to use the GPO pin to indicate when RF activity is taking place and to use that to delay the next I2C transaction for a few seconds.

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
Reading the tag's EEPROM memory - to see if a new record has been added - is delayed for up to two seconds from the last RF activity.

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

This prevents most App confusion. You may still see occasions where the App thinks there is no NDEF record present. But this only happens when
a new RF transaction is attempted while the tag is already performing an I2C transaction (memory read). Once the RF read-modify-write is underway, the two second
timer prevents the next I2C transaction from interfering with it.

## Writing an Empty NDEF Record

After clearing the first 256 bytes of tag memory and writing the CC File, the code creates an empty NDEF record to keep the App happy.

```C++
/*
  To create an empty NDEF record:

  0x03 0x03 0xD0 0x00 0x00 0xFE

  Byte 0: Type5 Tag TLV-Format: T (Type field)
          0x03 = NDEF Message TLV
  Byte 1: Type5 Tag TLV-Format: L (Length field) (1-Byte Format)
          0x03 = 3 Bytes
  Bytes 2-4: Type5 Tag TLV-Format: V (Value field)
          Byte 2: Record Header = 0xD0
                  b7 = 0b1 MB (Message Begin)
                  b6 = 0b1 ME (Message End)
                  b5 = 0b0 CF (Chunk Flag)
                  b4 = 0b1 SR (Short Record)
                  b3 = 0b0 IL (ID Length)
                  b2 b1 b0 = 0b000 TNF (Type Name Format): Empty
          Byte 3: Type Length
                  0x00 = 0 Bytes
          Byte 4: Payload Length
                  0x00 = 0 bytes
  Byte 5: Type5 Tag TLV-Format: T (Type field)
          0xFE = Terminator TLV
*/
```

The code to create the empty record is:

```C++
  // Add an empty record at the first memory location after the CC File
  Serial.println(F("Writing an empty (zero-length) TLV Record"));
  uint16_t memoryLocation = tag.getCCFileLen(); // Write to the memory location immediately after the CC File
  tag.writeNDEFEmpty(&memoryLocation);
```

## Checking for new NDEF Records

It is possible to write multiple URI, WiFi and Text records into a single tag and for the code to detect when each is written.

The code uses three `static` counters to keep track of how many URI, WiFi and Text records have already been processed:

```C++
  static uint8_t uriRecord = 1; // Keep track of how many URIs have been read
  static uint8_t wifiRecord = 1; // Keep track of how many WiFi records have been read
  static uint8_t textRecord = 1; // Keep track of how many Text records have been read
```

By passing the appropriate counter into the `readNDEF` function, the code can ignore existing entries and only return ```true``` when
a new entry is found:

```C++
  if (tag.readNDEFURI(thing1, 50, uriRecord))
    {
      Serial.println(F("URI Record Found!"));
      Serial.println(thing1);
      uriRecord++; // Increase the count
    }
```
