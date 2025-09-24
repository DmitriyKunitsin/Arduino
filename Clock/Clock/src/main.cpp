#include <Arduino.h>

#include "ClockDisplay.h"
#include "UARTHandler.h"

ClockDisplay clockDisplay;
UARTHandler uartHandler;

void setup() {
    clockDisplay.setup();
    uartHandler.begin(103);
    pinMode(LED_BUILTIN, OUTPUT);
    sei();
}
void loop() {
    clockDisplay.loop();

    if (uartHandler.isPackageReady()) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
