
#include <SPI.h>
#include <RF24.h>

#define PIN_TRIG 8
#define PIN_ECHO 7

RF24 radio(9, 10); // порты D9, D10: CSN CE
 // адрес рабочей трубы;

byte data;
long duration, cm;
const int numReadings = 5; // количество чтений для медианного фильтра
int readings[numReadings]; // массив для хранения последних чтений
int index = 0; // индекс текущего чтения

void setup()
{
Serial.begin(115200);
Serial.println("TransmitterTester ON");

radio.begin(); // инициализация
delay(600);
radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
radio.setChannel(0x7c); // установка канала
radio.setAutoAck(false); // автоответ
radio.powerUp(); // включение или пониженное потребление powerDown - powerUp
radio.stopListening(); //радиоэфир не слушаем, только передача
radio.openWritingPipe(0xAABBCCDD33LL); // открыть трубу на отправку

Serial.begin (115200);
//Определяем вводы и выводы
pinMode(PIN_TRIG, OUTPUT);
pinMode(PIN_ECHO, INPUT);

const int numReadings = 5; // количество чтений для медианного фильтра
int readings[numReadings]; // массив для хранения последних чтений
int index = 0; // индекс текущего чтения
}

void loop(){
digitalWrite(PIN_TRIG, LOW);
delayMicroseconds(5);
digitalWrite(PIN_TRIG, HIGH);

// Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
delayMicroseconds(10);
digitalWrite(PIN_TRIG, LOW);

// Время задержки акустического сигнала на эхолокаторе.
duration = pulseIn(PIN_ECHO, HIGH);

// Теперь осталось преобразовать время в расстояние
cm = (duration / 2) / 29.1;
// добавление текущего чтения в массив
readings[index] = cm;
index++;

// если достигнут конец массива, начинаем заново
if (index >= numReadings) {
index = 0;
}

// сортировка массива чтений
for (int i = 0; i < numReadings - 1; i++) {
for (int j = i + 1; j < numReadings; j++) {
if (readings[i] > readings[j]) {
int tmp = readings[i];
readings[i] = readings[j];
readings[j] = tmp;
}
}
}

// определение медианного значения
int median = readings[numReadings / 2];

// вывод медианного значения на серийный порт
Serial.println(median);

// Задержка между измерениями для корректной работы скеча
delay(600);

data = median;
radio.write(&median, 1);
Serial.println("distance= " + String(cm));

}
