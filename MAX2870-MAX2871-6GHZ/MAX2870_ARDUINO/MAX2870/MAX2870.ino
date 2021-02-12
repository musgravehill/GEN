
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

#define MAX2871_reference_frequency_mhz 25.0  //reference frequency 25-50-100MHz quartz
#define MAX2871_R_divider 2 //R divider to set phase/frequency detector comparison frequency
#define MAX2871_pin_LE 10   //DigitalOut, init = 1 //latch enable
#define MAX2871_pin_CE 9    //DigitalOut, init =1 //chip enable
#define MAX2871_pin_RF_EN 8    //DigitalOut, init =1 //RF output enable  PDBRF. RF Power-Down. A logic low on this pin mutes the RF outputs

             

MAX2871 MAX2871_my(10);

void setup() {
  pinMode (MAX2871_pin_LE, OUTPUT);
  digitalWrite(MAX2871_pin_LE, LOW);

  pinMode (MAX2871_pin_CE, OUTPUT);
  digitalWrite(MAX2871_pin_CE, LOW);

  pinMode (MAX2871_pin_RF_EN, OUTPUT);
  digitalWrite(MAX2871_pin_RF_EN, LOW);

  SPI.setClockDivider(SPI_CLOCK_DIV16); //16MHz system clock \ 16 = 1MHz SPI
  SPI.setDataMode(SPI_MODE0); //CPOL = CPHA = 0, 8 bits per frame
  SPI.setBitOrder(MSBFIRST);

  SPI.begin();

  MAX2871_my.powerOn(true); //set all hardware enable pins and deassert software shutdown bits
  MAX2871_my.setPFD(MAX2871_reference_frequency_mhz , MAX2871_R_divider); //inputs are reference frequency and R divider to set phase/frequency detector comparison frequency
  MAX2871_my.setRFOUTA(433);

}

void loop() {
  // put your main code here, to run repeatedly:

}
