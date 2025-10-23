#include <Arduino.h>
#include <ArduinoJson.h>

#include "ClockDisplay.h"
#include "UARTHandler.h"

ClockDisplay clockDisplay;
UARTHandler uartHandler('\x02', '\x03');

void setup() {
    clockDisplay.setup();
    uartHandler.begin(103);
    // Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    sei();
    String welcome = "Arduino Nano has initilizated";
    for (int i = 0; i < welcome.length(); i++) {
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
        StaticJsonDocument<200> Time;

        DeserializationError error = deserializeJson(Time, uartHandler.getFilledBuffer());
        if (error) {
            uartHandler.AsyncStringTransmit("error");
            // uartHandler.clearBuffer();  // TODO добавить метод очистки буфера
            return;
        }
        unsigned char hour = Time["hour"].as<unsigned char>();
        unsigned char minute = Time["minute"].as<unsigned char>();

        clockDisplay.externalUpdateTimes(hour, minute);
        delay(250);
    } else {
        // digitalWrite(LED_BUILTIN, LOW);
        // delay(1000);
    }
}
