/* This will show you the boot count, but only if you use the button to wake up from
   from deep sleep. As you will see, hitting the reset button, reflashing and powering
   down will all start the count at the last multiple of 10, because that is the last
   time the contents of the RTC RAM were copied to NVS (flash).
*/ 

#define BUTTON_GPIO GPIO_NUM_0
#define BUTTON_ACTIVE LOW

#include <ESP32_RTC_EEPROM.h>

void setup() {
  Serial.begin(115200);

  // You can set any number up to EEPROM_SIZE (set at 2048) from ESP32_RTC_EEPROM.h
  // While what you specify in begin() will limit what you can read/write, "EEPROM" 
  // data will always take up EEPROM_SIZE in RTC RAM.
  EEPROM.begin(512);

  // Read from "EEPROM"
  uint8_t bootcount = EEPROM.read(0);

  // Increase bootcount
  bootcount++;

  // Print boot count
  Serial.println("Boot count: " + String(bootcount));

  // See if this was restored from NVS
  if (EEPROM.wasRestored()) {
    Serial.println("(This value was restored from NVS, so we may have missed a few.)");
  }
  
  // Write new value
  EEPROM.write(0, bootcount);

  // Save a copy to NVS every 10 boots, so we can resume from this point if we reset / lose power
  if (bootcount % 10 == 0) {
    Serial.println("Backing up the current EEPROM data from RTC RAM to NVS (flash).");
    EEPROM.toNVS();
  }
  
  // Data is written immediately, calls to commit() and end() are ignored
  EEPROM.commit();
  EEPROM.end();

  // Print that we're going to sleep
  Serial.println("Going to deep sleep, wake me up with the button.");

  // But first wait for button to be released, or will wake up again immediately
  delay(100);
  while (digitalRead(BUTTON_GPIO) == BUTTON_ACTIVE);

  // Set button as wakeup source
  esp_sleep_enable_ext0_wakeup(BUTTON_GPIO, BUTTON_ACTIVE);

  // Go to deep sleep now
  esp_deep_sleep_start();
}

void loop() {
  // Nothing here
}
