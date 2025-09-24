#include <MyWiFi.h>

const char *_LOGIN_AP = "AP_DIMA_K";
const char *_PASSWORD_AP = "PASSWORD";

const char *_LOGIN_STA = "Xiaomi_1C16";  // home : Xiaomi_1C16
const char *_PASSWORD_STA = "20081111";

WiFiUDP ntpUDP;
MyClassEsp8266 CustomWiFiModule(_LOGIN_AP, _PASSWORD_AP, _LOGIN_STA, _PASSWORD_STA, ntpUDP);

#ifndef LED_BUILTIN
#define LED_BUILTIN
#endif
bool blink = true;

void setup() {
    Serial.begin(9600);
    Serial.println("next step setup modes");
    CustomWiFiModule.setupingTwoModes();
    Serial.println("next step serverOn");
    CustomWiFiModule.serverOn();

    pinMode(LED_BUILTIN, OUTPUT);
}
unsigned int delayTimer = 250;
unsigned int cnt = 0;
void loop() {
    digitalWrite(LED_BUILTIN, blink);
    blink = !blink;
    auto func = CustomWiFiModule.isConnectedToWifi();  // Создаем ссылку на лямбду
    bool result = func();                              // Вычисляем результат
    if (result) {
        digitalWrite(LED_BUILTIN, false);
        if (CustomWiFiModule.ConnectedToWIfi()) {
            Serial.println("Succerful connect to WiFi");
            Serial.println(CustomWiFiModule.getNetworksTimeFormattedDislpay());
            // myPort.println(CustomWiFiModule.getNetworksTimeFormattedDislpay());
        } else {
            Serial.println("Aborted connect to WiFi");
        }
        digitalWrite(LED_BUILTIN, true);
    }
    delay(delayTimer);
    if (WiFi.softAPgetStationNum() > 0) {
        int numClients = WiFi.softAPgetStationNum();
        delayTimer = 50;
        if (cnt % 25 == 0) {
            // Serial.print("Conncted device : ");
            // Serial.print(numClients);
            Serial.print("\x02Hello\x03");
        }
    } else {
        delayTimer = 250;
    }
    cnt++;
    // timeClient.update();
    // Serial.println(timeClient.GetFormattedTimeForDisplay());
}