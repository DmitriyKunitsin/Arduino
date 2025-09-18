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
    static String bufferBody; // буфер для накопления тела

    Serial.println("Input method onBody");
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

const char *EspWebServer::getWifiSSID() const
{
    if (this->wifiSSID == nullptr)
        return nullptr;
    return this->wifiSSID;
}