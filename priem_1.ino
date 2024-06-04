#include <SPI.h>
//#include "nRF24L01.h"
//#include "RF24.h"
//
//RF24 radio(9, 10); // порты D9, D10: CSN CE
//uint8_t  pipe;
// // адрес рабочей трубы;
//
//byte data[1];
//int scn;  //счетчик циклов прослушивания эфира
//int sm;  //счетчик числа принятых пакетов с передатчика
//
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("ReceiverTester ON");
//
//  radio.begin();  // инициализация
//  delay(2000);
//  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
//  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
//   
//  radio.setChannel(0x7c);// установка канала
//  radio.setAutoAck(false);       // автоответ
//  radio.openReadingPipe(1, 0xAABBCCDD22LL); // открыть трубу на приём
//  radio.openReadingPipe(2, 0xAABBCCDD11LL);
//  radio.startListening();        // приём
//}
//
//void loop() {
//    if (radio.available(&pipe))
//    {
//      radio.read(&data, sizeof(data));
//      if(pipe==2)
//      {
//      sm = data [0];
//      Serial.println("Расстояние = " + String(sm));
//      }
//  else
//  {
//    Serial.print("Нет сигнала");
//  }
//      
//     if(pipe==1){
//      
//      sm = data [0];
//      Serial.println("Расстояние = " + String(sm));
//      }
//  else
//  {
//    Serial.print("Нет сигнала");
//  }
//      
//    }
//  delay(750);
//}
//
//
//
//











#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10); // порты D9, D10: CSN CE
uint8_t  pipe;

byte data[2]; // Массив для хранения двух дальностей
float d1, d2; // Переменные для хранения дальностей от объекта до каждой антенны

void setup() {
  Serial.begin(115200);
  Serial.println("Receiver ON");

  radio.begin();  // инициализация
  delay(2000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setChannel(0x7c);// установка канала
  radio.setAutoAck(false);       // автоответ
  radio.openReadingPipe(1, 0xAABBCCDD22LL); // открыть трубу на приём
  radio.openReadingPipe(2, 0xAABBCCDD11LL);
  radio.startListening();        // приём
}

void loop() {
  if (radio.available(&pipe)) {
    radio.read(&data, sizeof(data));
    if (pipe == 1) {
      d1 = data[0]; // Сохраняем первую дальность
      Serial.print("Расстояние = " + String(d1));
    } else if (pipe == 2) {
      d2 = data[0]; // Сохраняем вторую дальность
      Serial.print("Расстояние = " + String(d2));
    }

    if (d1 != 0 && d2 != 0) { // Если обе дальности измерены
      calculateCoordinates(d1, d2); // Вызываем функцию для вычисления координат объекта
      d1 = 0; // Сбрасываем значения дальностей для следующих измерений
      d2 = 0;
    }
  }
}

// Функция для вычисления координат объекта
void calculateCoordinates(float d1, float d2) {
  // Координаты известных точек (антенн)
  float x1 = 0.0; // Координата x первой антенны
  float y1 = 0.0; // Координата y первой антенны
  float x2 = 30.0; // Координата x второй антенны
  float y2 = 2.0; // Координата y второй антенны

  // Рассчет координат объекта с помощью трилатерации
  float A = 2 * (x2 - x1);
  float B = 2 * (y2 - y1);
  float C = (d1 * d1 - d2 * d2) - (x1 * x1 - x2 * x2) - (y1 * y1 - y2 * y2);

  float objectX = (C * B - A * C) / (A * A + B * B);
  float objectY = (A * C + B * C) / (A * A + B * B);

  // Вывод координат объекта
  Serial.print("Координаты объекта: (");
  Serial.print(objectX); // Вывод координаты x объекта
  Serial.print(", ");
  Serial.print(objectY); // Вывод координаты y объекта
  Serial.println(")");}  

  // Отправка координат через серийный порт для построения графика
  // Например, можно использовать Processing для визуализации координат на графике





  
