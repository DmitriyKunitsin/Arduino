/// пин зашелка, подключается на плате 74HC595 к ST_CP (12 ножка)
const int latchPin = 7;
// пин тактирования , подключается на плате 74HC595 к SH_CP (11 ножка)
const int clockPin = 8;
// пин данных D11, подключается на плате 74HC595 к DS (14 ножка)
const int dataPin = 4;
// пин для земли первой цифры
const int oneDigit = A1;
// пин для земли второй цифры
const int secondDigit = A2;
// пин для земли третьей цифры
const int threeDigit = A3;
// пин для земли четвертой цифры
const int fourDigit = A4;

unsigned long prevMillies;
unsigned long currentMillies;
// интервал для обновления секунды 
const unsigned long interval = 250;  // 1 секунда
// переменная секунды
int seconds = 0;
// 2 байта для отображения минут на циферблате
unsigned int min = 0x0500;   // минуты
// 2 байта для отображения часа на циферблате
unsigned int hour = 0x0100;  // часы
byte data = 0;
void setup() {
  // put your setup code here, to run once:
  // Инициалиазация пинов для дальнейшей работы с 74HC595
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(oneDigit, OUTPUT);
  pinMode(secondDigit, OUTPUT);
  pinMode(threeDigit, OUTPUT);
  pinMode(fourDigit, OUTPUT);
}
// Массив состоящий из пинов земель транзисторов
int grounds[4] = {
  oneDigit,     // A1
  secondDigit,  // A2
  threeDigit,   // A3
  fourDigit     // A4
};
// Набор цифр для сдвигового регистра
byte digitals[11] = {
  0b1111110,  // digit 0
  0b0110000,  // digit 1
  0b1101101,  // digit 2
  0b1111001,  // digit 3
  0b0110011,  // digit 4
  0b1011011,  // digit 5
  0b1011111,  // digit 6
  0b1110000,  // digit 7
  0b1111111,  // digit 8
  0b1111011,  // digit 9
  0b1001111,  // error E
};
// метод отключает землю у всех сегментов
void offAllSegmentsDisplaing() {
  for (int i = 0; i < 4; i++) {
    disconnectToGround(i);
  }
}
// метод выводит на конкретном сегменте дисплея цифру , задержка 1 секунда и отключает
void printSpecificSegment(int digitSegment, byte valuePrint) {
  connectToGround(digitSegment - 1);
  printDisplay(valuePrint);
  delay(1000);
  disconnectToGround(digitSegment - 1);
}
// метод выводит на всех сегментах с заденржкой 250мс, цифры из массива
void displaingValues(int values[], int delayTime) {
  for (int indexDigit = 0; indexDigit < 4; indexDigit++) {
    writeDigit(indexDigit, values[indexDigit]);
    delay(delayTime);
    disconnectToGround(indexDigit);
  }
}
// метод покдлючает землю и устанавливает указанном сегменте дисплея указанную цифру
void writeDigit(int digitSelectedSegments, byte printDigit) {
  connectToGround(digitSelectedSegments);  // подтянул землю
  printDisplay(printDigit);                // выставил в сдвиговый регистор необхоимую цифру
}
// метод подключает землю к указанной цифре
void connectToGround(int digitGround) {
  digitalWrite(grounds[digitGround], HIGH);
}
// метод отключает землю от указанной цифры
void disconnectToGround(int digitGround) {
  digitalWrite(grounds[digitGround], LOW);
}
// метод выставляет в сдвиговый регистор, необходимую цифру
// и щелкает для дальнейшей передачи
void printDisplay(byte printNumber) {
  int templatedIndex;
  if (printNumber >= 10 || printNumber < 0) {
    templatedIndex = digitals[10];
  } else {
    templatedIndex = digitals[printNumber];
  }
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);
  for (int index = 0; index <= 7; index++) {
    digitalWrite(clockPin, LOW);
    if ((templatedIndex >> index) & 1) {  // если бит под индексом равен 1
      digitalWrite(dataPin, HIGH);
    } else {  // иначе 0
      digitalWrite(dataPin, LOW);
    }
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
  digitalWrite(clockPin, HIGH);
}
bool firstStart = true;
void InitializatedDisplay() {
  int error[4] = { 11, 11, 11, 11 };
  displaingValues(error, 250);
  delay(250);
  displaingValues(error, 250);
  delay(250);
  displaingValues(error, 250);
  firstStart = false;
}
byte getHighByte(unsigned int val) {
  return (val & 0xFF);
}
byte getLowByte(unsigned int val) {
  return (val >> 8) & 0xFF;
}
bool updateHighByte(unsigned int &val, int max) {
  unsigned int highByte = (val >> 8) & 0xFF;
  highByte++;
  if (highByte > max) return false;
  val = (highByte << 8) | (val & 0xFF);
  return true;
}
bool updateLowByte(unsigned int &val, int max) {
  byte lowByte = (byte)(val & 0xFF);
  lowByte++;
  if (lowByte > max) return false;
  val = (val & 0xFF00) | (lowByte & 0xFF);
  return true;
}
void incrementHour(unsigned int &hour) {
  byte leftDigital = getLowByte(hour);
  byte rightDigital = getHighByte(hour);
  rightDigital++;
  if (leftDigital >= 2) {
    if (rightDigital > 4) {
      leftDigital = 0;
      rightDigital = 1;
    }
  }
  if (rightDigital > 9) {

    rightDigital = 0;
    leftDigital++;
  }
  hour = (leftDigital << 8) | rightDigital;
}
void incrementMinutes(unsigned int &min) {
  byte leftDigital = getLowByte(min);    // left
  byte rightDigital = getHighByte(min);  // right

  rightDigital++;
  if (rightDigital >= 9) {
    rightDigital = 0;
    leftDigital++;
    if (leftDigital > 5) {
      leftDigital = 0;  // сбросить минуты на 00
      incrementHour(hour);
    }
  }
  min = (leftDigital << 8) | rightDigital;
}

void updateClock() {
  currentMillies = millis();
  if (currentMillies - prevMillies >= interval) {  // если прошло указанное время
    prevMillies = currentMillies;
    if (seconds >= 1) {  // прошла минута
      seconds = 0;
      incrementMinutes(min);
    }
    seconds++;
  }
}

void loop() {
  if (firstStart) {
    InitializatedDisplay();
  }
  updateClock();
  int values[4] = { getLowByte(hour), getHighByte(hour), getLowByte(min), getHighByte(min) };
  displaingValues(values, 5);
  // printSpecificSegment(3, 8);
}