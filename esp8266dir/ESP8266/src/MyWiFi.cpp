#include "MyWiFi.h"

MyClassEsp8266::MyClassEsp8266(String login, String password, String logSTA, String passSTA)
    : _loginAP(login), _passwordAP(password), _loginSTA(logSTA), _passwordSTA(passSTA), server(80) {
}

void MyClassEsp8266::setupWiFiApMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->_loginAP, this->_passwordAP);
    Serial.println("init AP mode");
}

void MyClassEsp8266::setupWiFiSTAMode() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->_loginSTA, this->_passwordSTA);
    Serial.println("init STA mode");
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
    while (WiFi.status() != WL_CONNECTED) {  // если не подключены
        delay(500);
        Serial.println("Try connecting " + countTry);
        countTry++;
        if (countTry == 10) {
            break;
            return false;
        }
    }
    Serial.println("Connecting succerful!");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
    return true;
}

bool MyClassEsp8266::serverOn() {
    // if (WiFi.status() != WL_CONNECTED) {
    //     return false;
    // }
    // передаем обработчик как лямбду
    server.on("/hi", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = R"html(
            <!DOCTYPE html>
            <html>
            <head>
                <title>My page</title>
            </head>
            <body>
                <h1>Hello, world!</h1>
                <p>This is my HTML-page for ESP8266.</p>
                <button onclick="alert('Клик!')">Click me!</button>
            </body>
            </html>
        )html";
        request->send(200, "text/html", html);
    });

    server.begin();
    return true;
}