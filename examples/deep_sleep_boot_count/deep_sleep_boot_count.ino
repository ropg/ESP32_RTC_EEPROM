/* This will show you the boot count, but only if you use the button to wake up from
   from deep sleep. As you will see, hitting the reset button, reflashing and powering
   down will all start the count at zero again.
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

  // Print boot count
  Serial.println("Boot count: " + String(EEPROM.read(0)));
  // Write new value
  EEPROM.write(0, EEPROM.read(0) + 1);
  
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
