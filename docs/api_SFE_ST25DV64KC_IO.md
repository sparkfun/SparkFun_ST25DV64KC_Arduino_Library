# API Reference for the SFE_ST25DV64KC_IO class

## Brief Overview

The ```SFE_ST25DV64KC_IO``` class provides the interface to the ST25DV hardware via I2C. It provides methods to: read and write single and multiple register values,
set or clear individual register bits, and confirm if a register bit is set.

## Initialization

### begin()

This method records the specified TwoWire port and uses that for all future I2C communication.

It also calls ```isConnected()``` and returns the result, confirming whether the ST25DV has been detected successfully.

```C++
bool begin(TwoWire &wirePort)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `wirePort` | `TwoWire &` | The I2C port to be used to communicate with the ST25DV |
| return value | `bool` | ```true``` if the ST25DV is detected, otherwise ```false``` |

### isConnected()

This method confirms if the ST25DV is connected by polling one of its I2C addresses.

```C++
bool isConnected()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if the ST25DV is detected, otherwise ```false``` |

## Register Read / Write

### readSingleByte()

This method reads a single register and returns the contents.

```C++
bool readSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *value)
```

Possible values for `address` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The register address |
| `value` | `uint8_t *` | `value` will hold the register value on return |
| return value | `bool` | ```true``` if the read was successful, otherwise ```false``` |

### writeSingleByte()

This method writes a value to the specified register.

```C++
bool writeSingleByte(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t value)
```

Possible values for `address` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The register address |
| `value` | `const uint8_t` | The value to be written |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### readMultipleBytes()

This method reads and returns multiple register values, starting at `registerAddress`.

!!! note
    `buffer` must be sufficiently large to hold all `packetLength` bytes.

```C++
bool readMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength)
```

Possible values for `address` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The start register address |
| `buffer` | `uint8_t *const` | A pointer to the array of uint8_t which will hold the read values |
| `packetLength` | `const uint16_t` | The number of registers to be read |
| return value | `bool` | ```true``` if the read was successful, otherwise ```false``` |

### writeMultipleBytes()

This method writes values to multiple registers, starting at `registerAddress`.

```C++
bool writeMultipleBytes(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, uint8_t *const buffer, const uint16_t packetLength)
```

Possible values for `address` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The start register address |
| `buffer` | `uint8_t *const` | A pointer to the array of uint8_t which holds the values to be written |
| `packetLength` | `const uint16_t` | The number of values to be written |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

## Register Bit Manipulation

### setRegisterBit()

This method will set the bit(s) defined by `bitMask` in the selected register.

This method uses a read-modify-write to set the bit(s).

```C++
bool setRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The register address |
| `bitMask` | `const uint8_t` | Defines which bits will be set. Each bit set in `bitMask` will cause the corresponding bit in `registerAddress` to be set |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### clearRegisterBit()

This method will clear the bit(s) defined by `bitMask` in the selected register.

This method uses a read-modify-write to clear the bit(s).

```C++
bool clearRegisterBit(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The register address |
| `bitMask` | `const uint8_t` | Defines which bits will be cleared. Each bit set in `bitMask` will cause the corresponding bit in `registerAddress` to be cleared |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### isBitSet()

This method returns ```true``` if the bit selected in `bitMask` is set.

!!! note
    If multiple bits are set in `bitMask`, the method will return ```true``` if _any one of those bits_ is set.

```C++
bool isBitSet(const SF_ST25DV64KC_ADDRESS address, const uint16_t registerAddress, const uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `address` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I2C address |
| `registerAddress` | `const uint16_t` | The register address |
| `bitMask` | `const uint8_t` | Defines which bits will be tested |
| return value | `bool` | ```true``` if the bit defined by `bitMask` is set, otherwise ```false``` |

## Member Variables

!!! note
    When the ST25DV is busy writing to its internal `DATA` memory, it will NACK any further attempted I2C transmissions until the write is complete. The write duration is variable and depends on whether the write crosses block boundaries. Therefore, the library implements a retry strategy. Each read or write will be attempted up to `maxRetries` times, with a delay of `retryDelay` in between, before an error is triggered.

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `readWriteChunkSize` | `uint8_t` | The number of bytes that will be read or written in a single I2C transmission. Default is 32 |
| `maxRetries` | `const uint8_t` | The maximum number of times a register read or write wil be attempted before triggering an error. Set to 6 |
| `retryDelay` | `const uint8_t` | The number of milliseconds delay between read or write attempts. Set to 5 |
