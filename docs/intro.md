# Introduction

The SparkFun Qwiic Universal Auto-Detect library makes it easy to create a scalable Qwiic (I<sup>2</sup>C) sensing and logging device on a wide range of platforms.

We are very proud of [OpenLog Artemis](https://www.sparkfun.com/products/19426). The aim of OLA is simple: provide a system which
can automatically detect a wide range of Qwiic sensors and log data from them, all without writing a single line of code. It has been _very_ successful.
However, adding new sensors to OLA is quite complicated. There are [six files to change](https://github.com/sparkfun/OpenLog_Artemis/blob/main/ADDING_SENSORS.md)
and new code needs to be added in multiple places. Also, OLA is, by its nature, tied to the Artemis (Apollo3) platform.

With this library we wanted to make a fresh start. The intention is to provide a universal library which can automatically detect, configure and log data from an
even wider range of Qwiic sensors and to let it run on multiple platforms. Want to make a logging system which runs on your favourite hardware platform?
With this library you can absolutely do that!

This library is stand-alone. It contains a built-in copy of the Arduino Library for each sensor. You do not need to worry about installing those libraries yourself.
Each time we update the library, we have a script which will automatically include the latest version of the Arduino Library for each sensor. And each time we add
a new sensor, the latest Arduino Library for that is included automatically too. All you need to do is keep this single library up to date, via the Arduino Library
Manager or by forking or cloning it on GitHub, and everything else happens automatically.

The built-in menus make it easy to define and store thing like: the logging interval; a WiFi SSID and password; NTP server names and time zone configuration; ThingSpeak API Key and Channel ID; etc. etc..

If your platform has (e.g.) WiFi or BLE connectivity, this library makes it easy to access the sensor data over those connections. However, you won't find
any WiFi or BLE code in this library. Those are layers you can add yourself depending on which hardware platform you are using. As time goes on, we will expand the provided
[_examples_](https://sparkfun.github.io/SparkFun_Qwiic_Universal_Auto-Detect/sparkfun-qwiic-universal-auto-detect-examples/ex_01_Thing_Plus_C/) showing how you can (e.g.) access the sensor data via an ESP32 WiFi web page. But we won't ever build that functionality directly into the library itself.
It then becomes hardware-dependent and that goes against what this library sets out to achieve: hardware-independence!

This library contains everything you need to log data from all supported sensors. You do not _need_ a WiFi connection and it does not need to connect to an IO server
or dashboard before you can access your sensor data. No account required!

When it comes to storing the sensor configuration and menu settings, we've made that as hardware-independent as possible too. You will find support and examples
showing how you can store the configuration on microSD card, LittleFS or in EEPROM memory.

The [examples](https://sparkfun.github.io/SparkFun_Qwiic_Universal_Auto-Detect/sparkfun-qwiic-universal-auto-detect-examples/ex_01_Thing_Plus_C/) in this library were written for the [SparkFun Thing Plus C - ESP32 WROOM (SPX-18018)](https://www.sparkfun.com/products/18018)
but can be adapted for just about any platform.

We've even tested it on the good old ATmega328P (as used on the original Uno and RedBoards). The 328P does not have enough memory to hold all of the sensor libraries,
but you can enable a small sub-set of sensors (by editing [src/SFE_QUAD_Sensors.h](https://github.com/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect/blob/main/src/SFE_QUAD_Sensors.h)) and still make use of the bulit-in menus!
