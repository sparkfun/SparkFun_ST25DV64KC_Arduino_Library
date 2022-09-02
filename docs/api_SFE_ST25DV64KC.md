# API Reference for the SFE_ST25DV64KC class

## Brief Overview

The ```SFE_ST25DV64KC``` class provides all of the necessary methods to exchange data with the ST25DV tag.

Communication with the tag is started by calling ```begin``` and providing the address of a ```TwoWire``` (I<sup>2</sup>C) Port. ```begin``` will default to ```Wire``` if no ```wirePort``` is provided.

The tag's unique identifier (UID) can be read with ```getDeviceUID```. The hardware version can be checked with ```getDeviceRevision```.

By default, the user memory can be both read and written to via both I<sup>2</sup>C and RF (NFC). But, to change any of the IC's settings, a security session needs to be opened
by entering the correct password. The default password is eight zeros ( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ) and - for I<sup>2</sup>C - can be entered by calling
```openI2CSession```. The status of the security session can be checked with ```isI2CSessionOpen```. The I<sup>2</sup>C password can be changed with ```writeI2CPassword```.

!!! attention
    The password can be read back from the tag with ```readRegisterValues```, _**but**_ only when a security session is open. If you change the password, close the security session and then forget the password, _**your tag is locked forever**_. There is no way to change or reset the pasword unless you know the password. If you change it, write it down somewhere.

The memory can be divided up into four areas, each of which can have different security levels applied. ```setMemoryAreaEndAddress``` and ```getMemoryAreaEndAddress```
can change and read the end address for each area.

With an open security session, the I<sup>2</sup>C read and write permissions for each area can be changed and read with ```programEEPROMReadProtectionBit```, ```programEEPROMWriteProtectionBit```,
```getEEPROMReadProtectionBit``` and ```getEEPROMWriteProtectionBit```.

With an open security session, the RF read and write permissions for each area can be changed and read with ```setAreaRfRwProtection``` and ```getAreaRfRwProtection```.
RF password control and be set and read with ```setAreaRfPwdCtrl``` and ```getAreaRfPwdCtrl```.

The EEPROM memory can be read and written with ```readEEPROM``` and ```writeEEPROM```.

The behaviour of the GPO pin can be set and read with ```setGPO1Bit```, ```getGPO1Bit```, ```setGPO2Bit``` and ```getGPO2Bit```.

Energy harvesting can be controlled with ```setEH_MODEBit``` and ```getEH_MODEBit```.

## Initialization / Configuration

### setErrorCallback()

This method sets the address of the error callback. Once set, a callback is triggered if an error occurs within a method.

```c++
void setErrorCallback(void (*errorCallback)(SF_ST25DV64KC_ERROR errorCode))
```

An example error handler - if ```tag``` is global - is:

```C++
void errorHandler(SF_ST25DV64KC_ERROR errorCode)
{
    Serial.print(F("Error Callback: "));
    Serial.println(tag.errorCodeString(errorCode));
}
```

```errorCodeString``` converts the ```SF_ST25DV64KC_ERROR``` enum into readable text.

The callback is set with:

```C++
tag.setErrorCallback(&errorHandler);
```

| Parameter | Description |
| :-------- | :---------- |
| `errorCallback` | The address of the callback |

### begin()

This method configures I<sup>2</sup>C communication with the tag and confirms the tag is connected.

```c++
bool begin(TwoWire &wirePort)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `wirePort` | `TwoWire &` | The address of the TwoWire port. Default is `Wire` |
| return value | `bool` | ```true``` if communication is begun successfully, otherwise ```false``` |

### isConnected()

This method confirms if a device is connected at the expected I<sup>2</sup>C address.
This method can only be called after ```begin```, as ```begin``` configures which TwoWire port will be used.
```begin``` calls ```isConnected``` internally to establish if a tag is connected.

```c++
bool isConnected()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if the tag is connected, otherwise ```false``` |

## Device Properties

### getDeviceUID()

This method reads the tag's 64-bit unique identifier as uint8_t[8].

```c++
bool getDeviceUID(uint8_t *values)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `values` | `uint8_t *` | A pointer to the array of uint8_t that will contain the UID. `values` must be uint8_t[8] |
| return value | `bool` | ```true``` if the read is successful, otherwise ```false``` |

### getDeviceRevision()

This method reads the tag's hardware revision.

```c++
bool getDeviceRevision(uint8_t *value)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `value` | `uint8_t *` | A pointer to uint8_t that will contain the revision |
| return value | `bool` | ```true``` if the read is successful, otherwise ```false``` |

## Security Session Password Control

### openI2CSession()

This method enters the I<sup>2</sup>C security session 64-bit password. password is uint8_t[8].
This method returns ```true``` if the password is written to the tag successfully - but that does not confirm that the password is valid/invalid.
The password validity must be confirmed with ```isI2CSessionOpen```.

```c++
bool openI2CSession(uint8_t *password)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `password` | `uint8_t *` | A pointer to the array of uint8_t that contains the password. password must be uint8_t[8] |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### isI2CSessionOpen()

This method checks if the I<sup>2</sup>C security session is open, i.e. that the correct password has been entered.

```c++
bool isI2CSessionOpen()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if the security session is open, otherwise ```false``` |

### writeI2CPassword()

This method changes the I<sup>2</sup>C password. It will only be successful when a security session is open, i.e. you need to know the current password to be able to change the password (obvs.).

!!! attention
    The password can be read back from the tag with ```readRegisterValues```, _**but**_ only when a security session is open. If you change the password, close the security session and then forget the password, _**your tag is locked forever**_. There is no way to change or reset the pasword unless you know the password. If you change it, write it down somewhere.

```c++
bool writeI2CPassword(uint8_t *password)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `password` | `uint8_t *` | A pointer to the array of uint8_t that contains the new password. password must be uint8_t[8] |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

## Memory Area Control

### setMemoryAreaEndAddress()

This method is used to modify the end address for memory Areas 1-3. The end address of Area 4 is always set to the tag's last memory location.

`endAddressValue` is an 8-bit value. The actual memory end address is: (32 * `endAddressValue`) + 31

```c++
bool setMemoryAreaEndAddress(uint8_t memoryNumber, uint8_t endAddressValue)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryNumber` | `uint8_t` | The memory area 1-3 |
| `endAddressValue` | `uint8_t` | The end address in 8-bit form. See notes above |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getMemoryAreaEndAddress()

This method returns the end address for the specified area.

If a read error occurs, an error callback is triggered.

The returned address is the actual end address in 16-bit format, not the 8-bit register value used by ```setMemoryAreaEndAddress```.

```c++
uint16_t getMemoryAreaEndAddress(uint8_t memoryArea)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-3 |
| return value | `uint16_t` | The actual end address (16-bit) |

## I<sup>2</sup>C Read and Write Protection

### programEEPROMReadProtectionBit()

This method sets/clears the I<sup>2</sup>C read protection bit for the specified memory area 1-4.

When set, the memory area is only readable if a security session is open.

!!! note
    Area 1 is _always_ readable. Calling ```programEEPROMReadProtectionBit(1, true)``` has no effect.

```c++
bool programEEPROMReadProtectionBit(uint8_t memoryArea, bool readSecured)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| `readSecured` | `bool` | ```true```: read is allowed only if an I<sup>2</sup>C security session is open. ```false```: read is always allowed |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### programEEPROMWriteProtectionBit()

This method sets/clears the I<sup>2</sup>C write protection bit for the specified memory area 1-4.

When set, the memory area is only writeable if a security session is open.

```c++
bool programEEPROMWriteProtectionBit(uint8_t memoryArea, bool writeSecured)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| `writeSecured` | `bool` | ```true```: write is allowed only if an I<sup>2</sup>C security session is open. ```false```: write is always allowed |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getEEPROMReadProtectionBit()

This method returns the state of read protection for the specified memory area 1-4..

```c++
bool getEEPROMReadProtectionBit(uint8_t memoryArea)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| return value | `bool` | ```true``` if memory read is protected, otherwise ```false``` |

### getEEPROMWriteProtectionBit()

This method returns the state of write protection for the specified memory area 1-4..

```c++
bool getEEPROMWriteProtectionBit(uint8_t memoryArea)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| return value | `bool` | ```true``` if memory write is protected, otherwise ```false``` |

## RF Read and Write Protection

### setAreaRfRwProtection()

This method sets the RF read and write protection for the specified memory area 1-4.

The default protection for all memory areas is `RF_RW_READ_ALWAYS_WRITE_ALWAYS`.

!!! note
    Area 1 is _always_ readable via RF. Calling ```setAreaRfRwProtection(1, RF_RW_READ_SECURITY_WRITE_SECURITY)``` or ```setAreaRfRwProtection(1, RF_RW_READ_SECURITY_WRITE_NEVER)``` has no effect.

```c++
bool setAreaRfRwProtection(uint8_t memoryArea, SF_ST25DV_RF_RW_PROTECTION rw)
```

The permissible values for `rw` are:

```C++
RF_RW_READ_ALWAYS_WRITE_ALWAYS,
RF_RW_READ_ALWAYS_WRITE_SECURITY,
RF_RW_READ_SECURITY_WRITE_SECURITY, // For Area 1: Read is always allowed
RF_RW_READ_SECURITY_WRITE_NEVER     // For Area 1: Read is always allowed
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| `rw` | `enum class SF_ST25DV_RF_RW_PROTECTION` | The level of read/write protection |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getAreaRfRwProtection()

This method returns the level of RF read and write protection for the specified memory area 1-4.

If a read error occurs, an error callback is triggered.

```c++
SF_ST25DV_RF_RW_PROTECTION getAreaRfRwProtection(uint8_t memoryArea)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| return value | `enum class SF_ST25DV_RF_RW_PROTECTION` | The level of read/write protection |

### setAreaRfPwdCtrl()

This method defines which password is required to open a security session for the specified memory area 1-4.

An "RF Configuration" security session can be opened by entering RF Password 0. This session allows the tag's configuration static registers
and RF Password 0 to be modified via RF. The default RF Password 0 is ( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ).

An "RF User" security session can be opened by entering RF Password 1-3. This session allows the user access to protected user memory
as defined by ```setAreaRfRwProtection```. The matching RF Password (1-3) can also be changed while the session is open.

By default, the `pwdCtrl` setting for each memory area is `RF_PWD_NEVER`, meaning that a RF User security session can not be opened by password.

!!! attention
    To protect a tag against unwanted changes via an RF Configuration Session, RF Password 0 should be changed using an App like ST's "NFC Tap". It is not possible to do this via I<sup>2</sup>C. The I<sup>2</sup>C interface has no access to the four RF passwords.

```c++
bool setAreaRfPwdCtrl(uint8_t memoryArea, SF_ST25DV_RF_PWD_CTRL pwdCtrl)
```

The permissible values for `pwdCtrl` are:

```C++
RF_PWD_NEVER,
RF_PWD_PWD1,
RF_PWD_PWD2,
RF_PWD_PWD3
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| `pwdCtrl` | `enum class SF_ST25DV_RF_PWD_CTRL` | The selected password option |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getAreaRfPwdCtrl()

This method returns the selected password option for the specified memory area 1-4.

If a read error occurs, an error callback is triggered.

```c++
SF_ST25DV_RF_PWD_CTRL getAreaRfPwdCtrl(uint8_t memoryArea)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `memoryArea` | `uint8_t` | The memory area 1-4 |
| return value | `enum class SF_ST25DV_RF_PWD_CTRL` | The selected password option |

## EEPROM Read and Write

### readEEPROM()

This method reads `dataLength` bytes from EEPROM memory, starting at `baseAddress`. The bytes are returned in `data`.

!!! note
    `data` must be sufficiently large to hold all `dataLength` bytes.

```c++
bool readEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `baseAddress` | `uint16_t` | The base (start) EEPROM address for the read |
| `data` | `uint8_t *` | A pointer to an array of uint8_t to hold the read data |
| `dataLength` | `uint16_t` | The number of bytes to be read |
| return value | `bool` | ```true``` if the read is successful, otherwise ```false``` |

### writeEEPROM()

This method writes `dataLength` bytes to EEPROM memory, starting at `baseAddress`. The bytes to be written are held in `data`.

```c++
bool writeEEPROM(uint16_t baseAddress, uint8_t *data, uint16_t dataLength)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `baseAddress` | `uint16_t` | The base (start) EEPROM address for the write |
| `data` | `uint8_t *` | A pointer to an array of uint8_t which holds the data to be written |
| `dataLength` | `uint16_t` | The number of bytes to be written |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

## RF Detection

### RFFieldDetected()

This method detects if an RF field is present.

```C++
bool RFFieldDetected()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if an RF field is detected, otherwise ```false``` |

## GPO Control

### setGPO1Bit

This method sets or clears the selected bit(s) in the GPO1 register.

Multiple bits can be set or cleared with a single call.

```c++
bool setGPO1Bit(uint8_t bitMask, bool enabled)
```

The GPO1 bit definitions are:

```C++
#define BIT_GPO1_GPO_EN (1 << 0)
#define BIT_GPO1_RF_USER_EN (1 << 1)
#define BIT_GPO1_RF_ACTIVITY_EN (1 << 2)
#define BIT_GPO1_RF_INTERRUPT_EN (1 << 3)
#define BIT_GPO1_FIELD_CHANGE_EN (1 << 4)
#define BIT_GPO1_RF_PUT_MSG_EN (1 << 5)
#define BIT_GPO1_RF_GET_MSG_EN (1 << 6)
#define BIT_GPO1_RF_WRITE_EN (1 << 7)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit(s) to be set or cleared |
| `enabled` | `bool` | If ```true```, the bit(s) set in `bitMask` are set. If ```false```, the bit(s) set in `bitMask` are cleared.
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getGPO1Bit()

This method returns the state of the selected bit in the GPO1 register.

```c++
bool getGPO1Bit(uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit to be read |
| return value | `bool` | ```true``` if the bit is set, otherwise ```false``` |

### setGPO2Bit

This method sets or clears the selected bit(s) in the GPO2 register.

Multiple bits can be set or cleared with a single call.

```c++
bool setGPO2Bit(uint8_t bitMask, bool enabled)
```

The GPO2 bit defintions are:

```C++
#define BIT_GPO2_I2C_WRITE_EN (1 << 0)
#define BIT_GPO2_I2C_RF_OFF_EN (1 << 1)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit(s) to be set or cleared |
| `enabled` | `bool` | If ```true```, the bit(s) set in `bitMask` are set. If ```false```, the bit(s) set in `bitMask` are cleared.
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getGPO2Bit()

This method returns the state of the selected bit in the GPO2 register.

```c++
bool getGPO2Bit(uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit to be read |
| return value | `bool` | ```true``` if the bit is set, otherwise ```false``` |

### setGPO_CTRL_DynBit()

This method sets or clears the GPO_EN bit in the GPO_CTRL_Dyn (Dynamic) register.
This allows the GPO pin to be enabled or disabled without making non-volatile changes to the GPO1 register GPO_EN bit.

```c++
bool setGPO_CTRL_DynBit(bool enabled)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `enabled` | `bool` | If ```true```, the GPO_EN bit is set, otherwise it is cleared |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getGPO_CTRL_DynBit()

This method retruns the state of the GPO_EN bit in the GPO_CTRL_Dyn (Dynamic) register.

```c++
bool getGPO_CTRL_DynBit()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if the GPO_EN bit is set, otherwise ```false``` |

## Interrupt Status

### getIT_STS_Dyn()

This method returns the content of the dynamic Interrupt Status register IT_SYS_Dyn, indicating the cause of an interrupt.

If a read error occurs, an error callback is triggered.

!!! note
    Once read, the ITSTS_Dyn register is cleared (set to 00h).

```c++
uint8_t getIT_STS_Dyn();
```

The bit definitions of the IT_SYS_Dyn register are:

```C++
#define BIT_IT_STS_DYN_RF_USER (1 << 0)
#define BIT_IT_STS_DYN_RF_ACTIVITY (1 << 1)
#define BIT_IT_STS_DYN_RF_INTERRUPT (1 << 2)
#define BIT_IT_STS_DYN_FIELD_FALLING (1 << 3)
#define BIT_IT_STS_DYN_FIELD_RISING (1 << 4)
#define BIT_IT_STS_DYN_RF_PUT_MSG (1 << 5)
#define BIT_IT_STS_DYN_RF_GET_MSG (1 << 6)
#define BIT_IT_STS_DYN_RF_WRITE (1 << 7)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `uint8_t` | The register value |

## Energy Harvesting

### setEH_MODEBit()

This method sets or clears the Energy Harvesting EH_MODE bit in the EH_MODE register.

```c++
bool setEH_MODEBit(bool value)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `value` | `bool` | If ```true```, Energy Harvesting is on demand only (default). If ```false```, EH  is forced after boot |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getEH_MODEBit()

This method returns the state of the Energy Harvesting EH_MODE bit in the EH_MODE register.

```c++
bool getEH_MODEBit()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `bool` | ```true``` if the EH_MODE bit is set, otherwise ```false``` |

### setEH_CTRL_DYNBit()

This method will set or clear bit(s) in the dynamic EH_CTRL_Dyn register.
This allows energy harvesting to be enabled or disabled without making non-volatile changes to the EH_MODE register.

```c++
bool setEH_CTRL_DYNBit(uint8_t bitMask, bool value)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit(s) to be set or cleared |
| `value` | `bool` | If ```true```, the bit(s) set in `bitMask` are set. If ```false```, the bit(s) set in `bitMask` are cleared |
| return value | `bool` | ```true``` if the write is successful, otherwise ```false``` |

### getEH_CTRL_DYNBit()

This method returns the state of the select bit in the dynamic EH_CTRL_Dyn register.

!!! note
    If multiple bits are set in `bitMask`, the method will return ```true``` if _any one of those bits_ is set.

```c++
bool getEH_CTRL_DYNBit(uint8_t bitMask)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `bitMask` | `uint8_t` | The bit to be read |
| return value | `bool` | ```true``` if the bit is set, otherwise ```false``` |

## Helper Methods

### errorCodeString()

This method converts an `SF_ST25DV64KC_ERROR` error code into readable text.

```C++
const char *errorCodeString(SF_ST25DV64KC_ERROR errorCode)
```

| Parameter | Description |
| :-------- | :---------- |
| `errorCode` | The ```enum class SF_ST25DV64KC_ERROR``` error code |
| return value | `const char *` | A pointer to the readable text |

### readRegisterValue()

This method reads a single register value.

```c++
bool readRegisterValue(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *value)
```

This is the equivalent of calling:

```c++
tag->st25_io.readSingleByte(addressType, registerAddress, value)
```

Possible values for `addressType` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `addressType` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I<sup>2</sup>C address |
| `registerAddress` | `const uint16_t` | The register address |
| `value` | `uint8_t *` | `value` will hold the register value on return |
| return value | `bool` | ```true``` if the read was successful, otherwise ```false``` |

### readRegisterValues()

This method reads multiple register values.

!!! note
    `data` must be sufficiently large to hold all `dataLength` bytes.

```c++
bool readRegisterValues(const SF_ST25DV64KC_ADDRESS addressType, const uint16_t registerAddress, uint8_t *data, const uint16_t dataLength)
```

This is the equivalent of calling:

```C++
tag->st25_io.readMultipleBytes(addressType, registerAddress, data, dataLength)
```

Possible values for `addressType` are:

```C++
DATA,          // E2 = 0, E1 = 1
SYSTEM,        // E2 = 1, E1 = 1
RF_SWITCH_OFF, // E2 = 0, E1 = 0
RF_SWITCH_ON,  // E2 = 1, E1 = 0
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `addressType` | `enum class SF_ST25DV64KC_ADDRESS` | The register type, equivalent to the I<sup>2</sup>C address |
| `registerAddress` | `const uint16_t` | The start register address |
| `data` | `uint8_t *` | `data` will hold the register values on return |
| `dataLength` | `const uint16_t` | The number of registers to be read |
| return value | `bool` | ```true``` if the read was successful, otherwise ```false``` |

## Member Variables

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `st25_io` | `class SFE_ST2525DV64KC_IO` | An instance of the `SFE_ST2525DV64KC_IO` class, providing I<sup>2</sup>C communication methods and storage for `wirePort` |
