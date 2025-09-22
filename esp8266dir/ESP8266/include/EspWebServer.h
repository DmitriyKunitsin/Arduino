#ifndef ESPWEBSERVER_H
#define ESPWEBSERVER_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
/**
 * @brief Класс для асинхронного веб-сервера на ESP32.
 * Наследует от AsyncWebServer и добавляет кастомные обработчики.
 */
class EspWebServer : public AsyncWebServer
{
public:
    /**
     * @brief Конструктор по умолчанию, инициализирует сервер на порту 80.
     */
    EspWebServer(): AsyncWebServer(80)
    {
        wifiSSID = nullptr;
        wifiPassword = nullptr;
        login_AP_MODE = nullptr;
        passwod_AP_MODE = nullptr;
        isConnWifi = false;
    }
    /// @brief 
    void setupWiFiSTAmode(const char *const loginAP, const char *const paswwodAP);
    /// @brief метод поднимает точку с WiFi, доступен будет по 192.168.4.1
    /// @param loginAP логин Acces poin точки (имя по которому искать)
    /// @param passwodAP пароль Acces poin точки (пароль который вводить)
    void setupWiFiApMode(const char* const loginAP, const char* const paswwodAP);
    /**
     * @brief это основной обработчик, который вызывается после полного получения
     * и обработки тела (если onBody задан).
     * Он подходит для финальной логики (например, отправки ответа клиенту).
     * @param request Указатель на запрос.
     */
    void onRequest(AsyncWebServerRequest *request);
    /** @brief Обработчик для загрузки файлов (multipart/form-data). Вызывается асинхронно по мере получения файла.
     *        Если загрузка не нужна, передавайте nullptr в server.on().
     *        Файл приходит по чанкам (кусочкам), что позволяет обрабатывать большие файлы без загрузки всего в память.
     * @param request Указатель на объект запроса AsyncWebServerRequest (для доступа к заголовкам, параметрам и ответу).
     * @param filename Имя загружаемого файла (из multipart/form-data).
     * @param index Смещение в файле (позиция, с которой начинается текущий чанк; 0 для первого чанка).
     * @param data Указатель на байты текущего чанка данных файла.
     * @param len Длина текущего чанка в байтах.
     *@param final Флаг завершения: true, если это последний чанк файла (можно выполнить финальную обработку, например, сохранить файл).
     */
    void onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
    /** @brief Обработчик тела запроса (request body), вызываемый асинхронно по мере получения данных.
     *        Тело приходит по чанкам (кусочкам), что позволяет обрабатывать большие данные без загрузки всего в память сразу.
     *        Рекомендуется накапливать тело в буфер и выполнять финальную обработку только на последнем чанке (когда index + len == total).
     *        После обработки очистите буфер, чтобы избежать утечек памяти и смешивания данных между запросами.
     * @param request Указатель на объект запроса AsyncWebServerRequest (для доступа к заголовкам, методу и ответу).
     * @param data Указатель на массив байтов текущего чанка тела (не null-terminated, используйте len для границ).
     * @param len Длина текущего чанка в байтах (может быть меньше полного тела).
     * @param index Смещение в общем теле (позиция, с которой начинается текущий чанк; 0 для первого чанка).
     * @param total Общий размер тела в байтах (известен заранее, если Content-Length задан).
     */
    void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    /**
     * @brief Обрабатывает тело HTTP-запроса для маршрута "/login".
     *
     * Метод вызывается при получении каждого чанка тела POST-запроса.
     * Накопляет данные в буфер и при получении всего тела выполняет парсинг JSON,
     * извлекает необходимые поля (например, "ssid" и "password") и выполняет соответствующую логику.
     * @param request Указатель на объект запроса AsyncWebServerRequest, предоставляет доступ к заголовкам, параметрам и возможность отправить ответ.
     * @param data Указатель на массив байтов текущего чанка тела запроса.
     *             Данные не null-terminated, длина определяется параметром len.
     * @param len Длина текущего чанка в байтах.
     * @param index Смещение текущего чанка в общем теле запроса (0 для первого чанка).
     * @param total Общий размер тела запроса в байтах (если известен, например, из заголовка Content-Length).
     */
    void handleLoginBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    /**
     * @brief Обрабатывает тело HTTP-запроса для маршрута "/set-time".
     * Аналогично handleLoginBody, метод вызывается при получении каждого чанка тела POST-запроса.
     * Накопляет тело, при полном получении парсит JSON и извлекает поле "time",
     * после чего выполняет необходимую обработку (например, обновление времени устройства).
     * @param request Указатель на объект запроса AsyncWebServerRequest, предоставляет доступ к заголовкам, параметрам и возможность отправить ответ.
     * @param data Указатель на массив байтов текущего чанка тела запроса.
     *             Данные не null-terminated, длина определяется параметром len.
     * @param len Длина текущего чанка в байтах.
     * @param index Смещение текущего чанка в общем теле запроса (0 для первого
     * @param total Общий размер тела в байтах (известен заранее, если Content-Length задан).
     */
    void handleSetTimeBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    /// @brief Получить SSID Wi-Fi сети.
    /// Метод возвращает текущий SSID, установленный для подключения к Wi-Fi.
    /// @return Указатель на строку с SSID (const char*), или nullptr, если SSID не установлена.
    /// @note Метод является константным и не изменяет состояние объекта.
    /// @note Возвращаемая строка является неизменяемой (const), чтобы предотвратить случайные изменения.
    const char *getWifiSSID() const;
    const char *getWifiPassword() const;
    /// @brief Пробует подключиться к указанному WiFi
    /// @return true в случае успеха, иначе false
    bool tryConnectedToSTA();

    bool isConnWifi;
private:
    /// @brief SSID WiFi сети, к которой должен подключиться сервер
    const char *wifiSSID;
    /// @brief Пароль от WiFi сети, к которой должен подключиться сервер
    const char *wifiPassword;
    /// @brief Полученный час с клиента
    const char *setHour;
    /// @brief Полученные минуты с клиента
    const char *setMinute;
    /// @brief Логин режима Acess point
    const char *login_AP_MODE;
    /// @brief пароль режима Acess point
    const char *passwod_AP_MODE;
};
#endif // ESPWEBSERVER_H
