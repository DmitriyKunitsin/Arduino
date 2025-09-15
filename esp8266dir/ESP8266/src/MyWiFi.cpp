#include "MyWiFi.h"

esp8266::esp8266(std::string login, std::string password) {
    this->_login = login;
    this->_password = password;
}
