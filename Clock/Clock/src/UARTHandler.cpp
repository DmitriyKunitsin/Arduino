#include "UARTHandler.h"

#include <avr/interrupt.h>

UARTHandler::UARTHandler()
    : tailPackage(0), headPackage(0), packageStart(0), lenPackage(0), packageIsReady(false), symbolParcelStart('s'), symbolParcelEnd('e') {
}
UARTHandler::UARTHandler(unsigned char symbolParcelStart, unsigned char symbolParcelEnd) : tailPackage(0),
                                                                                           headPackage(0),
                                                                                           packageStart(0),
                                                                                           lenPackage(0),
                                                                                           packageIsReady(false),
                                                                                           symbolParcelStart(symbolParcelStart),
                                                                                           symbolParcelEnd(symbolParcelEnd) {
}
void UARTHandler::begin(unsigned char ubrr) {
    /* Установка baudrate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    /* Включение приёмника, передатчика и прерывания по приёму */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0);

    /* Формат кадра: 8 бит данных,1 стоп бита */
    /**
    *  USBS0 = 0	1 стоп-бит
    *  USBS0 = 1	2 стоп-бита
    *  UCSZ01:0 = 00	5 бит данных
    *  UCSZ01:0 = 01	6 бит данных
    *  UCSZ01:0 = 10	7 бит данных
    *  UCSZ01:0 = 11	8 бит данных
     */
    UCSR0C = (0 << USBS0) | (3 << UCSZ00);
}

unsigned char UARTHandler::receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void UARTHandler::transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // ждём, пока регистр пуст
    UDR0 = data;
}

bool UARTHandler::isPackageReady() {
    return packageIsReady;
}

const unsigned char* UARTHandler::getBuffer() const {
    return buffer;
}

uint8_t UARTHandler::getPackageLength() const {
    return lenPackage;
}

void UARTHandler::resetPackageReady() {
    packageIsReady = false;
    // После обработки пакета сдвигаем headPackage, освобождая буфер
    headPackage = (packageStart + lenPackage) % LEN_CIRCLE_BUFFER;
}

void UARTHandler::handleISR() {
    unsigned char receivedByte = UDR0;
    uint8_t nextTail = (tailPackage + 1) % LEN_CIRCLE_BUFFER;

    // Проверка переполнения буфера
    if (nextTail == headPackage) {
        // Буфер переполнен — игнорируем байт
        return;
    }

    buffer[tailPackage] = receivedByte;
    tailPackage = nextTail;

    if (receivedByte == symbolParcelStart) {
        packageStart = tailPackage;  // начало пакета — следующий индекс после 'r'
        lenPackage = 0;
        packageIsReady = false;
    } else if (receivedByte == symbolParcelEnd) {
        if (tailPackage >= packageStart) {
            lenPackage = tailPackage - packageStart;
        } else {
            lenPackage = LEN_CIRCLE_BUFFER - packageStart + tailPackage;
        }
        packageIsReady = true;
    } else {
        if (!packageIsReady) {
            lenPackage++;
        }
    }
}
bool state = false;
// // Глобальная функция для ISR, делегирующая обработку в класс
/**
 * USART_RX_vect прерывание при приёме данных (Receive Complete)
 * Срабатывает, когда в регистре приёма (UDR0) появляется новый принятый байт.
 */
ISR(USART_RX_vect) {
    uartHandler.handleISR();
    digitalWrite(LED_BUILTIN, state);
    state = !state;
}

/**
 * USART_UDRE_vect прерывание при пустом регистре передачи (Data Register Empty)
 * Срабатывает, когда регистр передачи (UDR0) готов принять новый байт (то есть пуст).
 */
ISR(USART_UDRE_vect) {
}
/**
 * USART_TX_vect прерывание по окончании передачи (Transmit Complete)
 * Срабатывает, когда передача текущего байта полностью завершена (включая сдвиг всех бит и стоп-бит)
 */
ISR(USART_TX_vect) {
}