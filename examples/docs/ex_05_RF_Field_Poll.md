# Example 5 - RF Field Detection (using Polling)

An example showing how to detect an RF field (using polling) using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Detecting an RF Field

## Detecting an RF Field

Example 5 is very simple. It does not need to open a security session as it is not changing any of the tag settings.
It simply polls whether an RF field is present using ```RFFieldDetected```. It does this inside the ```loop()``` so the code repeats automatically.

In this example, we poll the RF field every 250ms. The RF tag detection by your smart phone can be very fast.
So don't be surprised if you only see one or two detections each time your bring your phone near the tag. You can generate a
field for longer if you use the "NFC Tap" App to read the tag's memory.

```C++
void loop()
{
  bool value = tag.RFFieldDetected();
  if (value)
    Serial.println(F("RF field detected."));
  else
    Serial.println(F("RF field not detected."));

  delay(250);
}
```

The next example shows how to use interrupts for field detection.
