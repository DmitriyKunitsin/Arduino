#include <Arduino.h>

#include "ClockDisplay.h"
#include "UARTHandler.h"

ClockDisplay clockDisplay;
UARTHandler uartHandler;

void setup() {
    clockDisplay.setup();
    uartHandler.begin(103);
    sei();
}

void loop() {
    clockDisplay.loop();
    if(uartHandler.isPackageReady()) {

    }
}

