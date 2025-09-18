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

        const char* pass = fullAnswer["password"];
        const char* log = fullAnswer["username"];

        Serial.println("Passwod : " + String(pass));
        Serial.println("Login : " + String(log));

        bufferBody = "";
    }
}
