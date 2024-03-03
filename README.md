# ThNetwork
Implementation of sensors network for capturing temperature and humidity. 

# How it works
Transmitters based on Esp8266 reads data from ATH10 (temperature and humidity) and sends it over ESP-NOW protocol to host board. Host is based on Esp32 and provides website with readings chart and administration view.

![img](resources/diagram.png)

## Charts and admin view
![img](resources/demo.png)

# Setup
Project uses `PlatformIO` as buildsystem and contains two targets: `host` and `transmitter`. `host` is prepared for `esp32` board, and `transmitter` for `esp8266`. `CMakeLists.txt` located in project is used only for building unit tests.

# Configuration
## First setup
During first startup host will provide access point named `TH-NETWORK`. By connecting to it and opening the address 192.168.4.1 you can setup wifi connection. When you will do it board should reboot and connect to provided network.

## Pairing
You have to enable pairing on host (by pair button), then hold `pair` button on the transmitter and click reset. After pairing, transmitter should be visible in `Admin` view, were you can change its name.

# Boards preparation
The project was built based on prototype boards nodemcu (esp8266) and node32s (esp32).
![img](resources/boards.jpg)
Transmitter board have one additional connection: D0 is connected with reset pin. It is mandatory as transmitter after sending data is going to deep sleep for specified time. Board will not be able to wake up without this connection.

## Pinout
Pinout can be customized by modification of:
 - Host: `src/host/BoardConfiguration.hpp`
 - Transmitter: `src/transmitter/BoardConfiguration.hpp`

## Default configuration
### Transmitter (nodemcu)
ATH10
 - SCL: D5
 - SDA: D6

Pair button
 - pin: D2

### Host (node32s)
Buttons
 - wifi: 14
 - pair/factory reset: 18

Led
 - pin: 23

# Tests
Unit tests are written in `cpputest`. To build them you have to use CMake:
```
$ cmake -B build
$ cmake --build build
$ ./build/HostUTs && ./build/TransmitterUTs && ./build/CommonUTs
```