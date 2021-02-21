
//=====================================================================================================
//============================== HOWTO ================================================================
// SWEEP:  open COM-port at 115200. Send [freqMHz_from;freqMHz_to;]  [400;500;]  [5000;6000;]
// SWEEP: GET from COM-port data  [freqMHz;AnalogPort0;AnalogPort1;]  AnalogPort=0...1023

//=====================================================================================================


//===========================================MAX2870=============================================================
/*
  MOSI 11  3v3 ----> DATA
  MISO 12 3v3 ---- null, but d12 is SPI
  SCK 13  3v3 -----> CLK
  d10  3v3 no connect, but SPI SS. NOT use it for MAX2870_LE!

  My board doesnt have MAX2870_pin_RF_EN 3v3. And CE pin I hard connected to 3v3. My Arduino doesnt have enought pins to output all datas
  #define MAX2870_pin_CE   3v3    //init =1 //chip enable
  #define MAX2870_pin_RF_EN  3v3    //init =1 //RF output enable  PDBRF. RF Power-Down. A logic low on this pin mutes the RF outputs
*/
#include <SPI.h>
#include "MAX2870.h"
#define MAX2870_reference_frequency_Hz 100000000UL  //reference frequency 25-50-100MHz quartz  !!!! NOTE UL= only 4,294,967,295 == 4Ghz 294Mhz 967kHz 295Hz
#define MAX2870_R_divider 2                   //R divider to set phase/frequency detector comparison frequency. If reference oscill is 100MHZ, R=2 because in Fraction_mode F_PFD 50MHz max!
#define MAX2870_pin_LE 9    // 3v3 Load Enable Input. LE=0 write_one_register LE=1  register set to chip 3v3
#define MAX2870_pin_LD 8    //3v3 input for Lock detect 3v3 

MAX2870 MAX2870_my(MAX2870_pin_LE, MAX2870_pin_LD);

uint64_t MAX2870_OUT_A_frequency_target = 433920000; //Hz   store F for dec\inc dF
uint64_t MAX2870_OUT_A_frequency_real = 433000000; //Hz store real F from MAX2870 due to fraction-mode. I wish 433920000, but chip set F = 433920480
uint32_t MAX2870_step[6] = {
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000
};
uint8_t MAX2870_step_idx = 3;
String MAX2870_step_verb[6] = {"1k", "10k", "100k", "1M", "10M", "100M"};

uint8_t MAX2870_noiseMode_idx = 0; // 0 1 2
uint8_t MAX2870_outPower_idx = 0;  // 0 1 2 3
uint8_t MAX2870_chargePumpCurrent_idx = 0;// 0-15

String MAX2870_noiseMode_verb[3] = {"Noise", "Spur1", "Spur2"};
String MAX2870_outPower_verb[4] = {"-4", "-1", "+2", "+5"};

boolean MAX2870_LD_isOk = false;

#define MAX2870_freqHz_min  20000000
#define MAX2870_freqHz_max  6500000000


//========================================== INTERFACE ==========================================================
#define ENCODER_pin_A 2 //Пин прерывания
#define ENCODER_pin_B 3 //Любой другой пин 

//Периодически проверять ENCODER_interrupt_delta и делать действтия,потом ENCODER_interrupt_delta=0 и снова ждем вращения
//в прерываниях делать дела нельзя - слишком долго
volatile int ENCODER_interrupt_delta = 0;       //Счетчик оборотов.
volatile int ENCODER_interrupt_state = 0;       // Переменная хранящая статус вращения
volatile int ENCODER_interrupt_pin_A_val = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int ENCODER_interrupt_pin_B_val = 0;   // Переменные хранящие состояние пина, для экономии времени

#define BTN_ANALOG_IN A1
#define PORT_ANALOG_IN_0 A2
#define PORT_ANALOG_IN_1 A3

#define KEYBOARD_power 1
#define KEYBOARD_screen_next 2
#define KEYBOARD_step 3
#define KEYBOARD_noiseMode 4
#define KEYBOARD_cpc 5 //charge pump current

//=====================================1602 LCD i2c==============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
uint8_t MONITOR_screen_num = 0; //0..1


//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_next_101ms = 0L;
uint32_t TIMEMACHINE_next_311ms = 0L;
uint32_t TIMEMACHINE_next_2000ms = 0L;

//=======================================SYS=======================================================================
volatile boolean SYS_isNeedProcessConfig = true;

//SWEEP
String SERIAL_data = "";
boolean SERIAL_isDataReady = false;

// #define DBG 1

void setup() {
  Serial.begin(115200);
  MONITOR_init();
  
  MAX2870_my.start();
  MAX2870_my.setPFD(MAX2870_reference_frequency_Hz, MAX2870_R_divider);
  MAX2870_my.pre_set_frequency_OUT_A(433920000);
  MAX2870_my.pre_set_noiseMode(MAX2870_noiseMode_idx);
  MAX2870_my.pre_set_power_OUT_A(MAX2870_outPower_idx);
  MAX2870_my.pre_set_chargePumpCurrent(MAX2870_chargePumpCurrent_idx);
  MAX2870_my.setConfig();

  ENCODER_init();
  BUTTON_init();
  MONITOR_render();
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
