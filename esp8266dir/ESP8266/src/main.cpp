#include "MyWiFi.h"

const char* _LOGIN = "WiFi_DIMA_K";
const char* _PASSWORD = "PASSWORD";

MyClassEsp8266 WiFiModule(_LOGIN, _PASSWORD);

void setup() {
    WiFiModule.setupWiFi();
}

void loop() {

}