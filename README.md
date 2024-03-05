# ESP32_RTC_EEPROM

#### Emulation of Arduino EEPROM in RTC RAM instead of flash

*"Survives deep sleep, but not reset, powercycle or reflashing."*

**Do not use this library if you just want the data in some variables to survive deep sleep. Just mark the variables `RTC_DATA_ATTR` and you're all set. There's a tutorial [here](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/). This library is for you only if you have existing code that uses [`EEPROM.h`](https://github.com/espressif/arduino-esp32/blob/master/libraries/EEPROM/src/EEPROM.h), and you're okay with having anything written to EEPROM not survive reset, power loss or flashing new code.**

&nbsp;

### Introduction

Flash is subject to wear from being rewritten. Flash in Espressif's own modules is guaranteed to 100.000 writes, generic SPI flash modules are generally rated anywhere from 10.000 to 100.000. **BUT** this is generally assuming that the chip is at 20 °C (68 °F). Inside a module it's always going to be hotter, and there's plenty of horror stories on the net of SPI flash chips lasting orders of magnitude flash cycles less than advertised.  

Espressif's ESP32 Arduino code includes an Arduino EEPROM implementation that writes the EEPROM to flash as blob in their NVS key-value storage. What this effectively means is that for every change of even just one bit in the "EEPROM", your entire EEPROM is rewritten to flash, the old copy marked deleted, and if the flash page fills up, a new page is marked Active and so forth. This can lead to more flash wear than one would expect, the exact details are [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/nvs_flash.html#internals).

If your application keeps the ESP32 battery-powered but puts it in deep sleep, it uses so little power it's effectively off as far as battery life is concerned. You can have it wake up again after a certain amount of time has expired and/or if a GPIO (i.e. button) goes high or low.

This library will emulate EEPROM in RTC RAM, which stays powered during deep sleep. To learn more about deep sleep, check [this easy tutorial](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/), or [Espressif's documentation](https://docs.espressif.com/projects/esp-idf/en/v5.2/esp32s3/api-reference/system/sleep_modes.html) if you want all the details. Like mentioned above, use this library only if your existing code uses EEPROM. Simply having some variables survive deep sleep is way easier, check the tutorial.

### Optional saving to NVS (flash)

If you call `EEPROM.toNVS()`, a copy of your EEPROM data will be save to the key "eeprom" in NVS flash, exactly like when you would use the original `EEPROM.h`. Whenever the ESP32_RTC_EEPROM wakes up with an empty RTC RAM (which is detects by the presence of a magic word), it will try to see if there's a saved copy of the right size. If so, that is loaded. You can also manually revert to the last save by calling `EEPROM.fromNVS()`.

The function `EEPROM.wasRestored()` will tell you whether or not the contents were automaticcaly restored from NVS in this boot cycle.

### Usage

1. Add this library using the library manager or by cloning this repository into your Arduino library folder. 

2. Replace `#include <EEPROM.h>` in your code by `#include <ESP32_RTC_EEPROM.h>`.

3. There is no 3, you are done.

### Details

* This library will always claim `EEPROM_SIZE` (set at 2048, change in the .h file) bytes of the RTC's RAM (it has 8k). You can set a lower number when calling `EEPROM.begin()`, but that number only  limits what your code can read/write (and how much is written to NVS flash every time when backing up), not the actual RTC RAM used.

* Cannot repeat it often enough: **only useful in combination with the ESP's deep sleep, data in this fake EEPROM does not survive powercycling, resetting or reflashing** 
