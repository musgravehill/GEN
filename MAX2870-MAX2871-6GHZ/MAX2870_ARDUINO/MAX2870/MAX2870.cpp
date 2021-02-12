

#include <SPI.h>
#include "MAX2870.h"
#include <math.h>
#include <stdio.h>

//****************************************************************************
MAX2870::MAX2870(const uint8_t MAX2870_pin_LE, const uint8_t MAX2870_pin_CE, const uint8_t MAX2870_pin_RF_EN) {
  pin_LE = MAX2870_pin_LE;
  pin_CE = MAX2870_pin_CE;
  pin_RF_EN = MAX2870_pin_RF_EN;

  reg0.all = 0x007d0000;
  reg1.all = 0x2000fff9;
  reg2.all = 0x00004042;
  reg3.all = 0x0000000b;
  reg4.all = 0x6180b23c;
  reg5.all = 0x00400005;
  reg6.all = 0x00000000;

  updateAll();

  delay(20);

  updateAll();
}



//****************************************************************************
void MAX2870::write(const uint32_t data) {
  //TODO  m_le = 0;
  SPI.transfer((0xFF000000 & data) >> 24);  //????  в таком порядке или обратном????
  SPI.transfer((0x00FF0000 & data) >> 16);
  SPI.transfer((0x0000FF00 & data) >> 8);
  SPI.transfer( 0x000000FF & data);
  //TODO  m_le = 1;
}

//****************************************************************************
void MAX2870::updateAll()
{
  write(reg5.all);
  write(reg4.all);
  write(reg3.all);
  write(reg2.all);
  write(reg1.all);
  write(reg0.all);
}



//****************************************************************************
void MAX2870::setRFOUTA(const double freq)
{
  uint32_t n, frac, m, diva = 0;
  double pll_coefficient, fractional = 0;

  //double f_pfd = getPFD(); //GOT IT FROM main code???

  while (freq * powf(2, diva) < 3000.0)
  {
    diva = diva + 1;
  }
  pll_coefficient = freq * powf(2, diva) / f_pfd;
  n = floor(pll_coefficient);

  fractional = pll_coefficient - n;
  m = 4000;
  frac = round(m * fractional);

  reg0.bits.frac = frac;
  reg0.bits.n = n;
  reg1.bits.m = m;
  reg4.bits.diva = diva;

  reg3.bits.mutedel = 1;

  updateAll();
  f_rfouta = f_pfd * (reg0.bits.n + 1.0 * reg0.bits.frac / reg1.bits.m) / powf(2, reg4.bits.diva);
}

void MAX2870::setPFD(const double ref_in, const uint16_t rdiv)
{
  f_pfd = ref_in / rdiv;

  if (f_pfd > 32.0)
    reg2.bits.lds = 1;
  else reg2.bits.lds = 0;

  reg3.bits.cdiv = round(f_pfd / 0.10);

  reg2.bits.dbr = 0;
  reg2.bits.rdiv2 = 0;
  reg2.bits.r = rdiv;

  uint32_t bs = f_pfd * 20;

  if (bs > 1023)
    bs = 1023;
  else if (bs < 1)
    bs = 1;

  reg4.bits.bs = 0x03FF & bs;
  reg4.bits.bs2 = 0x03 & (bs >> 8);

  updateAll();
}



void MAX2870::powerOn(const bool pwr)
{
  reg2.bits.shdn =  !pwr;
  reg4.bits.sdldo = !pwr;
  reg4.bits.sddiv = !pwr;
  reg4.bits.sdref = !pwr;
  reg4.bits.sdvco = !pwr;
  reg5.bits.sdpll = !pwr;

  updateAll();
}








