/*
  LED LOCKDETECT do

   ////--------------remove  pullup if user hardware resistors
  PINOUT!!!! d2 - for ENCODER_A!!


  ENCODER_interrupt_delta меняется в прерывании. ENCODER_interrupt_delta проверять в СтаетМашин каждые 300мс и менять частоту, ENCODER_interrupt_delta=0 устанавливать.
  Экран отрисовывать каждые 10сек и по событиям, рисовать экран каждые 300мс нет смысла.
  i2c LCD 5V, а система 3.3В. Поэтому нужен преобразователь уровней и 2 питания: 5В и 3.3В.

*/

//=====================================================================================================
//============================== HOWTO ================================================================
// SWEEP:  open COM-port at 115200. Send [freqMHz_from;freqMHz_to;]  [400;500;]  [5000;6000;]
// SWEEP: GET from COM-port data  [freqMHz;AnalogPort0;AnalogPort1;]  AnalogPort=0...1023


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
#define ADF4351_LE_pin A3
#define ADF4351_LD_pin A2
uint32_t ADF4351_referenceFreq = 2500000L; //*10 Hz reference frequency = quartz 25 MHz
uint32_t ADF4351_frequency = 43300000L; //*10 Hz = 433 MHz
uint32_t ADF4351_freqStepCurrent = 0L;
uint32_t ADF4351_stepsVariants[6] = {
  625, //*10Hz 6,25 khz, 5khz does not work in Int-N mode (MOD> 4095) at 25Mhz Ref.
  1000, //*10Hz 10 khz
  10000, //*10Hz 100 khz
  100000, //*10Hz 1 Mhz
  1000000, //*10Hz 10 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
  10000000 //*10Hz 100 Mhz //only for fast inc\dec by encoder. ADF cannot LOCK at this freq-step
};
uint8_t ADF4351_stepsVariantsNumCurrent = 3;
String OLED_stepsVariants_val[6] = {"6.25kHz", "10kHz", "100kHz", "1MHz", "10MHz", "100MHz"};

uint8_t ADF4351_lowNoiseOrSpurVariants[2] = {B0, B11};
uint8_t ADF4351_lowNoiseOrSpur_current = 0; //at lowSpur cannot lock sometimes
String ADF4351_lowNoiseOrSpur_verb[2] = {"LN", "LS"};

uint8_t ADF4351_outputPowerVariants[4] = {B0, B01, B10, B11};
uint8_t ADF4351_outputPower_current = 0; //5dBm doesnot work, only -4 ... 2
String ADF4351_outputPower_verb[4] = {"-4dBm", "-1dBm", "2dBm", "5dBm"};

uint32_t ADF4351_registers[6]; //ADF4351 Registers, see datasheet

#define ADF4351_freqMHz_min  30
#define ADF4351_freqMHz_max  5000

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
#define BTN_screen2 6
#define BTN_lownoisespur 7
#define BTN_out_power 8
#define LED_lock_detect 9

#define PORT_ANALOG_IN_0 A0

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

//SWEEP
String SERIAL_data = "";
boolean SERIAL_isDataReady = false;

void setup() {
  Serial.begin(115200);
  
  ADF4351_init();
  MONITOR_init();
  ENCODER_init();
  BUTTON_init();  
}

void loop() {
  TIMEMACHINE_loop();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '[') {
      SERIAL_data = "";
      //SERIAL_isDataReady = false;
    }
    else if (inChar == ']') {
      SERIAL_isDataReady = true;
    }
    else {
      SERIAL_data += inChar;
      //SERIAL_isDataReady = false;
    }
  }
}
