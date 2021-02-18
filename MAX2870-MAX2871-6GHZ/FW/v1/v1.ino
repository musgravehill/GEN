
/*
  Предположим, вы хотите напечатать "number" в HEX:
  uint64_t number;
  unsigned long long1 = (unsigned long)((number & 0xFFFF0000) >> 16 );
  unsigned long long2 = (unsigned long)((number & 0x0000FFFF));
  String hex = String(long1, HEX) + String(long2, HEX); // six octets
*/

//===========================================MAX2870=============================================================
#define MAX2870_pin_LE 9                     //Load Enable Input. LE=0 write_one_register LE=1  register set to chip
#define MAX2870_pin_LD 8                      //input for Lock detect 
uint64_t MAX2870_OUT_A_frequency_target = 433920000; //Hz   store F for dec\inc dF
uint64_t MAX2870_OUT_A_frequency_real = 433000000; //Hz  store real F from MAX2870 due to fraction-mode. I wish 433920000, but chip set F = 433920480
uint32_t MAX2870_step[6] = {
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000
};
uint8_t MAX2870_step_idx = 5;   
String MAX2870_step_verb[6] = {"1k", "10k", "100k", "1M", "10M", "100M"};
boolean MAX2870_LD_isOk = false;

#define MAX2870_freqMHz_min  20
#define MAX2870_freqMHz_max  6500


//========================================== INTERFACE ==========================================================
#define ENCODER_pin_A 2 //Пин прерывания
#define ENCODER_pin_B 3 //Любой другой пин 

//Периодически проверять ENCODER_interrupt_delta и делать действтия,потом ENCODER_interrupt_delta=0 и снова ждем вращения
//в прерываниях делать дела нельзя - слишком долго
volatile int ENCODER_interrupt_delta = 0;       //Счетчик оборотов.
volatile int ENCODER_interrupt_state = 0;       // Переменная хранящая статус вращения
volatile int ENCODER_interrupt_pin_A_val = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int ENCODER_interrupt_pin_B_val = 0;   // Переменные хранящие состояние пина, для экономии времени

#define ENCODER_button 4
#define BTN_ANALOG_IN A2
#define PORT_ANALOG_IN_0 A0
#define PORT_ANALOG_IN_1 A1



//=====================================1602 LCD i2c==============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_next_101ms = 0L;
uint32_t TIMEMACHINE_next_311ms = 0L;
uint32_t TIMEMACHINE_next_2000ms = 0L;

//=======================================SYS=======================================================================
volatile boolean SYS_isNeedProcessConfig = true;

//SWEEP
String SERIAL_data = "";
boolean SERIAL_isDataReady = false;


void setup() {
  MONITOR_init();

  //MAX2870_my.start();
  //MAX2870_my.setPFD(MAX2870_reference_frequency_Hz , MAX2870_R_divider);
  //MAX2870_my.pre_set_frequency_OUT_A(433920000);
  //MAX2870_my.setConfig();

  ENCODER_init();
  BUTTON_init();

  MONITOR_render();
  Serial.begin(115200);
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
