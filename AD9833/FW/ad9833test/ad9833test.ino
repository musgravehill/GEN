//AD9833 use SPI.setDataMode(SPI_MODE2);
//MCP41010 use the same??????

/*
   MOSI 11 ----> DAT
   MISO 12 ----
   SCK 13 -----> CLK
   d10 ------  FSY AD9833
   A6 ------- CS  MCP41010

  CS SPI Chip select to MCP41010
  DAT SPI MOSI to AD9833 and MCP41010    d11
  CLK SPI Clock to AD9833 and MCP41010   d13
  FSY SPI Chip select for AD9833         d10
  GND 0V
  VCC 3V ~ 5V

*/

//============================================================= AD9833 =========================================
#include <SPI.h>
#include <AD9833.h>

#define AD9833_DATA  11
#define AD9833_CLK   13
#define AD9833_FSYNC 10
AD9833 AD9833_gen(AD9833_FSYNC, 25000000UL);  //referenceFrequency oscillator
#define AD9833_DIGIPOT_CS A6          // MCP41010 chip select - digital potentiometer, then opAmp 

int32_t AD9833_DIGIPOT_val = 255;

int32_t AD9833_frequency = 1000L; //1kHz


   
void setup() {
  AD9833_frequency = 1000000L; //1MHz
  AD9833_init();
  AD9833_setConfig(); 
}

void loop() {
  
}


