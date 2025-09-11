#include <Arduino.h>
#include "ClockDisplay.h"

ClockDisplay clockDisplay;

void setup() {
    clockDisplay.setup();
}

void loop() {
    clockDisplay.loop();
}
