
#include <SPI.h>
#include <SD.h>

File myFile;

const int analogInPin = A0;
uint16_t sensorValue = 0;

#define average_count 128
uint16_t average_val = 1;
uint8_t average_i = 0;
uint16_t average_el[average_count];

uint32_t c_0 = 0L;
uint32_t c_1 = 0L;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }




  //analogReference(INTERNAL); //===========1.1V===============================================


  // ADCSRA |= (1 << ADPS2);                     //Биту ADPS2 присваиваем единицу - коэффициент деления 16
  // ADCSRA &= ~ ((1 << ADPS1) | (1 << ADPS0));  //Битам ADPS1 и ADPS0 присваиваем нули

  for (int i = 0; i < average_count; i++) {
    average_el[i] = 0;
  }
}

void loop() {
  //delay(100); //TODO test delays 0..***
  myFile = SD.open("f01.dat", FILE_WRITE);
  sensorValue = analogRead(analogInPin);
  byte b = (byte) sensorValue; //get last small 8 bit
  myFile.write(b);
  myFile.close();

  //Serial.println();
  //Serial.print("curr=");
  //Serial.println(sensorValue, DEC);
  /*Serial.print(b, BIN);
  Serial.print('=');
  Serial.print(bitRead(sensorValue, 0), DEC);
  Serial.print(bitRead(sensorValue, 1), DEC);
  Serial.print(bitRead(sensorValue, 2), DEC);
  Serial.print(bitRead(sensorValue, 3), DEC);
  Serial.print(bitRead(sensorValue, 4), DEC);
  Serial.print(bitRead(sensorValue, 5), DEC);
  Serial.print(bitRead(sensorValue, 6), DEC);
  Serial.print(bitRead(sensorValue, 7), DEC);
  Serial.println();*/

  /*
    average_el[average_i] = sensorValue;
    average_i++;
    if (average_i >= average_count) {
      average_i = 0;

      uint32_t average_sum = 0L;
      for (uint8_t i = 0; i < average_count; i++) {
        average_sum += average_el[i];
      }
      average_val = 0.8 * average_val + 0.2 * average_sum / average_count;
    }


    int16_t delta = average_val - sensorValue;
      delta = abs(delta);
      if (delta < 50) {
      if (sensorValue > average_val) {
        c_1++;
        Serial.print(1);
      } else {
        c_0++;
        Serial.print(0);
      }
      }*/


  /*if ((millis() % 5000) < 10) {
    Serial.print(average_val);
    Serial.print(' ');
    Serial.print(c_0);
    Serial.print(' ');
    Serial.println(c_1);
    }*/
}
