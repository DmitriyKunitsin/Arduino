#ifndef MYWIFI_H
#define MYWIFI_H

#include <ESP8266WiFi.h>
#include <Arduino.h> 

class MyClassEsp8266 {
    
public:
    /// @brief Конструктор инициализирует объект WiFi с указаным логином и паролем 
    /// @param login логин 
    /// @param password пароль
    MyClassEsp8266(String login, String password);
    /// @brief метод поднимает точку с WiFi, доступен будет по 192.168.4.1
    void setupWiFi();

private:
    String _login;
    String _password;
};

#endif // ESP8266WIFI_H