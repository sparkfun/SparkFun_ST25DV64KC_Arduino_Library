# Introduction

The ST25DV dynamic NFC RFID tag ICs are awesome! The ST25DV64KC offers 64-kBit (8-KBytes) of EEPROM memory which can be accessed over both I2C and RF (NFC)! It's a state-of-the-art
tag which conforms to ISO/IEC 15693 or NFC Forum Type 5 recommendations.

The memory can be divided into four areas, each of which can have different levels of protection applied. Want to store your data so everyone has read and write access over both I2C and RF? You can do that.
In fact, that's the default! Want to store your data in an area which can only be read if you know the password, and cannot be written to (via RF)? You can do that too!

This library provides all of the methods you need to read and write the user memory, control the read and write permissions, alter the area sizes and apply password control.

Not only that! We've included extra methods which will let you read and write NDEF (NFC Forum Data Exchange Format) URI, WiFi and Text records which your smart phone can understand!

Do you want your project to be able to log up to 8-kBytes of data (via I2C) and then be able to read it using your smart phone - even if your project is powered off? This library lets you do that!

Do you want to be able to write WiFi credentials (SSID and Password) into tag memory using your smart phone and then to be able to read them over I2C? With this library you can do that too!

Do you want to be able to manufacture and test your product and then program it with keys or other credentials just before you ship? We've got you covered.

Are you developing equipment or appliances from the home? Would you like to store diagnostic information so that the repair engineer can access it using their smart phone? This library lets you do that!

Would you like the customer to be able to read the diagnostics with _their_ smart phone and then email them to you? This library will let you do that too! (Apart from the email bit - obvs.. You will need to add that bit yourself.)

The [examples](https://sparkfun.github.io/SparkFun_ST25DV64KC_Arduino_Library/sparkfun-st25dv64kc-arduino-library-examples/ex_01_Device_Information/) in this library have been tested on multiple platforms.
We enjoyed writing and testing them. We hope you enjoy them too and that they make your life easier.
