#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include <iostream>
#include <string.h>
#include <esp8266WiFi.h>

class esp8266 {
    
public:
    /// @brief Конструктор инициализирует объект WiFi с указаным логином и паролем 
    /// @param login логин 
    /// @param password пароль
    esp8266(std::string login, std::string password);
    /// @brief метод поднимает точку с WiFi, доступен будет по 192.168.4.1
    void setupWiFi();

private:
    std::string _login;
    std::string _password;
};

#endif // ESP8266WIFI_H