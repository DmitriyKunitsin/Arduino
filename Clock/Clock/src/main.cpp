#include <Arduino.h>

#include "ClockDisplay.h"
#include "UARTHandler.h"

ClockDisplay clockDisplay;

void setup() {
    clockDisplay.setup();
    UARTHandler uartHandler;
    uartHandler.begin(103);
    sei();
}

void loop() {
    clockDisplay.loop();
    if(uartHandler.isPackageReady()) {

    }
}

