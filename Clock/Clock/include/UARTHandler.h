#ifndef UARTHANDLER_H
#define UARTHANDLER_H

#include <Arduino.h>

#define LEN_CIRCLE_BUFFER 255

/**
 * @class UARTHandler
 * @brief Класс для работы с UART с буферизацией и распознаванием пакетов по символам начала и конца.
 *
 * Пример использования:
 * @code
 * #include "UARTHandler.h"
 *
 * UARTHandler uartHandler('s', 'e'); // символы начала и конца пакета
 *
 * void setup() {
 *     uartHandler.begin(103); // 9600 baud при 16 MHz
 *     sei(); // включаем глобальные прерывания
 * }
 *
 * void loop() {
 *     if (uartHandler.isPackageReady()) {
 *         const unsigned char* data = uartHandler.getBuffer();
 *         uint8_t len = uartHandler.getPackageLength();
 *         // Обработка пакета data длиной len
 *
 *         uartHandler.resetPackageReady();
 *     }
 * }
 *
 * ISR(USART_RX_vect) {
 *     uartHandler.handleISR();
 * }
 * @endcode
 */
class UARTHandler {
   public:
    /// @brief Конструктор по умолчанию, символ начала послыки  : 's' , символ конца посылки : 'e'
    UARTHandler();
    /// @brief Конструктор для возможности вручную указать символы начала и конца посылки
    /// @param symbolParcelStart
    /// @param symbolParcelEnd
    UARTHandler(unsigned char symbolParcelStart, unsigned char symbolParcelEnd);
    /**
     * @brief Инициализация USART с заданным значением UBRR.
     *
     * Настраивает скорость передачи, формат кадра и включает приёмник, передатчик и прерывание по приёму.
     *
     * @param ubrr Значение регистра UBRR для установки скорости передачи.
     *
     * @note Значения ubrr для частоты 16 MHz и стандартных скоростей:
     * - 103  — 9600 baud
     * - 207  — 4800 baud
     * - 51   — 19200 baud
     * - 25   — 38400 baud
     * - 12   — 76800 baud (если поддерживается)
     * - 8    — 115200 baud (может быть нестабильным на 16 MHz)
     *
     * Пример:
     * @code
     * UARTHandler uartHandler;
     * uartHandler.begin(103); // 9600 baud при 16 MHz
     * sei(); // включить прерывания
     * @endcode
     *
     * @warning Значения рассчитаны для асинхронного режима USART, без двойной скорости (U2X0=0).
     * Для использования двойной скорости (U2X0=1) значения будут вдвое меньше.
     *
     * @see datasheet AVR для точных формул расчёта UBRR:
     * UBRR = (F_CPU / (16 * baud)) - 1
     */
    void begin(unsigned char ubrr);

    /**
     * @brief Получить принятый байт (блокирующий вызов).
     * @return Принятый байт.
     */
    unsigned char receive();

    /**
     * @brief Отправить байт (блокирующий вызов).
     * @param data Байты для передачи.
     */
    void transmit(unsigned char data);

    /**
     * @brief Проверить, готов ли пакет к обработке.
     * @return true, если пакет готов.
     */
    bool isPackageReady();

    /**
     * @brief Получить указатель на начало пакета в буфере.
     * @return Указатель на буфер с принятыми данными.
     */
    const unsigned char* getBuffer() const;

    /**
     * @brief Получить длину текущего пакета.
     * @return Длина пакета.
     */
    uint8_t getPackageLength() const;

    /**
     * @brief Сбросить флаг готовности пакета после обработки.
     */
    void resetPackageReady();

    /**
     * @brief Обработчик прерывания по приему байта. Вызывать из ISR.
     */
    void handleISR();

   private:
    unsigned char buffer[LEN_CIRCLE_BUFFER];
    /// @brief индекс для записи нового байта (tail)
    volatile uint8_t tailPackage;
    /// @brief индекс для чтения данных (head)
    volatile uint8_t headPackage;
    /// @brief индекс начала текущего пакета (позиция после символа 'r')
    uint8_t packageStart;
    /// @brief длина текущего пакета
    uint8_t lenPackage;
    /// @brief флаг готовности пакета к обработке
    volatile bool packageIsReady;
    /// @brief Символ обозначающий начало посылки
    const unsigned char symbolParcelStart;
    /// @brief Символ обозначающий конец посылки
    const unsigned char symbolParcelEnd;
};

extern UARTHandler uartHandler;

#endif  // UARTHANDLER_H
