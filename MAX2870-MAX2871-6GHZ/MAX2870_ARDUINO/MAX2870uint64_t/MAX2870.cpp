

#include <SPI.h>
#include "MAX2870.h"
#include <math.h>
#include <stdio.h>

//****************************************************************************
//const uint8_t MAX2870_pin_CE, const uint8_t MAX2870_pin_RF_EN,
MAX2870::MAX2870(const uint8_t MAX2870_pin_LE, const uint8_t MAX2870_pin_LD) {
  pin_LE = MAX2870_pin_LE;
  pin_LD = MAX2870_pin_LD;
  //pin_CE = MAX2870_pin_CE;
  //pin_RF_EN = MAX2870_pin_RF_EN;
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

  pinMode (pin_LD, INPUT);

  //pinMode (pin_CE, OUTPUT);
  //digitalWrite(pin_CE, 1);

  //pinMode (pin_RF_EN, OUTPUT);
  //digitalWrite(pin_RF_EN, 0); //disable rf_out until fully start

  delay(100);

  // fractional-N mode  see registers at MAX2870-all.png

  reg0.all = 0x0022B568;
  reg1.all = 0x2000FD01;
  reg2.all = 0x80008042;
  reg3.all = 0x00000FA3;
  reg4.all = 0x63BE8224;
  reg5.all = 0x00400005;
  reg6.all = 0x00000006;

  setConfig();

  delay(50); //Upon power-up, the registers should be programmed twice with at least a 20ms pause between writes.

  setConfig();

  delay(50);
}



//****************************************************************************
void MAX2870::writeData(uint32_t data) {
  digitalWrite(pin_LE, 0);
  delayMicroseconds(2); //datasheed tLES=20ns=0,02mks. But I set 2mks

  SPI.transfer((0xFF000000 & data) >> 24);  //????  в таком порядке или обратном????
  SPI.transfer((0x00FF0000 & data) >> 16);
  SPI.transfer((0x0000FF00 & data) >> 8);
  SPI.transfer( 0x000000FF & data);

  delayMicroseconds(2); //datasheed tLEH=10ns=0,01mks. But I set 2mks
  digitalWrite(pin_LE, 1);
  delayMicroseconds(10); //datasheed tLEW=20ns=0,02mks. But I set 10mks

  //Serial.println(" ");
  //Serial.println(data, HEX);
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
void MAX2870::pre_set_frequency_OUT_A(uint64_t freqHz) {
  uint32_t n, frac, m, diva = 0;
  double pll_coefficient, fractional = 0;
  m = 4000; // max 4096. Чем больше, тем точнее считает.

  /*double freqMHz = freqHz / 1000000.0;
    while (freqMHz * powf(2, diva) < 3000.0)  {
    diva = diva + 1;
    }*/

  if (freqHz >= 3000000000) {
    diva = B000;
  }
  else if (1500000000 <= freqHz && freqHz < 3000000000) {
    diva = B001;
  }
  else if (750000000 <= freqHz && freqHz < 1500000000) {
    diva = B010;
  }
  else if (375000000 <= freqHz && freqHz < 750000000) {
    diva = B011;
  }
  else if (187500000 <= freqHz && freqHz < 375000000) {
    diva = B100;
  }
  else if (93750000 <= freqHz && freqHz < 187500000) {
    diva = B101;
  }
  else if (46875000 <= freqHz && freqHz < 93750000) {
    diva = B110;
  }
  else {
    diva = B111;
  }

  double freqK = (float) freqHz / f_pfd;
  pll_coefficient = (float) freqK  * powf(2, diva);
  n = floor(pll_coefficient);
  fractional = pll_coefficient - n;
  frac = round(m * fractional);

  reg0.bits.frac = frac;
  reg0.bits.n = n;
  reg1.bits.m = m;
  reg4.bits.diva = diva;
  //reg3.bits.mutedel = 1;  2871 only

  //setConfig(); CALL this after change some registers: freq + power + noisemdode ==> then setConfig()

  f_out_A = f_pfd * (reg0.bits.n + 1.0 * reg0.bits.frac / reg1.bits.m) / powf(2, reg4.bits.diva);

  /*
    Serial.print("reg0.bits.frac=");
    Serial.println(reg0.bits.frac, DEC);
    Serial.print("reg0.bits.n=");
    Serial.println(reg0.bits.n, DEC);
    Serial.print("reg1.bits.m=");
    Serial.println(reg1.bits.m, DEC);
    Serial.print("reg4.bits.diva=");
    Serial.println(reg4.bits.diva, DEC);
    Serial.print("f_out_A=");
    Serial.println(float(f_out_A / 1000));
  */

}

void MAX2870::setPFD(const uint64_t referenceFreqHz, const uint16_t rdiv) {
  // fPFD = (fREF/1M) * [(1 + DBR)/(R * (1 + RDIV2))]
  // DBR=0 RDIV2=0
  // fPFD = fREF * [1/R] = fREF / R
  //

  f_pfd = referenceFreqHz / rdiv;

  if (f_pfd > 32000000UL) {
    reg2.bits.lds = 1;
  }
  else {
    reg2.bits.lds = 0;
  }

  reg3.bits.cdiv = f_pfd / 100000UL; // round  (fHz/E6)/0.10 === /1 000 00    !!!E5

  reg2.bits.dbr = 0;
  reg2.bits.rdiv2 = 0;
  reg2.bits.r = rdiv;

  uint32_t bs = (f_pfd / 1000000UL) * 20;

  if (bs > 1023) {
    bs = 1023;
  }
  else if (bs < 1) {
    bs = 1;
  }

  reg4.bits.bs = 0x03FF & bs;
  reg4.bits.bs2 = 0x03 & (bs >> 8);

  setConfig();

  /*
    Serial.print("reg2.bits.lds=");
    Serial.println(reg2.bits.lds, BIN);
    Serial.print("reg3.bits.cdiv=");
    Serial.println(reg3.bits.cdiv, DEC);
    Serial.print("bs=");
    Serial.println(bs, DEC);
  */

}



uint64_t MAX2870::getPFD() {
  return f_pfd;
}

uint64_t MAX2870::get_frequency_OUT_A() {
  return f_out_A;
}

void MAX2870::pre_set_noiseMode() {
  noiseMode_idx = constrain(noiseMode_idx, 0, 2);
  reg2.bits.sdn = noiseMode[noiseMode_idx];  
}

void MAX2870::pre_set_power_OUT_A() {
  outPower_idx = constrain(outPower_idx, 0, 3);
  reg4.bits.apwr = outPower[outPower_idx];  
}

void MAX2870::pre_set_chargePumpCurrent() {
  chargePumpCurrent_idx = constrain(chargePumpCurrent_idx, 0, 15);
  reg2.bits.cp = chargePumpCurrent[chargePumpCurrent_idx];  
}


/*
  void MAX2870::setActive(bool isOn) {
  reg2.bits.shdn =  (isOn ? B0 : B1); //sd = shootDown
  reg4.bits.sdldo = (isOn ? B0 : B1);
  reg4.bits.sddiv = (isOn ? B0 : B1);
  reg4.bits.sdref = (isOn ? B0 : B1);
  //reg4.bits.sdvco = !isOn;  2871 only
  //reg5.bits.sdpll = !isOn;  2871 only

  setConfig();

  //digitalWrite(pin_RF_EN, (isOn ? 1 : 0));
  }
*/





