/*
PINOUT!!!! d2 - for ENCODER_A!!


ENCODER_count меняется в прерывании. ENCODER_count проверять в СтаетМашин каждые 300мс и менять частоту, ENCODER_count=0 устанавливать. 
Экран отрисовывать каждые 10сек и по событиям, рисовать экран каждые 300мс нет смысла.
i2c LCD 5V, а система 3.3В. Поэтому нужен преобразователь уровней и 2 питания: 5В и 3.3В.



 
*/


//============================================   3.3V ONLY ===================================================================================================================
//============================================   3.3V ONLY ===================================================================================================================
//============================================   3.3V ONLY ===================================================================================================================
//============================================   3.3V ONLY ===================================================================================================================

// ADF4351 PLL-Synthesizer 33Mhz - 4,4Ghz
// Integer mode Demo OE6OCG 1/2015
// up/down = Freq +- with stepsize
// left = frequency step's from 6.25khz to 1Mhz
/*
         ============================================   3.3V ONLY ===================================================================================================================
         ============================================   3.3V ONLY ===================================================================================================================
         ============================================   3.3V ONLY ===================================================================================================================
         ============================================   3.3V ONLY ===================================================================================================================
         ============================================   3.3V ONLY ===================================================================================================================
         ============================================   3.3V ONLY ===================================================================================================================
   MOSI 11 ----> DATA
   MISO 12 ---- null, but d12 is SPI
   SCK 13 -----> CLK
   d10 no connect, but SPI SS. CE connected to 3.3V, so SPI always enabled. I use custom LE pin for custom SPI interface
   d3----------> LE  LOAD_ENABLE 1=upload registers 0=work
   d5----------> LD = lock detect, 1=ok 0=cannot_generate
                 CE---3V3 SET CHIP_ENABLE ON



    ENCODER add
    if Freq>4.4GHz stop ++
    if Freq < 35Mhz stop --


     int8_t      |  char                       |   РѕС‚ -128 РґРѕ 127
  uint8_t    |  byte, unsigned char |   РѕС‚ 0 РґРѕ 255
  int16_t    |  int                          |   РѕС‚ -32768 РґРѕ 32767
  uint16_t  |  unsigned int, word   |   РѕС‚ 0 РґРѕ 65535

  int32_t    |  long                        |  РѕС‚  -2147483648 РґРѕ 2147483647
  uint32_t  |  unsigned long          |  РѕС‚ 0 РґРѕ 4294967295

*/

//============================================================= ADF4351 =========================================
#include <SPI.h>
#define ADF4351_LE_pin 3  
#define LD_pin 5
uint32_t ADF4351_referenceFreq = 2500000L; //*10 Hz reference frequency = quartz 25 MHz
uint32_t ADF4351_frequency = 43300000L; //*10 Hz = 433 MHz
uint32_t ADF4351_freqStepCurrent = 0L;
uint32_t ADF4351_stepsVariants[7] = {
  625, //*10Hz 6,25 khz, 5khz does not work in Int-N mode (MOD> 4095) at 25Mhz Ref.
  1000, //*10Hz 10 khz
  1250, //*10Hz 12.5 khz
  10000, //*10Hz 100 khz
  100000, //*10Hz 1 Mhz
  1000000, //*10Hz 10 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
  10000000 //*10Hz 100 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
};
uint8_t ADF4351_stepsVariantsNumCurrent = 4;
String OLED_stepsVariants_val[7] = {"6.25", "10", "12.5", "100", "1", "10", "100"};
String OLED_stepsVariants_kmhz[7] = {"kHz", "kHz", "kHz", "kHz", "MHz", "MHz", "MHz"};

uint8_t ADF4351_lowNoiseOrSpurVariants[2] = {B0, B11};
uint8_t ADF4351_lowNoiseOrSpur_current = 0; //at lowSpur cannot lock sometimes
String ADF4351_lowNoiseOrSpur_verb[2] = {"LOW-NOISE-MODE", "LOW-SPUR-MODE"};

//uint8_t ADF4351_outputPowerVariants[4] = {B0, B01, B10, B11};
//uint8_t ADF4351_outputPower_current = 0; //5dBm doesnot work, only -4 ... 2
//String ADF4351_outputPower_verb[4] = {"-4", "-1", "2", "5"};

uint32_t ADF4351_registers[6]; //ADF4351 Registers, see datasheet

boolean ADF4351_isNeedSetNewConfig = false;


//========================================== INTERFACE ==========================================================
#define ENCODER_pin_A 2 //Пин прерывания
#define ENCODER_pin_B 7 //Любой другой пин 

volatile int ENCODER_count = 0;       // Счетчик оборотов. Периодически проверять ENCODER_count и делать действтия,потом ENCODER_count=0 и снова ждем вращения
// в прерываниях делать дела нельзя - слишком долго
volatile int ENCODER_state = 0;       // Переменная хранящая статус вращения

volatile int ENCODER_pin_A_val = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int ENCODER_pin_B_val = 0;   // Переменные хранящие состояние пина, для экономии времени


#define ENCODER_button 8
#define BTN_step A3
#define BTN_lownoisespur A2
#define BTN_out_power A1

//=====================================1602 LCD i2c==============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_prev_5ms = 0L;
uint32_t TIMEMACHINE_prev_311ms = 0L;
uint32_t TIMEMACHINE_prev_101ms = 0L;

void setup() {
  ADF4351_init();
  MONITOR_init();
  ENCODER_init();
  BUTTON_init();
}

void loop() {
  TIMEMACHINE_loop();
}


