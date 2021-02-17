

#include <SPI.h>
#include "MAX2870.h"
#include <math.h>
#include <stdio.h>

//****************************************************************************
MAX2870::MAX2870(const uint8_t MAX2870_pin_LE, const uint8_t MAX2870_pin_CE, const uint8_t MAX2870_pin_RF_EN, const uint8_t MAX2870_pin_LD) {
  pin_LE = MAX2870_pin_LE;
  pin_CE = MAX2870_pin_CE;
  pin_RF_EN = MAX2870_pin_RF_EN;
  pin_LD = MAX2870_pin_LD;
}

void MAX2870::start() {
  delay(100);

  SPI.setClockDivider(SPI_CLOCK_DIV16); //16MHz system clock \ 16 = 1MHz SPI
  SPI.setDataMode(SPI_MODE0); //CPOL = CPHA = 0, 8 bits per frame
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  delay(50);

  pinMode (pin_LE, OUTPUT);
  digitalWrite(pin_LE, 1);

  pinMode (pin_CE, OUTPUT);
  digitalWrite(pin_CE, 1);

  pinMode (pin_RF_EN, OUTPUT);
  digitalWrite(pin_RF_EN, 0); //disable rf_out until fully start

  pinMode (pin_LD, INPUT);

  delay(100);

  // fractional-N mode  see registers at MAX2870-all.png

  reg0.all = 0x0022B568;
  reg1.all = 0x2000FD01;
  reg2.all = 0x80008042;
  reg3.all = 0x0000000B;
  reg4.all = 0x63BE823C;
  reg5.all = 0x00400005;
  reg6.all = 0x00000006;

  setConfig();

  delay(100); //see datasheed: init  & powerUp

  setConfig();

  delay(50); //see datasheed: init  & powerUp

  setActive(true);
}



//****************************************************************************
void MAX2870::writeData(uint32_t data) {
  digitalWrite(pin_LE, 0);
  delayMicroseconds(300);

  SPI.transfer((0xFF000000 & data) >> 24);  //????  в таком порядке или обратном????
  SPI.transfer((0x00FF0000 & data) >> 16);
  SPI.transfer((0x0000FF00 & data) >> 8);
  SPI.transfer( 0x000000FF & data);

  delayMicroseconds(2500);
  digitalWrite(pin_LE, 1);
  delayMicroseconds(2500);
}

//****************************************************************************
void MAX2870::setConfig() {
  writeData(reg5.all);
  writeData(reg4.all);
  writeData(reg3.all);
  writeData(reg2.all);
  writeData(reg1.all);
  writeData(reg0.all);
}



//****************************************************************************
void MAX2870::set_frequency_OUT_A(double freqMHz) {
  uint32_t n, frac, m, diva = 0;
  double pll_coefficient, fractional = 0;

  while (freqMHz * powf(2, diva) < 3000.0)  {
    diva = diva + 1;
  }
  pll_coefficient = freqMHz * powf(2, diva) / f_pfd;
  n = floor(pll_coefficient);

  fractional = pll_coefficient - n;
  m = 4000;
  frac = round(m * fractional);

  reg0.bits.frac = frac;
  reg0.bits.n = n;
  reg1.bits.m = m;
  reg4.bits.diva = diva;

  //reg3.bits.mutedel = 1;  2871 only

  setConfig();

  f_out_A = f_pfd * (reg0.bits.n + 1.0 * reg0.bits.frac / reg1.bits.m) / powf(2, reg4.bits.diva);
}

void MAX2870::setPFD(const double referenceFreqMHz, const uint16_t rdiv) {
  // fPFD = fREF * [(1 + DBR)/(R * (1 + RDIV2))]
  // DBR=0 RDIV2=0
  // fPFD = fREF * [1/R] = fREF / R
  //

  f_pfd = referenceFreqMHz / rdiv;

  if (f_pfd > 32.0) {
    reg2.bits.lds = 1;
  }
  else {
    reg2.bits.lds = 0;
  }

  reg3.bits.cdiv = round(f_pfd / 0.10);

  reg2.bits.dbr = 0;
  reg2.bits.rdiv2 = 0;
  reg2.bits.r = rdiv;

  uint32_t bs = f_pfd * 20;

  if (bs > 1023) {
    bs = 1023;
  }
  else if (bs < 1) {
    bs = 1;
  }

  reg4.bits.bs = 0x03FF & bs;
  reg4.bits.bs2 = 0x03 & (bs >> 8);

  setConfig();
}


void MAX2870::setActive(bool isOn) {
  reg2.bits.shdn =  (isOn ? B0 : B1); //sd = shootDown
  reg4.bits.sdldo = (isOn ? B0 : B1);
  reg4.bits.sddiv = (isOn ? B0 : B1);
  reg4.bits.sdref = (isOn ? B0 : B1);
  //reg4.bits.sdvco = !isOn;  2871 only
  //reg5.bits.sdpll = !isOn;  2871 only

  setConfig();

  digitalWrite(pin_RF_EN, (isOn ? 1 : 0));
}

double MAX2870::getPFD() {
  return f_pfd;
}

double MAX2870::get_frequency_OUT_A() {
  return f_out_A;
}

void MAX2870::set_noiseMode() {
  noiseMode_idx = constrain(noiseMode_idx, 0, 2);
  reg2.bits.sdn = noiseMode[noiseMode_idx];
  setConfig();
}

void MAX2870::set_power_OUT_A() {
  outPower_idx = constrain(outPower_idx, 0, 3);
  reg4.bits.apwr = outPower[outPower_idx];
  setConfig();
}

void MAX2870::set_chargePumpCurrent() {
  chargePumpCurrent_idx = constrain(chargePumpCurrent_idx, 0, 15);
  reg2.bits.cp = chargePumpCurrent[chargePumpCurrent_idx];
  setConfig();
}








