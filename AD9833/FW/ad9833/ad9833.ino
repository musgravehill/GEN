/*
   MOSI 11 ----> DAT
   MISO 12 ----  
   SCK 13 -----> CLK 
   d10 ------  FSY AD9833
   d9 ------- CS  MCP41010

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
AD9833 AD9833_gen(AD9833_FSYNC); 
#define AD9833_DIGIPOT_CS 9 // MCP41010 chip select - digital potentiometer, then opAmp 
uint8_t AD9833_DIGIPOT_val = 0U;
 
uint32_t AD9833_referenceFreq = 2500000L; //*10 Hz reference frequency = quartz 25 MHz
uint32_t AD9833_frequency = 43300000L; //*10 Hz = 433 MHz
uint32_t AD9833_freqStepCurrent = 0L;
uint32_t AD9833_stepsVariants[7] = {
  625, //*10Hz 6,25 khz, 5khz does not work in Int-N mode (MOD> 4095) at 25Mhz Ref.
  1000, //*10Hz 10 khz
  1250, //*10Hz 12.5 khz
  10000, //*10Hz 100 khz
  100000, //*10Hz 1 Mhz
  1000000, //*10Hz 10 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
  10000000 //*10Hz 100 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
};
uint8_t AD9833_stepsVariantsNumCurrent = 4;
String OLED_stepsVariants_val[7] = {"6.25kHz", "10kHz", "12.5kHz", "100kHz", "1MHz", "10MHz", "100MHz"};

uint8_t AD9833_lowNoiseOrSpurVariants[2] = {B0, B11};
uint8_t AD9833_lowNoiseOrSpur_current = 0; //at lowSpur cannot lock sometimes
String AD9833_lowNoiseOrSpur_verb[2] = {"LN", "LS"};

uint8_t AD9833_outputPowerVariants[4] = {B0, B01, B10, B11};
uint8_t AD9833_outputPower_current = 0; //5dBm doesnot work, only -4 ... 2
String AD9833_outputPower_verb[4] = {"-4dBm", "-1dBm", "2dBm", "5dBm"};

 

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
#define BTN_future1 6
#define BTN_lownoisespur 7
#define BTN_out_power 8
#define LED_lock_detect 9

//=====================================1602 LCD i2c==============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


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


