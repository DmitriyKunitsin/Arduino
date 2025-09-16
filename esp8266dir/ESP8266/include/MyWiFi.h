#ifndef MYWIFI_H
#define MYWIFI_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h> 

class MyClassEsp8266 {
    
public:
    /// @brief Конструктор инициализирует объект WiFi с указаным логином и паролем 
    /// @param loginAP логин 
    /// @param passwordAP пароль
    MyClassEsp8266(String loginAP, String passwordAP, String loginSTA, String passwordSTA);
    /// @brief метод поднимает точку с WiFi, доступен будет по 192.168.4.1
    void setupWiFiApMode();
    /// @brief метод инициализирует модуль и пытается подключиться к переданному вай фай
    void setupWiFiSTAMode();
    /// @brief инициализация двух режимов работы, пытается 5 сек достучаться до STA точки, в случае неудачи возвращает false
    /// @return в случае неудачи подклчюения к точки, возвращает false, иначе true
    bool setupingTwoModes();
    bool serverOn();

private:
    String _loginAP;
    String _passwordAP;
    String _loginSTA;
    String _passwordSTA;
    AsyncWebServer server;

};

#endif // ESP8266WIFI_H