#include "UARTHandler.h"

#include <avr/interrupt.h>

UARTHandler::UARTHandler()
    : tailPackage(0), headPackage(0), packageStart(0), lenPackage(0), packageIsReady(false), symbolParcelStart('s'), symbolParcelEnd('e'), txHead(0), txTail(0) {
}
UARTHandler::UARTHandler(unsigned char symbolParcelStart, unsigned char symbolParcelEnd) : tailPackage(0),
                                                                                           headPackage(0),
                                                                                           packageStart(0),
                                                                                           lenPackage(0),
                                                                                           packageIsReady(false),
                                                                                           symbolParcelStart(symbolParcelStart),
                                                                                           symbolParcelEnd(symbolParcelEnd),
                                                                                           txHead(0), 
                                                                                           txTail(0) {
}
void UARTHandler::begin(unsigned char ubrr) {
    /* Установка baudrate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    /* Включение приёмника, передатчика и прерывания по приёму */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

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
void UARTHandler::asyncTransmit(unsigned char data) {
    uint8_t nextHead = (this->txHead + 1) % TX_BUFFER_SIZE;

    if (nextHead == this->txTail) {
        // Буфер полон — игнорируем
        return;
    }
    this->txBuffer[txHead] = data;
    this->txHead = nextHead;

    enableUDRIE();
}
size_t strlen(const unsigned char* const str) {
    const unsigned char* s = str;
    while (*s != '\0') {
        s++;
    }
    return s - str;
}
void UARTHandler::AsyncStringTransmit(const unsigned char* const data) {
    unsigned int len = strlen(data);
    for (unsigned int i = 0; i < len; i++) {
        asyncTransmit(data[i]);
    }
}
bool UARTHandler::isPackageReady() {
    return this->packageIsReady;
}

const unsigned char* UARTHandler::getBuffer() const {
    return this->buffer;
}
const unsigned char* UARTHandler::getFilledBuffer() const{
    if (!packageIsReady) return nullptr;
    return &this->buffer[this->packageStart];
}
void UARTHandler::clearBuffer() 
{
    packageIsReady = false;
    packageStart = 0;
    lenPackage = 0;
}
uint8_t UARTHandler::getPackageLength() const {
    return this->lenPackage;
}

void UARTHandler::resetPackageReady() {
    packageIsReady = false;
    // Освобождаем буфер: сдвигаем headPackage к концу пакета
    this->headPackage = (this->packageStart + this->lenPackage) % LEN_CIRCLE_BUFFER;
    this->lenPackage = 0;
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
    uint8_t currentIndex = tailPackage + 1;  // индекс текущего байта, но его не учитываем, т.к. это протокол
    tailPackage = nextTail;

    if (receivedByte == symbolParcelStart) {
        packageStart = currentIndex;  // начало пакета — следующий индекс
        lenPackage = 1;
        packageIsReady = false;
    } else if (receivedByte == symbolParcelEnd) {
        if (packageStart != 0 || lenPackage > 0) {  // Проверяем, что пакет начат
            packageIsReady = true;
        }
    } else {
        if (!packageIsReady && lenPackage > 0) {
            lenPackage++;  // Увеличиваем только если пакет начат
        }
    }
}

void UARTHandler::enableUDRIE() {
    UCSR0B |= (1 << UDRIE0);
}
void UARTHandler::disableUDRIE() {
    UCSR0B &= ~(1 << UDRIE0);
}

bool UARTHandler::IsTxHeadEqualsTail() {
    return this->txHead == this->txTail;
}
uint8_t UARTHandler::GetValueTxBuffer(uint8_t txTail) {
    uint8_t value = this->txBuffer[this->txTail];
    this->txTail = (this->txTail + 1) % TX_BUFFER_SIZE;
    return value;
}
uint8_t UARTHandler::GetTxTail() {
    return this->txTail;
}
bool state = false;
// // Глобальная функция для ISR, делегирующая обработку в класс
/**
 * USART_RX_vect прерывание при приёме данных (Receive Complete)
 * Срабатывает, когда в регистре приёма (UDR0) появляется новый принятый байт.
 */
ISR(USART_RX_vect) {
    uartHandler.handleISR();
    // unsigned char temp = UDR0;
    // uartHandler.asyncTransmit(temp);
    digitalWrite(LED_BUILTIN, state);
    state = !state;
}

/**
 * USART_UDRE_vect прерывание при пустом регистре передачи (Data Register Empty)
 * Срабатывает, когда регистр передачи (UDR0) готов принять новый байт (то есть пуст).
 */
ISR(USART_UDRE_vect) {
    if (uartHandler.IsTxHeadEqualsTail()) {
        uartHandler.disableUDRIE();
    } else {
        UDR0 = uartHandler.GetValueTxBuffer(uartHandler.GetTxTail());
    }
}
/**
 * USART_TX_vect прерывание по окончании передачи (Transmit Complete)
 * Срабатывает, когда передача текущего байта полностью завершена (включая сдвиг всех бит и стоп-бит)
 */
ISR(USART_TX_vect) {
}