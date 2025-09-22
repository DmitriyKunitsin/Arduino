#include "MyWiFi.h"
MyClassEsp8266::MyClassEsp8266(char* login, char* password, String logSTA, String passSTA)
    : _loginAP(login), _passwordAP(password), _loginSTA(logSTA), _passwordSTA(passSTA), deviceIP(192, 168, 4, 1)
{
}

void MyClassEsp8266::initAPmode() {
    Serial.println("InitAPPPPPPPP");
    this->server.setupWiFiApMode(this->_loginAP , this->_passwordAP);
}

void MyClassEsp8266::setupWiFiSTAMode()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
    Serial.println("init STA mode");
}

bool MyClassEsp8266::setupingTwoModes()
{
    WiFi.mode(WIFI_AP_STA);
    if (!_loginSTA.length() || !_passwordSTA.length())
    {
        // Если хотя бы одна пустая, возвращаем false (не готовы)
        return false;
    }
    WiFi.softAP(this->_loginAP, this->_passwordAP);
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
    int countTry = 0;
    Serial.print("Connecting to Wifi " + this->_loginSTA);
    while (WiFi.status() != WL_CONNECTED)
    { // если не подключены
        delay(500);
        Serial.println("Try connecting " + countTry);
        countTry++;
        if (countTry == 10)
        {
            break;
            return false;
        }
    }
    Serial.println("Connecting succerful!");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
    return true;
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

const char* MyClassEsp8266::getWifiSSID() {
    return this->server.getWifiSSID();
}