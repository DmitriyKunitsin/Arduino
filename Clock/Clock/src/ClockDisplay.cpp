#include "ClockDisplay.h"

ClockDisplay *ClockDisplay::instance = nullptr;

ClockDisplay::ClockDisplay()
    : status(0), firstStart(true), isChangedSegment(true), prevMillies(0), currentMillies(0), seconds(0), minuts(0x0500), hour(0x0100), lastPressInterruptBtn(0), IsCheckedBtnSetting(false) {
    grounds[0] = PIN_GROUND_ONE_DIGIT;
    grounds[1] = PIN_GROUND_SECOND_DIGIT;
    grounds[2] = PIN_GROUND_THREE_DIGIT;
    grounds[3] = PIN_GROUND_FOUR_DIGIT;

    instance = this;
}

bool ClockDisplay::IsMillies(unsigned long interval) {
    currentMillies = millis();
    return (currentMillies - prevMillies >= interval);
}

void ClockDisplay::offAllSegmentsDisplaing() {
    for (int i = 0; i < 4; i++) {
        disconnectToGround(i);
    }
}

void ClockDisplay::blinkSettingDigits(int segments[], int countSegments, byte digitPrintDisplay[]) {
    for (int i = 0; i < countSegments; i++) {
        printSpecificSegment(segments[i], digitPrintDisplay[i]);
    }
}

void ClockDisplay::printSpecificSegment(int digitSegment, byte valuePrint) {
    connectToGround(digitSegment);
    printDisplay(valuePrint);
    delay(250);
    disconnectToGround(digitSegment);
}

void ClockDisplay::displaingValues(int values[], int delayTime) {
    for (int indexDigit = 0; indexDigit < 4; indexDigit++) {
        writeDigit(indexDigit, values[indexDigit]);
        delay(delayTime);
        disconnectToGround(indexDigit);
    }
}

void ClockDisplay::writeDigit(int digitSelectedSegments, byte printDigit) {
    printDisplay(printDigit);
    connectToGround(digitSelectedSegments);
}

void ClockDisplay::connectToGround(int digitGround) {
    digitalWrite(grounds[digitGround], HIGH);
}

void ClockDisplay::disconnectToGround(int digitGround) {
    digitalWrite(grounds[digitGround], LOW);
}

void ClockDisplay::printDisplay(byte printNumber) {
    byte templatedIndex;
    if (printNumber >= 10) {
        templatedIndex = digitals[10];
    } else {
        templatedIndex = digitals[printNumber];
    }
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(DATA_PIN, LOW);
    for (int index = 0; index < 8; index++) {
        digitalWrite(CLOCK_PIN, LOW);
        if ((templatedIndex >> index) & 1) {
            digitalWrite(DATA_PIN, HIGH);
        } else {
            digitalWrite(DATA_PIN, LOW);
        }
        digitalWrite(CLOCK_PIN, HIGH);
    }
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(CLOCK_PIN, HIGH);
}

void ClockDisplay::InitializatedDisplay() {
    int error[4] = {11, 11, 11, 11};
    displaingValues(error, 250);
    delay(250);
    displaingValues(error, 250);
    delay(250);
    displaingValues(error, 250);
    firstStart = false;
}

byte ClockDisplay::getHighByte(unsigned int val) {
    return (val & 0xFF);
}

byte ClockDisplay::getLowByte(unsigned int val) {
    return (val >> 8) & 0xFF;
}

void ClockDisplay::incrementHour(unsigned int &hour) {
    byte leftDigital = getLowByte(hour);
    byte rightDigital = getHighByte(hour);
    rightDigital++;
    if (leftDigital >= 2) {
        if (rightDigital > 3) {
            leftDigital = 0;
            rightDigital = 0;
            hour = (leftDigital << 8) | rightDigital;
            return;
        }
    }
    if (rightDigital > 9) {
        rightDigital = 0;
        leftDigital++;
    }
    hour = (leftDigital << 8) | rightDigital;
}

void ClockDisplay::incrementMinutes(unsigned int &minuts) {
    byte leftDigital = getLowByte(minuts);
    byte rightDigital = getHighByte(minuts);

    rightDigital++;
    if (rightDigital >= 9) {
        rightDigital = 0;
        leftDigital++;
        if (leftDigital > 5) {
            leftDigital = 0;
            if (status == 0) {  // Work
                incrementHour(hour);
            }
        }
    }
    minuts = (leftDigital << 8) | rightDigital;
}

void ClockDisplay::updateClock() {
    if (IsMillies(INTERVAL)) {
        prevMillies = currentMillies;
        if (seconds >= MINUTE_FOR_CLOCK) {
            seconds = 0;
            incrementMinutes(minuts);
        }
        seconds++;
    }
}
void ClockDisplay::externalUpdateTimes(unsigned char byteTime[4]) {
    // Валидация диапазонов
    if (byteTime[0] > 2 || byteTime[1] > 9 || byteTime[2] > 5 || byteTime[3] > 9) {
        return;
    }

    unsigned char newHour = byteTime[0] * 10 + byteTime[1];  // Для проверки: 0–23
    unsigned char newMinute = byteTime[2] * 10 + byteTime[3];  // 0–59
    // Проверка перед присвоением
    if (newHour > 23 || newMinute > 59)
    {
        return; 
    } 

    // Обновление внутренних переменных (в формате: high byte = десятки, low byte = единицы)
    hour = (byteTime[0] << 8) | byteTime[1];       // hourTens в high, hourUnits в low
    minuts = (byteTime[2] << 8) | byteTime[3];     // minTens в high, minUnits в low

    // Сброс seconds для синхронизации (чтобы не было внезапного инкремента)
    seconds = 0;

    // Обновление timeArr для отображения
    updateTime();
}
void ClockDisplay::updateTime() {
    timeArr[0] = getLowByte(hour);
    timeArr[1] = getHighByte(hour);
    timeArr[2] = getLowByte(minuts);
    timeArr[3] = getHighByte(minuts);
}

void ClockDisplay::localInterrapt() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressInterruptBtn > 300) {
        IsCheckedBtnSetting = true;
        lastPressInterruptBtn = currentTime;
    }
}

void ClockDisplay::checkDurationBtnSetting() {
    unsigned long pressDuration = millis() - lastPressInterruptBtn;

    if (pressDuration >= 10000 && digitalRead(BTN_PIN_INTERRUPT) == HIGH) {
        IsCheckedBtnSetting = false;
        status = 0;  // Work
    } else if (status != 1 && pressDuration >= 1500 && digitalRead(BTN_PIN_INTERRUPT) == LOW) {
        status = 1;  // Setting
    } else if (status == 1 && digitalRead(BTN_PIN_INTERRUPT) == LOW) {
        changeSetment(isChangedSegment);
        isChangedSegment = !isChangedSegment;
    }
}

bool ClockDisplay::changeSetment(bool lvlSegment) {
    if (lvlSegment) {
        val[0] = timeArr[0];
        val[1] = timeArr[1];
        seg[0] = 0;
        seg[1] = 1;
        return true;
    } else {
        val[0] = timeArr[2];
        val[1] = timeArr[3];
        seg[0] = 2;
        seg[1] = 3;
        return false;
    }
}

void ClockDisplay::Settings() {
    if (changeSetment(isChangedSegment)) {
        if (digitalRead(PIN_BUTTON_SETTING_HOUR) == LOW) {
            incrementHour(hour);
            updateTime();
            lastPressInterruptBtn = millis();
        }
    } else {
        if (digitalRead(PIN_BUTTON_SETTING_MIN) == LOW) {
            incrementMinutes(minuts);
            updateTime();
            lastPressInterruptBtn = millis();
        }
    }
    blinkSettingDigits(seg, 2, val);
    delay(25);
}

void ClockDisplay::DoWork() {
    updateClock();
    updateTime();
    displaingValues(timeArr, 5);
}

void ClockDisplay::setup() {
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    pinMode(PIN_GROUND_ONE_DIGIT, OUTPUT);
    pinMode(PIN_GROUND_SECOND_DIGIT, OUTPUT);
    pinMode(PIN_GROUND_THREE_DIGIT, OUTPUT);
    pinMode(PIN_GROUND_FOUR_DIGIT, OUTPUT);

    pinMode(BTN_PIN_INTERRUPT, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SETTING_HOUR, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SETTING_MIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BTN_PIN_INTERRUPT), ISR_Handler, FALLING);
}

void ClockDisplay::loop() {
    if (firstStart) {
        InitializatedDisplay();
    }

    if (IsCheckedBtnSetting) {
        checkDurationBtnSetting();
    }

    if (status == 0) {
        DoWork();
    } else if (status == 1) {
        Settings();
    }
}

// Статический обработчик прерывания
void ClockDisplay::ISR_Handler() {
    if (instance != nullptr) {
        instance->localInterrapt();
    }
}
