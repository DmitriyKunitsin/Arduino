#include <MyWiFi.h>

const char *_LOGIN_AP = "AP_DIMA_K";
const char *_PASSWORD_AP = "PASSWORD";

const char *_LOGIN_STA = "DimaAP";
const char *_PASSWORD_STA = "ssaf2866";

MyClassEsp8266 CustomWiFiModule(_LOGIN_AP, _PASSWORD_AP, _LOGIN_STA, _PASSWORD_STA);

#ifndef LED_BUILTIN
#define LED_BUILTIN
#endif
bool blink = true;

WiFiUDP ntpUDP;
TimeManager timeClient(ntpUDP);

void setup()
{
    Serial.begin(115200);
    CustomWiFiModule.setupingTwoModes();
    // CustomWiFiModule.initAPmode();
    CustomWiFiModule.serverOn();
    timeClient.begin();
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED_BUILTIN, blink);
    blink = !blink;
    auto func = CustomWiFiModule.isConnectedToWifi(); // Создаем ссылку на лямбду
    bool result = func();                             // Вычисляем результат
    if (result)
    {
        digitalWrite(LED_BUILTIN, false);
        if (CustomWiFiModule.ConnectedToWIfi())
        {
            Serial.println("Succerful connect to WiFi");
        }
        else
        {
            Serial.println("Aborted connect to WiFi");
        }
        digitalWrite(LED_BUILTIN, true);
    }
    delay(250);
    timeClient.update();
    Serial.println(timeClient.GetFormattedTimeForDisplay());
}