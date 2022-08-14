# Example 11 - Memory Leak Test

An example showing how to check for memory leaks using the SparkFun ST25DV64KC Arduino Library.

## Key Features

- Checking for a memory leak by instantiating the tag object in the loop

## Memory Leak Test

Normally, the ```SFE_ST25DV64KC_NDEF tag``` object is instantiated once, at global scale, so that both ```setup()``` and ```loop()``` can access it.

Example 11 instantiates the tag object _inside_ ```loop()``` to check for possible memory leaks. The tag object is destructed and re-instantiated each time around the loop.
If the library were not cleaning up the memory correctly, we would soon see out-of-memory errors on platforms with limited RAM - like the good old ATmega328P.

!!! note
    Do not leave this example running for a prolonged period. The tag EEPROM has a lifetime of 400000 to 1000000 write cycles - depending on temperature. This example could quickly burn through those!

!!! note
    The example writes multiple NDEF URI, WiFi and Text records. If you disconnect the power during a write, the memory may end up 'corrupted' and not able to be read with NFC Tap. Re-run one of the earlier examples to reset the memory if required.



