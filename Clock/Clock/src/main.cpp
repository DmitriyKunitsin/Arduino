#include <Arduino.h>

#include "ClockDisplay.h"
#include "UARTHandler.h"

ClockDisplay clockDisplay;
UARTHandler uartHandler('\x02','\x03');

void setup() {
    clockDisplay.setup();
    uartHandler.begin(103);
    // Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    sei();
    String welcome = "Arduino Nano has initilizated";
    for(int i = 0 ; i < welcome.length(); i++) {
        uartHandler.transmit(welcome[i]);
    }
}


unsigned char ans[] = "Is Ready Package";
bool tt = false;
void loop() {
    clockDisplay.loop();
    if (uartHandler.isPackageReady()) {
        digitalWrite(LED_BUILTIN, tt);
        tt = !tt;
        uartHandler.AsyncStringTransmit(uartHandler.getBuffer());
        // uartHandler.AsyncStringTransmit(ans);
        delay(1000);
        // uartHandler.asyncTransmit(uartHandler.getPackageLength());
    } else {
        // digitalWrite(LED_BUILTIN, LOW);
        // delay(1000);
    }
}
 