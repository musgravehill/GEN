//AD9833 use SPI.setDataMode(SPI_MODE2);
//MCP41010 use the same??????

/*
   MOSI 11 ----> DAT
   MISO 12 ----
   SCK 13 -----> CLK
   d10 ------  FSY AD9833
   A0 ------- CS  MCP41010

  CS SPI Chip select to MCP41010         A0
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
#define AD9833_DIGIPOT_CS A0          // MCP41010 chip select - digital potentiometer, then opAmp 

int32_t AD9833_DIGIPOT_val = 255;

int32_t AD9833_frequency = 1000L; //1kHz

uint32_t AD9833_step_vals[6] = {
  1UL,
  100UL,
  1000UL,
  10000UL,
  100000UL,
  1000000UL
};
uint8_t AD9833_step_currPos = 2;
String AD9833_step_verbs[6] = {"1Hz", "100Hz", "1k", "10k", "100k", "1M"};

WaveformType  AD9833_wavetype_vals[4] = {SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, HALF_SQUARE_WAVE};
uint8_t AD9833_wavetype_currPos = 0;
String AD9833_wavetype_verbs[4] = {"SINE", "TRI", "SQR", "HSQR"};

#define AD9833_encoder_state_menus_freq 0
#define AD9833_encoder_state_menus_digipot 1
uint8_t AD9833_encoder_state_menus = AD9833_encoder_state_menus_freq; //inc\dec frequency, digipot val, etc by encoder rotation

//========================================== INTERFACE ==========================================================
#define ENCODER_pin_A 2 //Пин прерывания
#define ENCODER_pin_B 3 //Любой другой пин 

volatile int ENCODER_interrupt_delta = 0;       // Счетчик оборотов. Периодически проверять ENCODER_interrupt_delta и делать действтия,потом ENCODER_interrupt_delta=0 и снова ждем вращения
// в прерываниях делать дела нельзя - слишком долго
volatile int ENCODER_interrupt_state = 0;       // Переменная хранящая статус вращения
volatile int ENCODER_interrupt_pin_A_val = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int ENCODER_interrupt_pin_B_val = 0;   // Переменные хранящие состояние пина, для экономии времени

#define ENCODER_button 4
#define BTN_future0 5
#define BTN_future1 8
#define BTN_wavetype 7
#define BTN_digipot 5


//=====================================1602 LCD i2c==============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2); //0x3F  0x27
bool lcd_blink_tmp = true;


//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_next_101ms = 0L;
uint32_t TIMEMACHINE_next_311ms = 0L;
//uint32_t TIMEMACHINE_next_2000ms = 0L;

//=======================================SYS=======================================================================
volatile boolean SYS_isNeedProcessConfig = true;

void setup() {
  AD9833_init();
  MONITOR_init();
  ENCODER_init();
  BUTTON_init();
  // Serial.begin(9600);
}

void loop() {
  TIMEMACHINE_loop();
}
