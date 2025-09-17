#include "MyWiFi.h"

MyClassEsp8266::MyClassEsp8266(String login, String password, String logSTA, String passSTA)
    : _loginAP(login), _passwordAP(password), _loginSTA(logSTA), _passwordSTA(passSTA), server(80), deviceIP(192, 168, 4, 1) {
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
        }
    });
    server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
        Serial.println("Button login was clicked!");
        // Проверяем, есть ли тело запроса
        if (request->hasParam("body", true)) {  // true — для тела
            String body = request->getParam("body", true)->value();
            Serial.println("Body: " + body);

            // Парсим JSON (требуется ArduinoJson)
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, body);
            if (error) {
                Serial.println("JSON parse error: " + String(error.c_str()));
                request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
                return;
            }

            String username = doc["username"];
            String password = doc["password"];

            // Здесь можно добавить логику: сохранить WiFi-креды, подключиться и т.д.
            Serial.println("Username: " + username);
            Serial.println("Password: " + password);

            // Отправляем ответ
            request->send(200, "application/json", "{\"status\":\"Login data received\"}");
        } else {
            Serial.println("No body");
            request->send(400, "application/json", "{\"error\":\"No body\"}");
        }
    });

    server.begin();
    // Создаём middleware для логирования
    AsyncLoggingMiddleware* logging = new AsyncLoggingMiddleware();
    logging->setOutput(Serial);  // Вывод в Serial
    logging->setEnabled(true);
    server.addMiddleware(logging);  // Добавляем к серверу

    File htmlFile = LittleFS.open("/pageForInitClock.html", "r");
    if (!htmlFile) {
        Serial.println("was not read file");
        return false;
    }
    String page;
    while (htmlFile.available()) {
        String fileLine = htmlFile.readStringUntil('\n');
        page += fileLine;
    }
    htmlFile.close();
    Serial.println("Listing files in LittleFS:");
    Dir dir = LittleFS.openDir("/");
    Serial.print("Cur path : ");
    Serial.println(dir.fileName());
    while (dir.next()) {
        Serial.print("  ");
        Serial.print(dir.fileName());
        Serial.print("  SIZE: ");
        Serial.println(dir.fileSize());
    }

    Serial.println("Server started. Open http://" + this->deviceIP.toString());
    return true;
}

void alert(String message) {
    Serial.println(message);
}