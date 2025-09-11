#ifndef CLOCKDISPLAY_H
#define CLOCKDISPLAY_H

#include <Arduino.h>

class ClockDisplay {
public:
    ClockDisplay();

    void setup();
    void loop();

private:
    // Пины
    static const uint8_t LATCH_PIN = 7;
    static const uint8_t CLOCK_PIN = 8;
    static const uint8_t DATA_PIN = 4;

    static const uint8_t PIN_GROUND_ONE_DIGIT = A1;
    static const uint8_t PIN_GROUND_SECOND_DIGIT = A2;
    static const uint8_t PIN_GROUND_THREE_DIGIT = A3;
    static const uint8_t PIN_GROUND_FOUR_DIGIT = A4;

    static const uint8_t PIN_BUTTON_SETTING_HOUR = 9;
    static const uint8_t PIN_BUTTON_SETTING_MIN = 6;
    static const uint8_t BTN_PIN_INTERRUPT = 3;

    // Константы
    static const unsigned long INTERVAL = 1000;
    static const unsigned int MINUTE_FOR_CLOCK = 6;

    // Переменные
    volatile int status; // 0 - Work, 1 - Setting
    bool firstStart;
    bool isChangedSegment;

    unsigned long prevMillies;
    unsigned long currentMillies;

    int seconds;

    unsigned int minuts;
    unsigned int hour;

    byte val[2];
    int seg[2];
    int timeArr[4];

    int grounds[4];

    const byte digitals[11] = {
        0b1111110,  // 0
        0b0110000,  // 1
        0b1101101,  // 2
        0b1111001,  // 3
        0b0110011,  // 4
        0b1011011,  // 5
        0b1011111,  // 6
        0b1110000,  // 7
        0b1111111,  // 8
        0b1111011,  // 9
        0b1001111   // E (error)
    };

    // Прерывания и переменные для антидребезга
    volatile unsigned long lastPressInterruptBtn;
    volatile bool IsCheckedBtnSetting;

    // Методы
    bool IsMillies(unsigned long interval);
    void offAllSegmentsDisplaing();
    void blinkSettingDigits(int segments[], int countSegments, byte digitPrintDisplay[]);
    void printSpecificSegment(int digitSegment, byte valuePrint);
    void displaingValues(int values[], int delayTime);
    void writeDigit(int digitSelectedSegments, byte printDigit);
    void connectToGround(int digitGround);
    void disconnectToGround(int digitGround);
    void printDisplay(byte printNumber);
    void InitializatedDisplay();
    byte getHighByte(unsigned int val);
    byte getLowByte(unsigned int val);
    void incrementHour(unsigned int &hour);
    void incrementMinutes(unsigned int &min);
    void updateClock();
    void updateTime();
    void localInterrapt();
    void checkDurationBtnSetting();
    bool changeSetment(bool lvlSegment);
    void Settings();
    void DoWork();

    // Статический обработчик прерывания
    static void ISR_Handler();

    // Для доступа к объекту из ISR
    static ClockDisplay* instance;
};

#endif // CLOCKDISPLAY_H
