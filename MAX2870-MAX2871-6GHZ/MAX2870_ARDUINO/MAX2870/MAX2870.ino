
//TODO
/*
pins in constrructor  MAX2871 MAX2871_my(10);
function setPFD???  
How to determinate f_pfd = frequency of PFD
How to determinate frequency reference from oscillator?

See adf4351 SPI - 2 function. Там даннные ля регистра дробятся, потом пишутся. Тут дробление встроено в запись. Норм это?
void ADF4351_writeToRegister(int idx)
{ // make 4 byte from integer for SPI-Transfer
  byte buf[4];
  for (uint8_t i = 0; i < 4; i++)
    buf[i] = (byte)(ADF4351_registers[idx] >> (i * 8));
  ADF4351_writeData(buf[3], buf[2], buf[1], buf[0]);
}
int ADF4351_writeData(byte a1, byte a2, byte a3, byte a4) {
  // write over SPI to ADF4351
  digitalWrite(ADF4351_ss_pin, LOW);
  delayMicroseconds(10);
  SPI.transfer(a1);
  SPI.transfer(a2);
  SPI.transfer(a3);
  SPI.transfer(a4);
  ADF4351_ss_toggle();
}
int ADF4351_ss_toggle() {
  digitalWrite(ADF4351_ss_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ADF4351_ss_pin, LOW);
}




Что значит <> "" include


SET SPI mode0, MSB, spi begin

pinMode (ADF4351_ss_pin, OUTPUT);
  digitalWrite(ADF4351_ss_pin, LOW);
  
  delay(900);
  ADF4351_setConfig();
  
 */


#include <SPI.h>
#include "MAX2871.h" // .h auto-connect .cpp ?  #include "MAX2871.cpp"


double f_pfd = 50;//???
MAX2871 MAX2871_my(10);

void setup() {
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  
  MAX2871_my.setRFOUTA(433);

}

void loop() {
  // put your main code here, to run repeatedly:

}
