# SparkFun ST25DV64KC Arduino Library

<p align="center">
    <a href="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/issues" alt="Issues">
        <img src="https://img.shields.io/github/issues/sparkfun/SparkFun_ST25DV64KC_Arduino_Library.svg" /></a>
    <a href="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/actions" alt="Actions">
        <img src="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/actions/workflows/mkdocs.yml/badge.svg" /></a>
    <a href="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/actions" alt="Actions">
        <img src="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/actions/workflows/compile-sketch.yml/badge.svg" /></a>
    <a href="https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/blob/master/LICENSE" alt="License">
        <img src="https://img.shields.io/badge/license-MIT-blue.svg" /></a>
    <a href="https://twitter.com/intent/follow?screen_name=sparkfun">
        <img src="https://img.shields.io/twitter/follow/sparkfun.svg?style=social&logo=twitter"
        alt="follow on Twitter"></a>
</p>

<table class="table table-hover table-striped table-bordered">
    <tr align="center">
     <td><a href="https://www.sparkfun.com/products/19035"><img src="https://cdn.sparkfun.com/assets/parts/1/8/6/3/6/19035-Qwiic_RFID_Tag-01.jpg"></a></td>
    </tr>
    <tr align="center">
        <td><a href="https://www.sparkfun.com/products/19035">SparkFun Qwiic Dynamic RFID Tag - ST25DV64KC (SPX-19035)</a></td>
    </tr>
</table>

An Arduino library for the ST25DV64KC Dynamic NFC/RFID Tag.

With this library, you can:

* Write WiFi credentials to the tag using your smart phone and then have your Arduino board read them via I2C
* Log data from your Arduino project into the tag's EEPROM memory and then read it back with your smart phone - even while your Arduino board is asleep or powered off!
* Configure or update your project's keys, usernames or passwords securely and easily using contactless near field communication

## Key Features

* Provides a comprehensive set of methods to:
    * Read and write the tag's user memory
    * Adjust the size of the four memory areas
    * Change the I2C and RF passwords and read/write protection
    * Enable RF detection and Energy Harvesting
* Includes enhanced methods to:
    * Write the tag's Capability Container (CC) File
    * Read and write multiple NDEF URI records
    * Read and write multiple NDEF WiFi records
    * Read and write multiple NDEF Text records

## Getting Started

The [Getting Started Page](intro.md) outlines library installation and the general use of the SparkFun ST25DV64KC Arduino Library. 

Detailed [`examples`](https://github.com/sparkfun/SparkFun_ST25DV64KC_Arduino_Library/blob/main/examples) are included as part of the library installation process and available in the Arduino IDE `File > Examples >` menu. A walk-thru of key examples is contained in the [Examples](https://sparkfun.github.io/SparkFun_ST25DV64KC_Arduino_Library/sparkfun-st25dv64kc-arduino-library-examples/ex_01_Device_Information/) section of this documentation set. 

A full [API Reference](api_SFE_ST25DV64KC.md) is also provided for the library.

## Tested Platforms

- [ATmega328P](https://www.sparkfun.com/products/18158)
- [ESP32](https://www.sparkfun.com/products/15663)
- [Artemis (Apollo3)](https://www.sparkfun.com/products/15574)
- [SAMD21](https://www.sparkfun.com/products/14812)
- [SAMD51](https://www.sparkfun.com/products/14713)
- [STM32](https://www.sparkfun.com/products/17712)
- [RP2040](https://www.sparkfun.com/products/17745) following the instructions in the [Hookup Guide](https://learn.sparkfun.com/tutorials/rp2040-thing-plus-hookup-guide/software-overview) and using the pin definitions shown below:

```c++
// pins_arduino.h : pin definitions for the RP2040 Thing Plus (DEV-17745)
// SPI
#define PIN_SPI_MISO  (12u)
#define PIN_SPI_MOSI  (15u)
#define PIN_SPI_SCK   (14u)
#define PIN_SPI_SS    (9u)
// Wire
#define PIN_WIRE_SDA  (6u)
#define PIN_WIRE_SCL  (7u)
```

## License

The SparkFun ST25DV64KC Arduino Library is licensed using the Open Source MIT License