#include <MyWiFi.h>

const char* _LOGIN_AP = "AP_DIMA_K";
const char* _PASSWORD_AP = "PASSWORD";

const char* _LOGIN_STA = "Xiaomi_1C16";
const char* _PASSWORD_STA = "20081111";

MyClassEsp8266 CustomWiFiModule(_LOGIN_AP, _PASSWORD_AP, _LOGIN_STA, _PASSWORD_STA);

#ifndef LED_BUILTIN
#define LED_BUILTIN
#endif
bool blink = true;
void setup() {
    Serial.begin(9600);
    // CustomWiFiModule.setupingTwoModes();
    CustomWiFiModule.setupWiFiApMode();
    CustomWiFiModule.serverOn();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, blink);
    blink = !blink;
    delay(1000);
}