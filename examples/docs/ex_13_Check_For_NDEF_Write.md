# Example 13 - Check For NDEF Write

An example showing how to check if a new NDEF record has been written to the tag, using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Writing the tag's Capability Container (CC)
- Writing an Empty NDEF Record
- Waiting for a key press - to say it is 'safe' to read the tag with I<sup>2</sup>C
- Checking for the writing of new URI, WiFi or Text records

## Interface Arbitration

The ST25DV tag has a clever "interface arbitration" feature. If an I<sup>2</sup>C transaction is in progress, RF transactions are 'blocked'. And vice versa.

This is obviously a good thing! But there are some complications to be aware of. It looks like the ST "NFC Tap" App uses a read-modify-write
approach when adding new records to tag memory. We think it does this using separate RF transactions or 'sessions'. The App gets very
confused if the tag is busy servicing a _new_ I<sup>2</sup>C transaction (memory read) when it tries to do the RF 'write' after the 'modify'...

Note: this is not an issue with the tag itself. The tag is very robust. It is an issue of how the App - and this example - have been written.

The simple solution is to only read the tag using I<sup>2</sup>C when you know it is 'safe' to do so. I.e. when you know there is no RF activity taking place.
In this example, the user presses a key (sends a single Serial character) to indicate the tag should be read.

The next example shows how to avoid most I<sup>2</sup>C and RF 'collisions' using the GPO pin.

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

## Checking for a Key Press

You will have seen this code many times before. It is simple code which: empties the Serial RX buffer; and checks for the arrival of any new characters:

```C++
  while (Serial.available()) // Empty the Serial buffer
    Serial.read();
    
  Serial.println(F("Please press any key to check for new records"));
  Serial.println();

  while (!Serial.available())
    ; // Wait for the user to press a key (send any character)
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

The code reads _all_ new records by performing all three `readNDEF` calls inside a ```while``` loop.
`keepGoing` is set to ```true``` initially so the loop can be entered. Inside the loop, it is always set to ```false```
but is then set set back to ```true``` if any one `readNDEF` returns ```true```.
This way the code will stay in the `while` loop until all new records have been read and all three `readNDEF` methods have returned ```false```.

```C++
  bool keepGoing = true;

  while (keepGoing)
  {
    keepGoing = false;
    
    // Check for a new NDEF URI record
    if (tag.readNDEFURI(thing1, 50, uriRecord))
    {
      Serial.println(F("New URI Record Found:"));
      Serial.println(thing1);
      Serial.println();
      uriRecord++; // Increase the count
      keepGoing = true;
    }
  
    // Check for a new NDEF WiFi record
    if (tag.readNDEFWiFi(thing1, 50, thing2, 50, wifiRecord))
    {
      Serial.println(F("New WiFi Record Found:"));
      Serial.print(F("    SSID:"));
      Serial.println(thing1);
      Serial.print(F("Password:"));
      Serial.println(thing2);
      Serial.println();
      wifiRecord++; // Increase the count
      keepGoing = true;
    }
  
    // Check for a new NDEF Text record
    if (tag.readNDEFText(thing1, 50, textRecord))
    {
      Serial.println(F("New Text Record Found:"));
      Serial.println(thing1);
      Serial.println();
      textRecord++; // Increase the count
      keepGoing = true;
    }
  }
```