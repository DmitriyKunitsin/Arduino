#include "MyWiFi.h"

MyClassEsp8266::MyClassEsp8266(const char *const login, const char *const password,
                               const char *logSTA, const char *passSTA, WiFiUDP &udpInstance)
    : _loginAP(login),
      _passwordAP(password),
      _loginSTA(logSTA),
      _passwordSTA(passSTA),
      deviceIP(192, 168, 4, 1),
      timer(udpInstance),
      server() {
    Serial.println("init construct class for module WiFi");
}
void MyClassEsp8266::initAPmode() {
    this->server.setupWiFiApMode(this->_loginAP, this->_passwordAP);
}
void MyClassEsp8266::initWiFiSTAMode() {
    this->server.setupWiFiSTAmode(this->_loginSTA, this->_passwordSTA);
}
bool MyClassEsp8266::ConnectedToWIfi() {
    return this->server.tryConnectedToSTA();
}
bool MyClassEsp8266::setupingTwoModes() {
    this->server.setupTwoModes(this->_loginAP, this->_passwordAP, this->_loginSTA, this->_passwordSTA);
    return true;
}

bool MyClassEsp8266::serverOn() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mout failed");
        return false;
    }
    server.on("/hi", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("was request");
        if (LittleFS.exists("/pageForInitClock.html")) {
            request->send(LittleFS, "/pageForInitClock.html", "text/html");
        } else {
            request->send(404, "text/plain", "File not found");
        } });

    server.on("/login", HTTP_POST, [this](AsyncWebServerRequest *request) { server.onRequest(request); }, nullptr, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) { server.onBody(request, data, len, index, total); });
    server.on("/set-time", HTTP_POST, [this](AsyncWebServerRequest *request) { server.onRequest(request); }, nullptr, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) { server.onBody(request, data, len, index, total); });
    server.begin();
    // // Создаём middleware для логирования
    // // AsyncLoggingMiddleware *logging = new AsyncLoggingMiddleware();
    // // logging->setOutput(Serial); // Вывод в Serial
    // // logging->setEnabled(true);
    // // server.addMiddleware(logging); // Добавляем к серверу

    Serial.println("Server started. Open http://" + this->deviceIP.toString());
    return true;
}

const char *MyClassEsp8266::getWifiSSID() {
    return this->server.getWifiSSID();
}
const char *MyClassEsp8266::getWifiPassword() {
    return this->server.getWifiPassword();
}

String MyClassEsp8266::getSetHour() {
    return this->server.getSetHour();
}

String MyClassEsp8266::getSetMin() {
    return this->server.getSetMin();
}

String MyClassEsp8266::getNetworksTimeFormattedDislpay() {
    this->timer.update();
    return this->timer.GetFormattedTimeForDisplay();
}