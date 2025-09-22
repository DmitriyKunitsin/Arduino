#include <MyWiFi.h>

const char *_LOGIN_AP = "AP_DIMA_K";
const char *_PASSWORD_AP = "PASSWORD";

const char *_LOGIN_STA = "Xiaomi_1C16"; // home : Xiaomi_1C16
const char *_PASSWORD_STA = "20081111";

WiFiUDP ntpUDP;
MyClassEsp8266 CustomWiFiModule(_LOGIN_AP, _PASSWORD_AP, _LOGIN_STA, _PASSWORD_STA, ntpUDP);

#ifndef LED_BUILTIN
#define LED_BUILTIN
#endif
bool blink = true;

// TimeManager timeClient(ntpUDP);

// // // Порты для связи с Arduino Nano
// // const int rxPin = 1;  // Pin для RX (приёма данных)
// // const int txPin = 3;  // Pin для TX (передачи данных)

// // // Создаем объект SoftwareSerial
// // SoftwareSerial mySerial(rxPin, txPin); // RX, TX
// #define MYPORT_TX 1
// #define MYPORT_RX 3
// EspSoftwareSerial::UART myPort;

void setup()
{
    Serial.begin(115200);
    Serial.println("next step setup modes");
    CustomWiFiModule.setupingTwoModes();
    Serial.println("next step serverOn");
    CustomWiFiModule.serverOn();

    // myPort.begin(115200, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
    // if (!myPort)
    // {
    //     Serial.println("Invalid EspSoftwareSerial pin configuration, check config");
    //     while (1)
    //     { // Don't continue with invalid configuration
    //         delay(1000);
    //     }
    // }

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
            Serial.println(CustomWiFiModule.getNetworksTimeFormattedDislpay());
            // myPort.println(CustomWiFiModule.getNetworksTimeFormattedDislpay());
        }
        else
        {
            Serial.println("Aborted connect to WiFi");
        }
        digitalWrite(LED_BUILTIN, true);
    }
    delay(250);
    // timeClient.update();
    // Serial.println(timeClient.GetFormattedTimeForDisplay());
}