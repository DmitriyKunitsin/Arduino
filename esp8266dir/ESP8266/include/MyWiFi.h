#ifndef MYWIFI_H
#define MYWIFI_H

// #include <ESPAsyncWebServer.h>
#include <EspWebServer.h>
#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>

class MyClassEsp8266
{

public:
    /// @brief Конструктор инициализирует объект WiFi с указаным логином и паролем
    /// @param loginAP логин
    /// @param passwordAP пароль
    MyClassEsp8266(char* loginAP,char* passwordAP, String loginSTA, String passwordSTA);
    /// @brief иницирует Acess poin режим
    void initAPmode();
    /// @brief метод инициализирует модуль и пытается подключиться к переданному вай фай
    void setupWiFiSTAMode();
    /// @brief инициализация двух режимов работы, пытается 5 сек достучаться до STA точки, в случае неудачи возвращает false
    /// @return в случае неудачи подклчюения к точки, возвращает false, иначе true
    bool setupingTwoModes();
    /// @brief 
    /// @return 
    bool serverOn();
    /// @brief Метод обращается к приватному серверу и запрашивает у него сохранённый SSID wifi
    /// @return строку SSID-a
    const char* getWifiSSID();
private:
    char* _loginAP;
    char* _passwordAP;
    String _loginSTA;
    String _passwordSTA;
    EspWebServer server;
    /// @brief IP адрес устройства
    IPAddress deviceIP;
};

#endif // ESP8266WIFI_H