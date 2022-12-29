# ESP32 driver for [MCP9808](https://ww1.microchip.com/downloads/en/DeviceDoc/25095A.pdf) I2C temperature sensor.

## Table of Contents

- [ESP32 driver for MCP9808 I2C temperature sensor.](#esp32-driver-for-mcp9808-i2c-temperature-sensor)
  - [Table of Contents](#table-of-contents)
  - [About](#about)
  - [Use with PlatformIO](#use-with-platformio)
  - [Use with esp-idf](#use-with-esp-idf)
  - [Maintainer](#maintainer)

## About

## Use with PlatformIO

- Add the library to the `lib_deps` parameter

```sh
lib_deps =
    https://gitlab.server-kampert.de/esp32/libraries/mcp9808.git
```

- Run `pio -t menuconfig` from the root of your project to configure the driver

## Use with esp-idf

- Create a directory `components` in your project root
- Clone the repository into the `components` directory
- Build the project

## Maintainer

- [Daniel Kampert](mailto:daniel.kameprt@kampis-elektroecke.de)
