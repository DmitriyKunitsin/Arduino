/**
 * @file TimeManager.hpp
 * @author Dmitriy Kunitcin (kunicind1996@gmail.com)
 * @version 1.0
 * @date YYYY-MM-DD
 * @brief Класс TimeManager инкапсулирует функциональность библиотеки NTPClient, упрощая доступ к функционалу синхронизации времени и форматов представления дат и времени.
 *
 * Класс TimeManager реализует концепцию обёртки (Wrapper Pattern), обеспечивая удобный интерфейс для работы с библиотекой NTPClient, предоставляющей доступ к службам точного времени (NTP).
 *
 * ### Особенности:
 * - Простота использования: Класс скрывает детали работы с NTP и предоставляет удобные методы для получения времени.
 * - Расширяемость: Возможность расширения и адаптации существующих методов NTPClient.
 * - Инкапсуляция: Внутренняя реализация основана на библиотеке NTPClient, что предотвращает прямое вмешательство в её функционирование.
 *
 * ### Требования:
 * - Библиотека NTPClient и WiFiUdp необходимы для работы класса.
 *
 * ### Пример использования:
 * ```cpp
 * #include <MyWiFi.h>
 * #include <NTPClient.h>
 * #include <WiFiUdp.h>
 *
 * WiFiUDP ntpUDP;
 * TimeManager timeManager(ntpUDP);
 *
 * void setup() {
 *     Serial.begin(115200);
 *     timeManager.begin(); // старт синхронизации времени
 * }
 *
 * void loop() {
 *     timeManager.update(); // обновление времени
 *     Serial.println(timeManager.getFormattedTime()); // вывод времени
 *     delay(1000);
 * }
 * ```
 */

#pragma once

#include <NTPClient.h>
#include <WiFiUdp.h>

/**
 * @brief Класс TimeManager предоставляет удобную обёртку для синхронизации времени через NTP сервисы.
 *
 * Время синхронизируется через стандартные NTP-серверы, предоставляемые сервисом NTPClient.
 * Класс реализует паттерн Wrapper Pattern, инкапсулируя основные методы для удобной работы с синхронизацией времени.
 */
class TimeManager
{
public:
    /**
     * @brief Конструктор класса TimeManager.
     *
     * @param udpInstance Экземпляр объекта WiFiUDP, используемый для работы с NTP.
     * @param poolServerName Адрес NTP-сервера (по умолчанию: "pool.ntp.org").
     * @param gmtOffsetSecc Часовой сдвиг в секундах (GMT offset). 14400 == +4 от мск
     * @param daylightOffsetSec Летнее смещение времени (день света).
     */
    TimeManager(WiFiUDP &udpInstance, const char *poolServerName = "pool.ntp.org", long gmtOffsetSecc = 14400, int daylightOffsetSec = 0)
        : _ntpClient(udpInstance, poolServerName, gmtOffsetSecc, daylightOffsetSec) {}

    /**
     * @brief Запускает процесс синхронизации времени с NTP-сервером.
     */
    void begin();
    /**
     * @brief Обновляет текущее время, полученное с NTP-сервера.
     */
    void update();
    /**
     * @brief Возвращает полное форматированное время в формате "HH:mm:ss".
     *
     * @return Форматированное время в виде строки.
     */
    String getFormattedTime();
    /**
     * @brief Возвращает сокращённую версию времени в формате "HH:mm".
     *
     * @return Сокращённая версия времени в виде строки.
     */
    String GetFormattedTimeForDisplay();

private:
    /**
     * @brief Внутренний экземпляр класса NTPClient, используемый для работы с NTP.
     */
    NTPClient _ntpClient;
};