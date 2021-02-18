

/*
  include <> ""
  angle brackets (<>) causes the compiler to search the default include directory.
  Double quotes ("") causes it to search the current working directory and, if that search fails, it defaults to the default include directory.
*/

// ========== 3v3 ======== generator 3v3 logic ====== 3v3 =========
// ========== 3v3 ======== generator 3v3 logic ====== 3v3 =========
// ========== 3v3 ======== generator 3v3 logic ====== 3v3 =========
// ========== 3v3 ======== generator 3v3 logic ====== 3v3 =========
// ======== USE LOGIC LEVEL CONVERTER between Arduino and MAX2870 ==========
// ======== USE LOGIC LEVEL CONVERTER between Arduino and MAX2870 ==========
// ======== USE LOGIC LEVEL CONVERTER between Arduino and MAX2870 ==========


/*
  MOSI 11 ----> DATA
  MISO 12 ---- null, but d12 is SPI
  SCK 13 -----> CLK
  d10 no connect, but SPI SS. NOT use it for MAX2870_LE!

  My board doesnt have MAX2870_pin_RF_EN. And CE pin I hard connected to 3v3. My Arduino doesnt have enought pins to output all datas
  #define MAX2870_pin_CE      //init =1 //chip enable
  #define MAX2870_pin_RF_EN     //init =1 //RF output enable  PDBRF. RF Power-Down. A logic low on this pin mutes the RF outputs
*/

#include <SPI.h>
#include "MAX2870.h" //is .h auto-connect .cpp ?  #include "MAX2870.cpp" cause error

#define MAX2870_reference_frequency_Hz 100000000UL  //reference frequency 25-50-100MHz quartz  !!!! NOTE UL= only 4,294,967,295 == 4Ghz 294Mhz 967kHz 295Hz
#define MAX2870_R_divider 2                   //R divider to set phase/frequency detector comparison frequency. If reference oscill is 100MHZ, R=2 because in Fraction_mode F_PFD 50MHz max!
#define MAX2870_pin_LE 9                     //Load Enable Input. When LE goes high the data stored in the shift register is loaded into the appropriate latches.
#define MAX2870_pin_LD 8                      //input for Lock detect  

MAX2870 MAX2870_my(MAX2870_pin_LE, MAX2870_pin_LD);

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");

  MAX2870_my.start();
  MAX2870_my.setPFD(MAX2870_reference_frequency_Hz , MAX2870_R_divider);
  
  MAX2870_my.set_frequency_OUT_A(433920000);
  /*
    MAX2870_my.outPower_idx = 2; //+2dBm
    MAX2870_my.set_power_OUT_A();

    MAX2870_my.noiseMode_idx = 1; //los spur #1
    MAX2870_my.set_noiseMode();

    MAX2870_my.chargePumpCurrent_idx = 7; //B0111=2.56mA
    MAX2870_my.set_chargePumpCurrent();*/


  uint64_t real_freq = MAX2870_my.get_frequency_OUT_A(); //реальная частота рассчитывается по формулам и может отличатьчя от установленной на килогерцы и т.п.
  Serial.print("real_freq=");
  Serial.println(float(real_freq/1),6);

}
void loop() {


}

/*  TEST
  Serial.println("\r\n 4000");
  MAX2870_my.set_frequency_OUT_A(4000000000);
  Serial.println("\r\n 2000");
  MAX2870_my.set_frequency_OUT_A(2000000000);
  Serial.println("\r\n 1000");
  MAX2870_my.set_frequency_OUT_A(1000000000);
  Serial.println("\r\n 433");
  MAX2870_my.set_frequency_OUT_A(433000000);
  Serial.println("\r\n 100");
  MAX2870_my.set_frequency_OUT_A(100000000);
  Serial.println("\r\n 70");
  MAX2870_my.set_frequency_OUT_A(70000000);
  Serial.println("\r\n 30");
  MAX2870_my.set_frequency_OUT_A(30000000);
  Serial.println("\r\n 433.92");
  MAX2870_my.set_frequency_OUT_A(433920000);
  Serial.println("\r\n 5665");
  MAX2870_my.set_frequency_OUT_A(5665000000);
  Serial.println("\r\n 434.600");
  MAX2870_my.set_frequency_OUT_A(434600000);

*/



