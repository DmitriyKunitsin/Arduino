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
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    /* Формат кадра: 8 бит данных, 2 стоп бита */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

unsigned char UARTHandler::receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void UARTHandler::transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
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

// // Глобальная функция для ISR, делегирующая обработку в класс
ISR(USART_RX_vect) {
    uartHandler.handleISR();
}
