#include "MyWiFi.h"
MyClassEsp8266::MyClassEsp8266(const char *const login, const char *const password,
                               const char *logSTA, const char *passSTA, WiFiUDP &udpInstance)
    : _loginAP(login), _passwordAP(password), _loginSTA(logSTA), _passwordSTA(passSTA), deviceIP(192, 168, 4, 1), timer(udpInstance)
{
    Serial.println("init construct class for module WiFi");
}
void MyClassEsp8266::initAPmode()
{
    this->server.setupWiFiApMode(this->_loginAP, this->_passwordAP);
}
void MyClassEsp8266::initWiFiSTAMode()
{
    this->server.setupWiFiSTAmode(this->_loginSTA, this->_passwordSTA);
}
bool MyClassEsp8266::ConnectedToWIfi()
{
    return this->server.tryConnectedToSTA();
}
bool MyClassEsp8266::setupingTwoModes()
{
    WiFi.enableInsecureWEP(false);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    WiFi.disconnect();
    WiFi.softAP(this->_loginAP, this->_passwordAP);
    this->server.GetAndCheckSumAccessPoints();
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
    int countTry = 0;
    Serial.println("Connecting to Wifi " + String(this->_loginSTA));
    while (WiFi.status() != WL_CONNECTED && countTry <= 30)
    {
        Serial.println("Try connecting " + String(countTry));
        delay(500);
        countTry++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connecting successful!");
        Serial.print("IP address : ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else
    {
        Serial.println("Failed to connect to WiFi after multiple attempts");
        // initAPmode();
        return false;
    }
}

bool MyClassEsp8266::serverOn()
{
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS mout failed");
        return false;
    }
    server.on("/hi", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        Serial.println("was request");
        if (LittleFS.exists("/pageForInitClock.html")) {
            request->send(LittleFS, "/pageForInitClock.html", "text/html");
        } else {
            request->send(404, "text/plain", "File not found");
        } });

    server.on("/login", HTTP_POST, [this](AsyncWebServerRequest *request)
              { server.onRequest(request); }, nullptr, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { server.onBody(request, data, len, index, total); });
    server.on("/set-time", HTTP_POST, [this](AsyncWebServerRequest *request)
              { server.onRequest(request); }, nullptr, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { server.onBody(request, data, len, index, total); });
    server.begin();
    // // Создаём middleware для логирования
    // // AsyncLoggingMiddleware *logging = new AsyncLoggingMiddleware();
    // // logging->setOutput(Serial); // Вывод в Serial
    // // logging->setEnabled(true);
    // // server.addMiddleware(logging); // Добавляем к серверу

    Serial.println("Server started. Open http://" + this->deviceIP.toString());
    return true;
}

const char *MyClassEsp8266::getWifiSSID()
{
    return this->server.getWifiSSID();
}
const char *MyClassEsp8266::getWifiPassword()
{
    return this->server.getWifiPassword();
}