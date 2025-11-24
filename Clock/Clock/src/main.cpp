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
        StaticJsonDocument<200> Time;
        
        DeserializationError error = deserializeJson(Time, uartHandler.getFilledBuffer());
        if (error) {
            uartHandler.AsyncStringTransmit("error");
            uartHandler.clearBuffer();
            return;
        }
        unsigned char hour = Time["hour"].as<unsigned char>();
        unsigned char minute = Time["minute"].as<unsigned char>();
        
        // Извлечение цифр для hour (всегда две: 00–23)
        unsigned char hourTens = hour / 10;   // Десятки (0–2)
        unsigned char hourUnits = hour % 10;  // Единицы (0–9)
        
        // Извлечение цифр для minute (всегда две: 00–59)
        unsigned char minTens = minute / 10;   // Десятки (0–5)
        unsigned char minUnits = minute % 10;  // Единицы (0–9)
        
        unsigned char time[4] = {hourTens, hourUnits, minTens, minUnits};
        for(int i = 0; i < 4; i++) {
            String str = String(time[i]);
            uartHandler.AsyncStringTransmit("\ntime :");
            uartHandler.AsyncStringTransmit((const unsigned char*)str.c_str());
        }
        clockDisplay.externalUpdateTimes(time);
        uartHandler.clearBuffer();
        digitalWrite(LED_BUILTIN, tt);
        tt = !tt;
        // clockDisplay.loop();
    } else {
        // digitalWrite(LED_BUILTIN, LOW);
        // delay(1000);
    }
}
