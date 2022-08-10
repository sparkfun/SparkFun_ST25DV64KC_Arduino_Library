# Software Setup

## Installation

The SparkFun Qwiic Universal Auto-Detect library is available within in the Arduino library manager, which is launched via the `Sketch > Include Libraries > Manage Libraries …` menu option in the Arduino IDE. Just search for ***SparkFun Qwiic Universal***

!!! note
    This guide assumes you are using the latest version of the Arduino IDE on your desktop. The following resources available at [SparkFun](https://www.sparkfun.com) provide the details on setting up and configuring Arduino to use this library.

    - [Installing the Arduino IDE](https://learn.sparkfun.com/tutorials/installing-arduino-ide)
    - [Installing an Arduino Library](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)
    - [Installing Board Definitions in the Arduino IDE](https://learn.sparkfun.com/tutorials/installing-board-definitions-in-the-arduino-ide)

## General Use Pattern

The SparkFun Qwiic Universal Auto-Detect library includes a comprehensive set of examples which show, step by step, how to use the library and create the correct `SFE_QUAD_Sensors` object for your application.
The examples are available from the Examples menu of the Arduino IDE, and in the [`examples`](https://github.com/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect/blob/main/examples) folder of this repository. 

For a detailed description of the examples, please see the Examples section of the documentation. We recommend starting with the documentation for Example1 as that demonstrates
how to create and use a `SFE_QUAD_Sensors__SD` object in full detail. The later examples build upon Example1 and show how to use: `SFE_QUAD_Sensors__SdFat`, `SFE_QUAD_Sensors__LittleFS`, and `SFE_QUAD_Sensors__EEPROM`.
