#ifndef MYWIFI_H
#define MYWIFI_H

// #include <ESPAsyncWebServer.h>
#include <EspWebServer.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <TimeManager.hpp>
// #include <NTPClient.h>
// #include <WiFiUdp.h>

class MyClassEsp8266
{

public:
    /// @brief Конструктор инициализирует объект WiFi с указаным логином и паролем
    /// @param loginAP логин
    /// @param passwordAP пароль
    MyClassEsp8266(const char *const loginAP, const char *const passwordAP,
                   const char *loginSTA, const char *passwordSTA, WiFiUDP &udpInstance);
    /// @brief иницирует Acess poin режим
    void initAPmode();
    /// @brief метод инициализирует модуль и пытается подключиться к переданному вай фай
    void initWiFiSTAMode();
    /// @brief инициализация двух режимов работы, пытается 5 сек достучаться до STA точки, в случае неудачи возвращает false
    /// @return в случае неудачи подклчюения к точки, возвращает false, иначе true
    bool setupingTwoModes();
    /// @brief
    /// @return
    bool serverOn();
    /// @brief Метод обращается к приватному серверу и запрашивает у него сохранённый SSID wifi
    /// @return строку SSID-a
    const char *getWifiSSID();
    /// @brief Метод обращается к приватному серверу и запрашивает у него сохранённый password wifi
    /// @return строку password
    const char *getWifiPassword();
    /// @brief
    /// @return
    bool ConnectedToWIfi();
    /// @brief
    /// @return
    std::function<bool()> isConnectedToWifi() const
    {
        std::function<bool()> lambda = [this]() -> bool
        {
            return this->server.isConnWifi;
        };
        return lambda;
    }
private:
    const char *_loginAP;
    const char *_passwordAP;
    const char *_loginSTA;
    const char *_passwordSTA;
    EspWebServer server;
    TimeManager timer;
    /// @brief IP адрес устройства
    IPAddress deviceIP;
};

#endif // ESP8266WIFI_H