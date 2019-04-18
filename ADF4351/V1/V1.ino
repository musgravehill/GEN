/*
  TODO
  btn4 -> sweep start (every 0.5s freq+=step; show on screen freq, sweep-mode)



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
   d10 no connect, but SPI
   d3----------> LE
   d5----------> LD = lock detect
                 CE---3V3



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
#define ADF4351_ss_pin 3 //SPI-SS enable ADF4351
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

//SWEEP, control by device BTN or serial from PC soft
uint32_t ADF4351_SWEEP_freq_from = 33 * 100000; //*10 = X MHz
uint32_t ADF4351_SWEEP_freq_to = 5000 * 100000; //*10 = X MHz
boolean ADF4351_SWEEP_isOn = false;

String SERIAL_data = "";
boolean SERIAL_isDataReady = false;
//========================================== INTERFACE ==========================================================
#define ENCODER_button 7
#define ENCODER_A 9
#define ENCODER_B 8
boolean ENCODER_A_state;
boolean ENCODER_B_state;
boolean ENCODER_A_state_prev = false;

#define BTN_step A3
#define BTN_lownoisespur A2
#define BTN_out_power A1
#define BTN_sweep A0
//boolean ADF4351_SWEEP_tmp = true;

#define LED_pin 6

uint32_t INTERFACE_action_prev_ms = 0L;

//OLED SDA A4, SCL A5
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL);
extern uint8_t SmallFont[]; //6*8
extern uint8_t MediumNumbers[]; //12*16
boolean OLED_blynk_state = false;

//FUTURE
//uint8_t INTERFACE_freq_multiplier_variants[3] = {1, 2, 4};
//uint8_t INTERFACE_freq_multiplier_current = 0;

//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_prev_5ms = 0L;
uint32_t TIMEMACHINE_prev_311ms = 0L;
uint32_t TIMEMACHINE_prev_101ms = 0L;

void setup() {
  ADF4351_init();
  OLED_init();
  ENCODER_init();
  BUTTON_init();
  Serial.begin(57600);
  analogReference(DEFAULT); // 3.3v
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






