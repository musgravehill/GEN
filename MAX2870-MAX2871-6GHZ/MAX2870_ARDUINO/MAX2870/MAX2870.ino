

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
 */  


#include <SPI.h>
#include "MAX2870.h" //is .h auto-connect .cpp ?  #include "MAX2870.cpp" cause error

#define MAX2870_reference_frequency_mhz 100.0  //reference frequency 25-50-100MHz quartz
#define MAX2870_R_divider 2                   //R divider to set phase/frequency detector comparison frequency. If reference oscill is 100MHZ, R=2 because in Fraction_mode F_PFD 50MHz max!
#define MAX2870_pin_LE 9                     //Load Enable Input. When LE goes high the data stored in the shift register is loaded into the appropriate latches.
#define MAX2870_pin_CE 8                      //init =1 //chip enable
#define MAX2870_pin_RF_EN 7                   //init =1 //RF output enable  PDBRF. RF Power-Down. A logic low on this pin mutes the RF outputs
#define MAX2870_pin_LD 6                      //input for Lock detect  


MAX2870 MAX2870_my(MAX2870_pin_LE, MAX2870_pin_CE, MAX2870_pin_RF_EN, MAX2870_pin_LD);

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  
  MAX2870_my.start();
  //MAX2870_my.setActive(true); 
  //MAX2870_my.setPFD(MAX2870_reference_frequency_mhz , MAX2870_R_divider); //inputs are reference frequency and R divider to set phase/frequency detector comparison frequency
  //MAX2870_my.set_OUT_A_frequency(433.92);  

  //double real_freq = MAX2870_my.get_OUT_A_frequency(); //реальная частота рассчитывается по формулам и может отличатьчя от установленной на килогерцы и т.п.
 // Serial.println(real_freq,6);
}

/*
TODO
reg2.SDN  
Sets noise mode 
00 = Low-noise mode
01 = Reserved
10 = Low-spur mode 1
11 = Low-spur mode 2

APWR[1:0] RFOUTA Output
Power 
Sets RFOUTA single-ended output power.   
00 = -4dBm
01 = -1dBm
10 = +2dBm
11 = +5dBm



reg2.CP Charge-Pump Current
Sets charge-pump current in mA (RSET = 5.1kI). Double buffered by register 0.
0000 = 0.32
0001 = 0.64
0010 = 0.96
0011 = 1.28
0100 = 1.60
0101 = 1.92
0110 = 2.24
0111 = 2.56 [ICP = 1.63/RSET x (1 + CP<3:0>)]
1000 = 2.88
1001 = 3.20
1010 = 3.52
1011 = 3.84
1100 = 4.16
1101 = 4.48
1110 = 4.80
1111 = 5.12

*/


void loop() {


}
