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
        this->isConnWifi = true;
        // yield();
        // if (this->tryConnectedToSTA())
        // {
        //     Serial.println("Succerful connected to Wifi : " + String(this->wifiSSID));
        // }
        // else
        // {
        //     Serial.println("Aborted connected to Wifi : " + String(this->wifiSSID));
        // }
        // yield();
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
const char *EspWebServer::getWifiPassword() const
{
    if (this->wifiPassword == nullptr)
        return nullptr;
    return this->wifiPassword;
}
bool EspWebServer::tryConnectedToSTA()
{
    if (this->wifiSSID == nullptr && this->wifiPassword == nullptr)
    {
        return false;
    }
    WiFi.mode(WIFI_AP_STA);
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    WiFi.softAP(this->login_AP_MODE, this->passwod_AP_MODE);
    Serial.println("Conntecting to WiFi : " + String(this->wifiSSID));
    WiFi.disconnect();
    WiFi.begin(this->wifiSSID, this->wifiPassword);
    byte countTry = 0; /// колличество попыток подключения
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Try connecting : " + String(countTry) + " waiting 500 ms...");
        if (countTry >= 0x0A) // 10
        {
            this->isConnWifi = false;
            break;
            return false;
        }
        countTry++;
    }
    this->isConnWifi = false;
    return (WiFi.status() == WL_CONNECTED);
}
void EspWebServer::setupWiFiSTAmode(const char *const SSID, const char *const PASSWORD)
{
    if (SSID == nullptr || PASSWORD == nullptr)
    {
        Serial.println("NOT initialization STA mode");
        return;
    }
    WiFi.mode(WIFI_STA);
    this->wifiSSID = SSID;
    this->wifiPassword = PASSWORD;
    WiFi.begin(this->wifiSSID, this->wifiPassword);
    Serial.println("initialization STA mode");
}
void EspWebServer::setupWiFiApMode(const char *const loginAP, const char *const paswwodAP)
{
    if (loginAP == nullptr || paswwodAP == nullptr)
    {
        Serial.println("NOT initialization Acess Point mode");
        return;
    }
    WiFi.mode(WIFI_AP);
    this->login_AP_MODE = loginAP;
    this->passwod_AP_MODE = paswwodAP;
    WiFi.softAP(this->login_AP_MODE, this->passwod_AP_MODE);
    Serial.println("initialization Acess Point mode");
}

int EspWebServer::GetAndCheckSumAccessPoints()
{
    int countWiFiAccessPoints = WiFi.scanNetworks();
    if (countWiFiAccessPoints == 0)
    {
        Serial.println("Сети не найдены");
    }
    else
    {
        Serial.println(countWiFiAccessPoints);
        Serial.println("  сетей найдено");
        for (int i = 0; i < countWiFiAccessPoints; i++)
        {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print("\t");
            Serial.print(WiFi.RSSI(i));
            Serial.println("  dBm");
        }
    }
    return countWiFiAccessPoints;
}