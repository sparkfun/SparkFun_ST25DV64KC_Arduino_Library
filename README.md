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

## Documentation

A full library use overview, API reference guide and key example walk-throughs are available on GitHub Pages - [sparkfun.github.io/SparkFun_ST25DV64KC_Arduino_Library](https://sparkfun.github.io/SparkFun_ST25DV64KC_Arduino_Library/)
