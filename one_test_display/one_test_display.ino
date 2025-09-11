/// пин зашелка, подключается на плате 74HC595 к ST_CP (12 ножка)
int latchPin = 7;
// пин тактирования , подключается на плате 74HC595 к SH_CP (11 ножка)
int clockPin = 8; 
// пин данных D11, подключается на плате 74HC595 к DS (14 ножка)
int dataPin = 4;

byte data = 0;
void setup() {
  // put your setup code here, to run once:
  // Инициалиазация пинов для дальнейшей работы с 74HC595 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {

  // printDisplay(0);
  for (int i = 0; i < 10 ; i++) {
    printDisplay(i);
    delay(500);
  }
  printDisplay(33);
  delay(1000);
}
byte digitals[11] = 
{
  0b1111110, // digit 0
  0b0110000, // digit 1
  0b1101101, // digit 2
  0b1111001, // digit 3
  0b0110011, // digit 4
  0b1011011, // digit 5
  0b1011111, // digit 6
  0b1110000, // digit 7
  0b1111111, // digit 8
  0b1111011, // digit 9
  0b1001111, // error E
};
void printDisplay(byte printNumber) {
  int templatedIndex;
  if (printNumber >=10 || printNumber < 0) {
    templatedIndex = digitals[10];
  } else {
    templatedIndex = digitals[printNumber];
  }
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);-
  for(int index = 0; index <= 7; index++) {
    digitalWrite(clockPin, LOW);
    if((templatedIndex >> index) & 1) { // если бит под индексом равен 1
      digitalWrite(dataPin, HIGH);
    } else { // иначе 0
      digitalWrite(dataPin, LOW);
    }
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
  digitalWrite(clockPin, HIGH);
}