#include "MyWiFi.h"

MyClassEsp8266::MyClassEsp8266(String login, String password) {
    this->_login = login;
    this->_password = password;
}

void MyClassEsp8266::setupWiFi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->_login, this->_password);
}
