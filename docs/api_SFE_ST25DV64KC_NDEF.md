# API Reference for the SFE_ST25DV64KC_NDEF class

## Brief Overview

The ```SFE_ST25DV64KC_NDEF``` class provides additional methods to read and write NDEF (NFC Forum Data Exchange Format) records from EEPROM memory.
The ```SFE_ST25DV64KC_NDEF``` class inherits all of the methods of the ```SFE_ST25DV64KC``` class.

## Capability Container Methods

### writeCCFile4Byte()

This method will write a 4-byte Capability Container file to the first 4 bytes of EEPROM memory.

!!! note
    This method is only useful for small tags like the ST245DV04KC. The ST25DV64KC requires an 8-byte CC file.

```c++
bool writeCCFile4Byte(uint32_t val)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `val` | `uint32_t` | The 32-bit (4-byte) capability container. Default is 0xE1403F00 |
| return value | `bool` | ```true``` is the write was successful, otherwise ```false``` |

### writeCCFile8Byte()

This method will write a 8-byte Capability Container file to the first 8 bytes of EEPROM memory.

```c++
bool writeCCFile8Byte(uint32_t val1, uint32_t val2)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `val1` | `uint32_t` | The first 32-bits (4-bytes) of the capability container. Default is 0xE2400001 |
| `val2` | `uint32_t` | The second 32-bits (4-bytes) of the capability container. Default is 0x000003FF |
| return value | `bool` | ```true``` is the write was successful, otherwise ```false``` |

### setCCFileLen()

This method allows the ```_ccFileLen``` member variable to be manually updated if required.

```_ccFileLen``` has a default value of 8 (for the ST25DV64KC).

```_ccFileLen``` is automatically set to 4 when ```writeCCFile4Byte``` is called.
```_ccFileLen``` is automatically set to 8 when ```writeCCFile8Byte``` is called.

```C++
void setCCFileLen(uint16_t newLen)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `newLen` | `uint16_t` | The new value for ```_ccFileLen``` |

### getCCFileLen()

This method returns the current value of ```_ccFileLen```. It returns the correct start address to be passed to the ```writeNDEF``` functions.

```C++
uint16_t getCCFileLen()
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `uint16_t` | The current value of ```_ccFileLen``` |

## NDEF Methods

## writeNDEFURI()

This method writes an NDEF URI record to EEPROM.

The URI is passed in `uri`.

Setting `idCode` to the appropriate ID Code will save memory. The URI prefix (e.g. ```https://www.```)is then stored as a single byte.

Multiple records can be written by: providing an `address` plus Message Begin (`MB`) and Message End (`ME`) flags.

If `address` is provided, it is updated with the memory location after the final one written as part of this record. This allows it to be used as the start
address for the next record.

A single record has both `MB` and `ME` flags set ```true```.

The TLV T and L fields are set automatically. The L field is updated from 1-byte to 3-byte automatically if the new record takes the total length beyond 254 bytes.

```C++
bool writeNDEFURI(const char *uri, uint8_t idCode, uint16_t *address, bool MB, bool ME)
```

Possible values for `idCode` are:

```C++
#define SFE_ST25DV_NDEF_URI_ID_CODE_NONE 0x00
#define SFE_ST25DV_NDEF_URI_ID_CODE_HTTP_WWW 0x01
#define SFE_ST25DV_NDEF_URI_ID_CODE_HTTPS_WWW 0x02
#define SFE_ST25DV_NDEF_URI_ID_CODE_HTTP 0x03
#define SFE_ST25DV_NDEF_URI_ID_CODE_HTTPS 0x04
#define SFE_ST25DV_NDEF_URI_ID_CODE_TEL 0x05
#define SFE_ST25DV_NDEF_URI_ID_CODE_MAILTO 0x06
#define SFE_ST25DV_NDEF_URI_ID_CODE_FTP_ANON_ANON 0x07
#define SFE_ST25DV_NDEF_URI_ID_CODE_FTP_FTP 0x08
#define SFE_ST25DV_NDEF_URI_ID_CODE_FTPS 0x09
#define SFE_ST25DV_NDEF_URI_ID_CODE_SFTP 0x0A
#define SFE_ST25DV_NDEF_URI_ID_CODE_SMB 0x0B
#define SFE_ST25DV_NDEF_URI_ID_CODE_NFS 0x0C
#define SFE_ST25DV_NDEF_URI_ID_CODE_FTP 0x0D
#define SFE_ST25DV_NDEF_URI_ID_CODE_DAV 0x0E
#define SFE_ST25DV_NDEF_URI_ID_CODE_NEWS 0x0F
#define SFE_ST25DV_NDEF_URI_ID_CODE_TELNET 0x10
#define SFE_ST25DV_NDEF_URI_ID_CODE_IMAP 0x11
#define SFE_ST25DV_NDEF_URI_ID_CODE_RTSP 0x12
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN 0x13
#define SFE_ST25DV_NDEF_URI_ID_CODE_POP 0x14
#define SFE_ST25DV_NDEF_URI_ID_CODE_SIP 0x15
#define SFE_ST25DV_NDEF_URI_ID_CODE_SIPS 0x16
#define SFE_ST25DV_NDEF_URI_ID_CODE_TFTP 0x17
#define SFE_ST25DV_NDEF_URI_ID_CODE_BTSPP 0x18
#define SFE_ST25DV_NDEF_URI_ID_CODE_BTL2CAP 0x19
#define SFE_ST25DV_NDEF_URI_ID_CODE_BTGOEP 0x1A
#define SFE_ST25DV_NDEF_URI_ID_CODE_TCPOBEX 0x1B
#define SFE_ST25DV_NDEF_URI_ID_CODE_IRDAOBEX 0x1C
#define SFE_ST25DV_NDEF_URI_ID_CODE_FILE 0x1D
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_EPC_ID 0x1E
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_EPC_TAG 0x1F
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_EPC_PAT 0x20
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_EPC_RAW 0x21
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_EPC 0x22
#define SFE_ST25DV_NDEF_URI_ID_CODE_URN_NFC 0x23
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `uri` | `const char *` | A pointer to the URI |
| `idCode` | `uint8_t` | The URI prefix. ```SFE_ST25DV_NDEF_URI_ID_CODE_HTTPS_WWW``` = ```https://www.``` etc.. Default is ```SFE_ST25DV_NDEF_URI_ID_CODE_NONE``` |
| `address` | `uint16_t *` | A pointer to a `uint16_t` containing the start address for this record. Default is ```NULL```. If not provided, the record is written to ```_ccFileLen``` |
| `MB` | `bool` | Message Begin flag. Default is ```true``` |
| `ME` | `bool` | Message End flag. Default is ```true``` |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### readNDEFURI()

This method reads and returns a single NDEF URI from EEPROM.

By default, the first record is returned. The second and subsequent records can be read by setting `recordNo` to the desired value.

If the chosen `recordNo` is not present, the method returns ```false```. This provides an easy way to count the number of URIs in memory.

```C++
bool readNDEFURI(char *theURI, uint16_t maxURILen, uint8_t recordNo)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `theURI` | `char *` | A pointer to the char array which will hold the URI |
| `maxURILen` | `uint16_t` | The maximum number of bytes which `uri` can hold. I.e. the ```sizeof``` of `uri`  |
| `recordNo` | `uint8_t` | The URI record to be read. Default is 1 |
| return value | `bool` | ```true``` if the read is successful (i.e. `recordNo` exists), ```false``` otherwise |

### writeNDEFWiFi()

This method writes an NDEF WiFi record to EEPROM.

The WiFi SSID and Password are passed in `ssid` and `passwd`.

Multiple records can be written by: providing an `address` plus Message Begin (`MB`) and Message End (`ME`) flags.

If `address` is provided, it is updated with the memory location after the final one written as part of this record. This allows it to be used as the start
address for the next record.

A single record has both `MB` and `ME` flags set ```true```.

The TLV T and L fields are set automatically. The L field is updated from 1-byte to 3-byte automatically if the new record takes the total length beyond 254 bytes.

`auth` (the WiFi authentication) defaults to `SFE_ST25DV_WIFI_AUTH_WPA2_PERSONAL`, as specified by the Wi-Fi Simple Configuration Technical Specification.

`encrypt` (the WiFi encryption) defaults to `SFE_ST25DV_WIFI_ENCRYPT_AES`, as specified by the Wi-Fi Simple Configuration Technical Specification.

```C++
bool writeNDEFWiFi(const char *ssid, const char *passwd, uint16_t *address, bool MB, bool ME, const uint8_t auth[2], const uint8_t encrypt[2])
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `ssid` | `const char *` | A pointer to the SSID (Network Name) |
| `passwd` | `const char *` | A pointer to the password (Network Key)  |
| `address` | `uint16_t *` | A pointer to a `uint16_t` containing the start address for this record. Default is ```NULL```. If not provided, the record is written to ```_ccFileLen``` |
| `MB` | `bool` | Message Begin flag. Default is ```true``` |
| `ME` | `bool` | Message End flag. Default is ```true``` |
| `auth` | `const uint8_t[2]` | The WiFi authentication method. Default is `SFE_ST25DV_WIFI_AUTH_WPA2_PERSONAL` = {0x00, 0x20} |
| `encrypt` | `const uint8_t[2]` | The WiFi encryption method. Default is `SFE_ST25DV_WIFI_ENCRYPT_AES` = {0x00, 0x08} |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### readNDEFWiFi()

This method reads and returns a single NDEF WiFi record from EEPROM.

By default, the first record is returned. The second and subsequent records can be read by setting `recordNo` to the desired value.

If the chosen `recordNo` is not present, the method returns ```false```. This provides an easy way to count the number of WiFi records in memory.

```C++
bool readNDEFWiFi(char *ssid, uint16_t maxSsidLen, char *passwd, uint16_t maxPasswdLen, uint8_t recordNo)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `ssid` | `char *` | A pointer to the char array which will hold the SSID |
| `maxSsidLen` | `uint16_t` | The maximum number of bytes which `ssid` can hold. I.e. the ```sizeof``` of `ssid`  |
| `passwd` | `char *` | A pointer to the char array which will hold the password |
| `maxPasswdLen` | `uint16_t` | The maximum number of bytes which `passwd` can hold. I.e. the ```sizeof``` of `passwd`  |
| `recordNo` | `uint8_t` | The URI record to be read. Default is 1 |
| return value | `bool` | ```true``` if the read is successful (i.e. `recordNo` exists), ```false``` otherwise |

### writeNDEFText()

This method writes an NDEF Text record to EEPROM.

The text passed in ```const char *theText```.

The length of `theText` is calculated using ```strlen```, which will fail if `theText` contains any zero (0x00) bytes.
An overloaded method allows the text to be defined using ```const uint8_t *theText``` and ```uint16_t textLength``` if required.

The text language code can be defined using the `languageCode` parameter. `languageCode` defaults to ```NULL```, in which case the language code is set automatically to "en" (English).

Multiple records can be written by: providing an `address` plus Message Begin (`MB`) and Message End (`ME`) flags.

If `address` is provided, it is updated with the memory location after the final one written as part of this record. This allows it to be used as the start
address for the next record.

A single record has both `MB` and `ME` flags set ```true```.

The TLV T and L fields are set automatically. The L field is updated from 1-byte to 3-byte automatically if the new record takes the total length beyond 254 bytes.

```C++
  bool writeNDEFText(const char *theText, uint16_t *address, bool MB, bool ME, const char *languageCode)
  bool writeNDEFText(const uint8_t *theText, uint16_t textLength, uint16_t *address, bool MB, bool ME, const char *languageCode)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `theText` | `const char *` | A pointer to the text |
| `address` | `uint16_t *` | A pointer to a `uint16_t` containing the start address for this record. Default is ```NULL```. If not provided, the record is written to ```_ccFileLen``` |
| `MB` | `bool` | Message Begin flag. Default is ```true``` |
| `ME` | `bool` | Message End flag. Default is ```true``` |
| `languageCode` | `const char *` | A pointer to the text language code. Default is ```NULL``` |
| return value | `bool` | ```true``` if the write was successful, otherwise ```false``` |

### readNDEFText()

This method reads and returns a single NDEF Text record from EEPROM.

By default, the first record is returned. The second and subsequent records can be read by setting `recordNo` to the desired value.

If the chosen `recordNo` is not present, the method returns ```false```. This provides an easy way to count the number of WiFi records in memory.

```C++
bool readNDEFText(char *theText, uint16_t maxTextLen, uint8_t recordNo, char *language = NULL, uint16_t maxLanguageLen)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `theText` | `char *` | A pointer to the char array which will hold the text |
| `maxTextLen` | `uint16_t` | The maximum number of bytes which `theText` can hold. I.e. the ```sizeof``` of `theText`  |
| `recordNo` | `uint8_t` | The URI record to be read. Default is 1 |
| `language` | `char *` | A pointer to the char array which could hold the language code. Default is ```NULL``` |
| `maxLanguageLen` | `uint16_t` | The maximum number of bytes which `language` can hold. I.e. the ```sizeof``` of `language`. Default is zero |
| return value | `bool` | ```true``` if the read is successful (i.e. `recordNo` exists), ```false``` otherwise |

## Helper Methods

### getURIPrefix()

This method returns the URI prefix code at readable text. It is used by ```readNDEFURI``` to reconstruct the full URI based on the stored `idCode`.

```C++
const char *getURIPrefix(uint8_t prefixCode)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `prefixCode` | `uint8_t` | The prefix code |
| return value | `const char *` | A pointer to the prefix as readable text |

## Member Variables

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `_ccFileLen` | `uint16_t` | The length of the capability container. I.e. the start address for NDEF records |
