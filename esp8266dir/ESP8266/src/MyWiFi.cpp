#include "MyWiFi.h"
void onRequest(AsyncWebServerRequest *request);
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
MyClassEsp8266::MyClassEsp8266(String login, String password, String logSTA, String passSTA)
    : _loginAP(login), _passwordAP(password), _loginSTA(logSTA), _passwordSTA(passSTA), deviceIP(192, 168, 4, 1)
{
}

void MyClassEsp8266::setupWiFiApMode()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->_loginAP, this->_passwordAP);
    Serial.println("init AP mode");
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
    if (!_loginAP.length() || !_passwordAP.length() || !_loginSTA.length() || !_passwordSTA.length())
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
    // // if (WiFi.status() != WL_CONNECTED) {
    // //     return false;
    // // }
    // // передаем обработчик как лямбду
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

    server.on("/login", HTTP_POST, onRequest, nullptr, onBody);
    // // server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request)
    // //           {
    // //     Serial.println("Button login was clicked!");

    // //     // Проверяем, есть ли тело запроса
    // //     if (request->hasParam("body", true)) {  // true — для тела
    // //         String body = request->getParam("body", true)->value();
    // //         Serial.println("Body: " + body);

    // //         // Парсим JSON (требуется ArduinoJson)
    // //         DynamicJsonDocument doc(1024);
    // //         DeserializationError error = deserializeJson(doc, body);
    // //         if (error) {
    // //             Serial.println("JSON parse error: " + String(error.c_str()));
    // //             request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    // //             return;
    // //         }

    // //         String username = doc["username"];
    // //         String password = doc["password"];

    // //         // Здесь можно добавить логику: сохранить WiFi-креды, подключиться и т.д.
    // //         Serial.println("Username: " + username);
    // //         Serial.println("Password: " + password);

    // //         // Отправляем ответ
    // //         request->send(200, "application/json", "{\"status\":\"Login data received\"}");
    // //     } else {
    // //         Serial.println("No body");
    // //         request->send(400, "application/json", "{\"error\":\"No body\"}");
    // //     } });

    server.begin();
    // // Создаём middleware для логирования
    // // AsyncLoggingMiddleware *logging = new AsyncLoggingMiddleware();
    // // logging->setOutput(Serial); // Вывод в Serial
    // // logging->setEnabled(true);
    // // server.addMiddleware(logging); // Добавляем к серверу

    // // File htmlFile = LittleFS.open("/pageForInitClock.html", "r");
    // // if (!htmlFile)
    // // {
    // //     Serial.println("was not read file");
    // //     return false;
    // // }
    // // String page;
    // // while (htmlFile.available())
    // // {
    // //     String fileLine = htmlFile.readStringUntil('\n');
    // //     page += fileLine;
    // // }
    // // htmlFile.close();
    // // Serial.println("Listing files in LittleFS:");
    // // Dir dir = LittleFS.openDir("/");
    // // Serial.print("Cur path : ");
    // // Serial.println(dir.fileName());
    // // while (dir.next())
    // // {
    // //     Serial.print("  ");
    // //     Serial.print(dir.fileName());
    // //     Serial.print("  SIZE: ");
    // //     Serial.println(dir.fileSize());
    // // }

    Serial.println("Server started. Open http://" + this->deviceIP.toString());
    return true;
}
/// @brief это основной обработчик, который вызывается после полного получения 
/// и обработки тела (если onBody задан). 
/// Он подходит для финальной логики (например, отправки ответа клиенту).
/// @param request Указатель на запрос.
void onRequest(AsyncWebServerRequest *request)
{
    Serial.println("Input method onRequest");
}
/// @brief Обработчик для загрузки файлов (multipart/form-data). Вызывается асинхронно по мере получения файла.
///        Если загрузка не нужна, передавайте nullptr в server.on().
///        Файл приходит по чанкам (кусочкам), что позволяет обрабатывать большие файлы без загрузки всего в память.
/// @param request Указатель на объект запроса AsyncWebServerRequest (для доступа к заголовкам, параметрам и ответу).
/// @param filename Имя загружаемого файла (из multipart/form-data).
/// @param index Смещение в файле (позиция, с которой начинается текущий чанк; 0 для первого чанка).
/// @param data Указатель на байты текущего чанка данных файла.
/// @param len Длина текущего чанка в байтах.
/// @param final Флаг завершения: true, если это последний чанк файла (можно выполнить финальную обработку, например, сохранить файл).
void onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
}
/// @brief Обработчик тела запроса (request body), вызываемый асинхронно по мере получения данных.
///        Тело приходит по чанкам (кусочкам), что позволяет обрабатывать большие данные без загрузки всего в память сразу.
///        Рекомендуется накапливать тело в буфер и выполнять финальную обработку только на последнем чанке (когда index + len == total).
///        После обработки очистите буфер, чтобы избежать утечек памяти и смешивания данных между запросами.
/// @param request Указатель на объект запроса AsyncWebServerRequest (для доступа к заголовкам, методу и ответу).
/// @param data Указатель на массив байтов текущего чанка тела (не null-terminated, используйте len для границ).
/// @param len Длина текущего чанка в байтах (может быть меньше полного тела).
/// @param index Смещение в общем теле (позиция, с которой начинается текущий чанк; 0 для первого чанка).
/// @param total Общий размер тела в байтах (известен заранее, если Content-Length задан).
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    static String bufferBody; // буфер для накопления тела

    Serial.println("Input method onBody");
    for(size_t i = 0; i < len; i++) {
        bufferBody += (char)data[i];
    }
    
    if(index + len == total) { // проверка не последний ли чанк (конец посылка)
        Serial.println("Recieved body : " + bufferBody);
    }
}
