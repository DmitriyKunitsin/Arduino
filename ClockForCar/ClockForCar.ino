// Если прошло указанное в интервале время, то вернет true, иначе false
bool IsMillies(unsigned long interval);
// подмигивает выбранным сегментом и меняет время по нажатию кнопки
void blinkSettingDigits(int segments[], int countSegments);
// метод отключает землю у всех сегментов
void offAllSegmentsDisplaing();
// метод выводит на конкретном сегменте дисплея цифру , задержка 1 секунда и отключает
void printSpecificSegment(int digitSegment, byte valuePrint);
// метод выводит на всех сегментах с заденржкой 250мс, цифры из массива
void displaingValues(int values[], int delayTime);
// метод покдлючает землю и устанавливает указанном сегменте дисплея указанную цифру
void writeDigit(int digitSelectedSegments, byte printDigit);
// метод подключает землю к указанной цифре
void connectToGround(int digitGround);
// метод отключает землю от указанной цифры
void disconnectToGround(int digitGround);
// метод выставляет в сдвиговый регистор, необходимую цифру
// и щелкает для дальнейшей передачи
void printDisplay(byte printNumber);
// метод подсветки инициализации, при первом запуске
void InitializatedDisplay();
// возвращает страший байт
byte getHighByte(unsigned int val);
// возвращает младший байт
byte getLowByte(unsigned int val);
// инкрементирует часы
void incrementHour(unsigned int &hour);
// инкрементирует минуты
void incrementMinutes(unsigned int &min);
// обнволение часов с определенным интервалом
void updateClock();
// метод срабатывает при прерывании, сделал задержку 100мс для избежания дребезжания контактов
void localInterrapt();
// Обновляет массив с временем
void updateTime();
// Если IsCheckedBtnSetting, то входит в этот метод и проверяет состояние нажатия на кнопку настроек
void checkDurationBtnSetting();
// меняет выбранный сегмент
bool changeSetment(bool lvlSegment);
// метод со всей логикой работы в режиме настроек
void Settings();
// Метод со всей логикой работы в режиме работы
void DoWork();
// массив содержит в себе значения выбранного сегмента
byte val[2];
// массив содержит в себе номера выбранных сегментов
int seg[2];
// массив содержит в себе текущее время
int time[4];
/// пин зашелка, подключается на плате 74HC595 к ST_CP (12 ножка)
#define LATCH_PIN 7
// пин тактирования , подключается на плате 74HC595 к SH_CP (11 ножка)
#define CLOCK_PIN 8
// пин данных D11, подключается на плате 74HC595 к DS (14 ножка)
#define DATA_PIN 4
// пин для земли первой цифры
#define PIN_GROUND_ONE_DIGIT A1
// пин для земли второй цифры
#define PIN_GROUND_SECOND_DIGIT A2
// пин для земли третьей цифры
#define PIN_GROUND_THREE_DIGIT A3
// пин для земли четвертой цифры
#define PIN_GROUND_FOUR_DIGIT A4
// пин для кнопки D9
#define PIN_BUTTON_SETTING_HOUR 9
// пин для кнопки D6
#define PIN_BUTTON_SETTING_MIN 6
// надо все переделать на дефайны
#define BTN_PIN_INTERRUPT 3
// первый ли запуск
bool firstStart = true;
// булевая переменная определяющая какой из сегментов будет подсвечиваться
bool isChangedSegment = true;
// перечисление состояний
const int Work = 0;  // работа
const int Setting = 1;
// текущее состояние
volatile int status = Work;  // 0 - work
unsigned long prevMillies;
unsigned long currentMillies;
// интервал для обновления секунды
const unsigned long interval = 1000;  // 1 секунда
// скольки секундам равна минута для обнуления секундомера
const unsigned int minuteForClock = 6;
// переменная секунды
int seconds = 0;
// 2 байта для отображения минут на циферблате
unsigned int min = 0x0500;  // минуты
// 2 байта для отображения часа на циферблате
unsigned int hour = 0x0100;  // часы
// Массив состоящий из пинов земель транзисторов
int grounds[4] = {
  PIN_GROUND_ONE_DIGIT,     // A1
  PIN_GROUND_SECOND_DIGIT,  // A2
  PIN_GROUND_THREE_DIGIT,   // A3
  PIN_GROUND_FOUR_DIGIT     // A4
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
// Если прошло указанное в интервале время, то вернет true, иначе false
bool IsMillies(unsigned long interval) {
  currentMillies = millis();
  if (currentMillies - prevMillies >= interval) {
    return true;
  } else {
    return false;
  }
}
// метод отключает землю у всех сегментов
void offAllSegmentsDisplaing() {
  for (int i = 0; i < 4; i++) {
    disconnectToGround(i);
  }
}
void blinkSettingDigits(int segments[], int countSegments, byte digitPrintDisplay[]) {
  for (int i = 0; i < countSegments; i++) {
    printSpecificSegment(segments[i], digitPrintDisplay[i]);
  }
}
// метод выводит на конкретном сегменте дисплея цифру , задержка 1 секунда и отключает
void printSpecificSegment(int digitSegment, byte valuePrint) {
  connectToGround(digitSegment);
  printDisplay(valuePrint);
  delay(250);
  disconnectToGround(digitSegment);
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
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  for (int index = 0; index <= 7; index++) {
    digitalWrite(CLOCK_PIN, LOW);
    if ((templatedIndex >> index) & 1) {  // если бит под индексом равен 1
      digitalWrite(DATA_PIN, HIGH);
    } else {  // иначе 0
      digitalWrite(DATA_PIN, LOW);
    }
    digitalWrite(CLOCK_PIN, HIGH);
  }
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(CLOCK_PIN, HIGH);
}
void InitializatedDisplay() {
  int error[4] = { 11, 11, 11, 11 };
  displaingValues(error, 250);
  delay(250);
  displaingValues(error, 250);
  delay(250);
  displaingValues(error, 250);
  firstStart = false;
}
// возвращает страший байт
byte getHighByte(unsigned int val) {
  return (val & 0xFF);
}
// возвращает младший байт
byte getLowByte(unsigned int val) {
  return (val >> 8) & 0xFF;
}
// инкрементирует часы
void incrementHour(unsigned int &hour) {
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
// инкрементирует минуты
void incrementMinutes(unsigned int &min) {
  byte leftDigital = getLowByte(min);    // left
  byte rightDigital = getHighByte(min);  // right

  rightDigital++;
  if (rightDigital >= 9) {
    rightDigital = 0;
    leftDigital++;
    if (leftDigital > 5) {
      leftDigital = 0;  // сбросить минуты на 00
      if (status == Work) {
        incrementHour(hour);
      }
    }
  }
  min = (leftDigital << 8) | rightDigital;
}
// обнволение часов с определенным интервалом
void updateClock() {
  if (IsMillies(interval)) {  // если прошло указанное время
    prevMillies = currentMillies;
    if (seconds >= minuteForClock) {  // прошла минута
      seconds = 0;
      incrementMinutes(min);
    }
    seconds++;
  }
}
void updateTime() {
  time[0] = getLowByte(hour);
  time[1] = getHighByte(hour);
  time[2] = getLowByte(min);
  time[3] = getHighByte(min);
}
// время последнего надатия на кнопку прерывания
volatile unsigned long lastPressInterruptBtn = 0;
volatile unsigned long lastInterruptTime = 0;
volatile bool IsCheckedBtnSetting = false;
// метод срабатывает при прерывании, сделал задержку 300мс для избежания дребезжания контактов
void localInterrapt() {
  unsigned long currentTime = millis();
  if (currentTime - lastPressInterruptBtn > 300) {  // антидребезг 300 мс
    IsCheckedBtnSetting = true;
    lastPressInterruptBtn = currentTime;
  }
}
void checkDurationBtnSetting() {
  unsigned long pressDuration = millis() - lastPressInterruptBtn;          // время с момента последнего нажатия
  if (pressDuration >= 10000 && digitalRead(BTN_PIN_INTERRUPT) == HIGH) {  // после последнего нажатия на кнопку настроек прошло 10 сек, выходим из настроек
    IsCheckedBtnSetting = false;
    Serial.println("Кнопка настроек не была нажата более 10 сек");
    status = Work;
  } else if (status != Setting && pressDuration >= 1500 && digitalRead(BTN_PIN_INTERRUPT) == LOW) {  // кнопка зажата более 1.5 сек
    status = Setting;
    Serial.println("Вошли в настройки");
  } else if (status == Setting && digitalRead(BTN_PIN_INTERRUPT) == LOW) {  // если в настройках сидим и нажатие было короче 1 с
    changeSetment(isChangedSegment);
    isChangedSegment = !isChangedSegment;
  }
}
bool changeSetment(bool lvlSegment) {
  if (lvlSegment) {  // hour
    val[0] = time[0];
    val[1] = time[1];
    seg[0] = 0;
    seg[1] = 1;
    return true;
  } else  // min
  {
    val[0] = time[2];
    val[1] = time[3];
    seg[0] = 2;
    seg[1] = 3;
    return false;
  }
}
void Settings() {
  if (changeSetment(isChangedSegment)) {
    byte btnLeftHourChange = digitalRead(PIN_BUTTON_SETTING_HOUR);
    if (btnLeftHourChange == LOW) {
      incrementHour(hour);
      updateTime();
      lastPressInterruptBtn = millis();
    }
  } else {
    byte btnRightMinChange = digitalRead(PIN_BUTTON_SETTING_MIN);
    if (btnRightMinChange == LOW) {
      incrementMinutes(min);
      updateTime();
      lastPressInterruptBtn = millis();
    }
  }
  blinkSettingDigits(seg, 2, val);
  delay(25);
}
void DoWork() {
  updateClock();
  updateTime();
  displaingValues(time, 5);
}
void setup() {
  // put your setup code here, to run once:
  // Инициалиазация пинов для дальнейшей работы с 74HC595
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  // пины транзисторов
  pinMode(PIN_GROUND_ONE_DIGIT, OUTPUT);
  pinMode(PIN_GROUND_SECOND_DIGIT, OUTPUT);
  pinMode(PIN_GROUND_THREE_DIGIT, OUTPUT);
  pinMode(PIN_GROUND_FOUR_DIGIT, OUTPUT);
  // пин D3 для обработки нажатия кнопки для прерывания (подать GND) D3 -> Button -> GND
  pinMode(BTN_PIN_INTERRUPT, INPUT_PULLUP);
  // FALLING - при нажатии на кнопку будет сигнал 0
  attachInterrupt(1, localInterrapt, FALLING);
  // пины кнопок
  pinMode(PIN_BUTTON_SETTING_HOUR, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SETTING_MIN, INPUT_PULLUP);
  // Serial.begin(9600);
}
void loop() {
  if (firstStart) {
    // InitializatedDisplay();
  }
  if (IsCheckedBtnSetting) {
    checkDurationBtnSetting();
  }
  if (status == Work) {
    DoWork();
  } else if (status == Setting) {
    Settings();
  } else {
    Serial.println("Default");
    printSpecificSegment(1, 44);
  }
  // for (int j = 0; j < 4; j++) {
  //   for (int i = 0; i < 10; i++) {
  //     // connectToGround(j);
  //     printSpecificSegment(j,i);
  //     // delay(250);
  //     // disconnectToGround(j);
  //   }
  // }
}
