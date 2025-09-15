#include "MyWiFi.h"

MyClassEsp8266::MyClassEsp8266(String login, String password, String logSTA, String passSTA) {
    this->_loginAP = login;
    this->_passwordAP = password;
    this->_loginSTA = logSTA;
    this->_passwordSTA = passSTA;
}

void MyClassEsp8266::setupWiFiApMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->_loginAP, this->_passwordAP);
}

void MyClassEsp8266::setupWiFiSTAMode() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
}

bool MyClassEsp8266::setupingTwoModes() {
    WiFi.mode(WIFI_AP_STA);
    if (!_loginAP.length() || !_passwordAP.length() || !_loginSTA.length() || !_passwordSTA.length()) {
        // Если хотя бы одна пустая, возвращаем false (не готовы)
        return false;
    }
    WiFi.softAP(this->_loginAP, this->_passwordAP);
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
    int countTry = 0;
    Serial.print("Connecting to Wifi " + this->_loginSTA);
    while(WiFi.status() != WL_CONNECTED) {// если не подключены
        delay(500);
        Serial.println("Try connecting " + countTry );
        if(countTry == 10) {
            return false;
        }
    }
    Serial.println("Connecting succerful!");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
    return true;
}
