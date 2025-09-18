#include <EspWebServer.h>
void EspWebServer::onRequest(AsyncWebServerRequest *request)
{
    Serial.println("Input method onRequest");
}
void EspWebServer::onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
}
void EspWebServer::onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    String url = request->url();
    if (url == "/login")
    {
        handleLoginBody(request, data, len, index, total);
    }
    else if (url == "/set-time")
    {
        handleSetTimeBody(request, data, len, index, total);
    }
    else
    {
        request->send(404, "text\plain", "Unknown route");
    }
}
void EspWebServer::handleLoginBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    static String bufferBody; // буфер для накопления тела
    for (size_t i = 0; i < len; i++)
    {
        bufferBody += (char)data[i];
    }

    if (index + len == total)
    { // проверка не последний ли чанк (конец посылка)
        Serial.println("Recieved body : " + bufferBody);

        JsonDocument fullAnswer, password, login;
        DeserializationError error = deserializeJson(fullAnswer, bufferBody);
        if (error)
        {
            Serial.println("deserializeJson error : ");
            Serial.println(error.f_str());
            return;
        }

        this->wifiSSID = fullAnswer["username"];
        this->wifiPassword = fullAnswer["password"];

        Serial.println("Login : " + String(this->wifiSSID));
        Serial.println("Passwod : " + String(this->wifiPassword));

        bufferBody = "";
    }
}
void EspWebServer::handleSetTimeBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    static String bufferBody; // буфер для накопления тела
    for (size_t i = 0; i < len; i++)
    {
        bufferBody += (char)data[i];
    }

    if (index + len == total)
    { // проверка не последний ли чанк (конец посылка)
        Serial.println("Recieved body : " + bufferBody);

        JsonDocument fullAnswer, password, login;
        DeserializationError error = deserializeJson(fullAnswer, bufferBody);
        if (error)
        {
            Serial.println("deserializeJson error : ");
            Serial.println(error.f_str());
            return;
        }

        this->setHour = fullAnswer["hours"];
        this->setMinute = fullAnswer["minutes"];

        Serial.println("Hour : " + String(this->setHour));
        Serial.println("Minutes : " + String(this->setMinute));

        bufferBody = "";
    }
}
const char *EspWebServer::getWifiSSID() const
{
    if (this->wifiSSID == nullptr)
        return nullptr;
    return this->wifiSSID;
}