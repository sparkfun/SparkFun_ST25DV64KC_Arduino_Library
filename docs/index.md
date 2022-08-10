# SparkFun Qwiic Universal Auto-Detect

<p align="center">
		<a href="https://github.com/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect/issues" alt="Issues">
				<img src="https://img.shields.io/github/issues/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect.svg" /></a>
		<a href="https://github.com/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect/blob/master/LICENSE" alt="License">
				<img src="https://img.shields.io/badge/license-MIT-blue.svg" /></a>
		<a href="https://twitter.com/intent/follow?screen_name=sparkfun">
						<img src="https://img.shields.io/twitter/follow/sparkfun.svg?style=social&logo=twitter"
								alt="follow on Twitter"></a>
</p>

<table class="table table-hover table-striped table-bordered">
	<tr align="center">
	 <td><a href="https://www.sparkfun.com/products/18018"><img src="https://cdn.sparkfun.com//assets/parts/1/7/2/3/9/18018-Thing_Plus_C_-_ESP32_WROOM-01.jpg"></a></td>
	</tr>
	<tr align="center">
		<td><a href="https://www.sparkfun.com/products/18018">SparkFun ESP32 Thing Plus C (SPX-18018)</a></td>
	</tr>
</table>

The SparkFun Qwiic Universal Auto-Detect library makes it easy to create a scalable Qwiic (I<sup>2</sup>C) sensing and logging device on a wide range of platforms.

## Key Features

- Automatic detection of a wide range of Qwiic sensors
	- Built-in [Qwiic Mux](https://www.sparkfun.com/products/16784) support
- Only one library to install
	- This library contains automatically-updated copies of all the necessary sensor libraries
- Adding new sensors is easy
	- There is only _**one**_ file to add and _**three**_ files to modify when adding a new sensor
- Built-in menu system
- The sensor and menu configuration can be stored in:
	- EEPROM
	- LittleFS
	- SD or microSD card: both SD and SdFat are supported

## Getting Started

The [Getting Started Page](intro.md) outlines library installation and the general use of the SparkFun Qwiic Universal Auto-Detect library. 

Detailed [`examples`](https://github.com/sparkfun/SparkFun_Qwiic_Universal_Auto-Detect/blob/main/examples) are included as part of the library installation process and available in the Arduino IDE `File > Examples >` menu. A walk-thru of key examples is contained in the [Examples](https://sparkfun.github.io/SparkFun_Qwiic_Universal_Auto-Detect/sparkfun-qwiic-universal-auto-detect-examples/ex_01_Thing_Plus_C/) section of this documentation set. 

A full [API Reference](api_SFE_QUAD_Sensors.md) is also provided for the library.

## Supported Sensors

- Pressure, Altitude, Humidity and Temperature Data:
	- [AHT20](https://www.sparkfun.com/products/16618) humidity and temperature sensor
	- [BME280](https://www.sparkfun.com/products/15440) atmospheric sensor
	- [LPS25HB](https://www.sparkfun.com/products/14767) absolute pressure sensor
	- [MPR0025PA](https://www.sparkfun.com/products/16476) MicroPressure sensor
	- [MS5637](https://www.sparkfun.com/products/14688) barometric pressure sensor
	- [MS8607](https://www.sparkfun.com/products/16298) PHT sensor
	- [SDP31](https://www.sparkfun.com/products/17874) differential pressure sensor
	- [SHTC3](https://www.sparkfun.com/products/16467) humidity and temperature sensor
- Air Quality and Environmental Sensors:
	- [CCS811](https://www.sparkfun.com/products/14193) air quality sensor
	- [SCD30](https://www.sparkfun.com/products/15112) CO<sub>2</sub> humidity and temperature sensor
	- [SCD40](https://www.sparkfun.com/products/18365) CO<sub>2</sub> humidity and temperature sensor
	- [SGP30](https://www.sparkfun.com/products/16531) air quality and Volatile Organic Compound (VOC) sensor
	- [SGP40](https://www.sparkfun.com/products/17729) air quality (VOC Index) Sensor
	- [STC31](https://www.sparkfun.com/products/18385) CO<sub>2</sub> sensor
	- [VEML6075](https://www.sparkfun.com/products/15089) UV light sensor
	- [VEML7700](https://www.sparkfun.com/products/18981) ambient light sensor
- Precision Temperature Sensors:
	- [Qwiic PT100 ADS122C04](https://www.sparkfun.com/products/16770) platinum resistance sensor
	- [MCP9600](https://www.sparkfun.com/products/16294) thermocouple amplifier
	- [TMP117](https://www.sparkfun.com/products/15805) precision temperature sensor
- Global Navigation Satellite System (GNSS) navigation data:
	- SFE QUAD can be linked to any of SparkFun’s u-blox GNSS boards allowing you to log accurate position, velocity and time data
- Distance:
	- [VCNL4040](https://www.sparkfun.com/products/15177) proximity sensor
	- [VL53L1X](https://www.sparkfun.com/products/14722) laser Time of Flight (ToF) sensor
- Weight:
	- [NAU7802](https://www.sparkfun.com/products/15242) load cell sensor
- Battery Condition:
	- [MAX17048](https://www.sparkfun.com/products/17715) fuel gauge
- Waypoint / Event logging:
	- [Qwiic Button](https://www.sparkfun.com/products/15932)

## Tested Platforms

- [ESP32 with microSD](https://www.sparkfun.com/products/18018)
- [ESP32 with EEPROM or LittleFS](https://www.sparkfun.com/products/15663)
- [Artemis (Apollo3) with EEPROM](https://www.sparkfun.com/products/15574)
- [SAMD21 with FlashAsEEPROM_SAMD as EEPROM](https://www.sparkfun.com/products/14812)
- [SAMD51 with FlashAsEEPROM_SAMD as EEPROM](https://www.sparkfun.com/products/14713)
- [STM32 with FlashStorage_STM32 as EEPROM](https://www.sparkfun.com/products/17712)
- [RP2040 with microSD](https://www.sparkfun.com/products/17745) following the instructions in the [Hookup Guide](https://learn.sparkfun.com/tutorials/rp2040-thing-plus-hookup-guide/software-overview) and using the pin definitions shown below:

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

SparkFun Qwiic Universal Auto-Detect is licensed using the Open Source MIT License