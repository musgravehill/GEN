#ifndef _MAX2870_H_
#define _MAX2870_H_

#include <Arduino.h>

/**
  @brief The MAX2870 is an ultra-wideband phase-locked loop (PLL) with integrated
  voltage control oscillators (VCOs)capable of operating in both integer-N and
  fractional-N modes. When combined with an external reference oscillator and
  loop filter, the MAX2870 is a high-performance frequency synthesizer capable
  of synthesizing frequencies from 23.5MHz to 6.0GHz while maintaining superior
  phase noise and spurious performance.

  SPI         spi(D11,D12,D13);           //mosi, miso, sclk
    Serial      pc(USBTX,USBRX,9600);       //tx, rx, baud

    DigitalOut  le(D10,1);                  //Latch enable pin for MAX2871
    DigitalIn   ld(D6);                     //Lock detect output pin
    DigitalOut  led(LED_BLUE,1);            //blue LED on MAX32600MBED board

    DigitalOut  rfouten(D8,1);              //RF output enable pin
    DigitalOut  ce(D9,1);                   //Chip enable pin

    double freq_entry;                  //variable to store user frequency input
    char buffer[256];                   //array to hold string input from terminal

    double v_tune, temperature;         //stores TUNE voltage and die temperature of MAX2871
    uint32_t vco;                       //stores active VCO in use
    double freq_rfouta;                 //variable to calculate ouput frequency from register settings

    spi.format(8,0);                    //CPOL = CPHA = 0, 8 bits per frame
    spi.frequency(1000000);             //1 MHz SPI clock

    MAX2871 max2871(spi,D10);           //create object of class MAX2871, assign latch enable pin

    max2871.setActive(true);              //set all hardware enable pins and deassert software shutdown bits

    max2871.setPFD(50.0,2);             //inputs are reference frequency and R divider to set phase/frequency detector comparison frequency

    //The routine in the while(1) loop will ask the user to input a desired
    //output frequency, check that it is in range, calculate the corresponding
    //register settings, update the MAX2871 registers, and then independently
    //use the programmed values to re-calculate the output frequency chosen
    while(1){
        pc.printf("\n\rEnter a frequency in MHz:");
        fgets(buffer,256,stdin);
         pc.printf("Line: %s\n", buffer);                          //store entry as string until newline entered
        freq_entry = floor(1000*atof(buffer))/1000;         //convert string to a float with 1kHz precision
        if((freq_entry < 23.5) || (freq_entry > 6000.0))    //check the entered frequency is in MAX2871 range
            pc.printf("\n\rNot a valid frequency entry.");
        else
        {
            pc.printf("\n\rTarget: %.3f MHz",freq_entry);   //report the frequency derived from user's input
            max2871.setRFOUTA(freq_entry);                  //update MAX2871 registers for new frequency

            while(!ld)                                      //blink an LED while waiting for MAX2871 lock detect signal to assert
            {
                led = !led;
                wait_ms(30);
            }
            led = 1;

            vco = max2871.readVCO();                        //read the active VCO from MAX2871
            v_tune = max2871.readADC();                     //read the digitized TUNE voltage
            freq_rfouta = max2871.getRFOUTA();              //calculate the output frequency of channel A
            temperature = max2871.readTEMP();               //read die temperature from MAX2871

            //print the achieved output frequency and MAX2871 diagnostics
            pc.printf("\n\rActual: %.3f MHz",freq_rfouta);
            pc.printf("\n\rVTUNE: %.3f V, VCO: %d, TEMP: %f",v_tune,vco,temperature);
        }
    }

  @code
  #include "mbed.h"
  #include <stdio.h>

  #include "MAX2870.h"

  SPI         spi(D11,D12,D13);           //mosi, miso, sclk
  Serial      pc(USBTX,USBRX,9600);       //tx, rx, baud

  DigitalOut  le(D10,1);                  //latch enable
  DigitalOut  ce(D9,1);                   //chip enable
  DigitalOut  rfout_en(D8,1);             //RF output enable

  int main() {
    float freq_entry;                   //frequency input to terminal
    float freq_actual;                  //frequency based on MAX2870 settings
    float freq_pfd;                     //frequency of phase frequency detector
    float pll_coefficient;              //fractional-N coefficient (N + F/M)
    float vco_divisor;                  //divisor from f_vco to f_out_A
    char buffer[256];                   //string input from terminal

    spi.format(8,0);                    //CPOL = CPHA = 0, 8 bits per frame
    spi.frequency(1000000);             //1 MHz SPI clock

    MAX2870 MAX2870(spi,D10);           //create object of class MAX2870

    //The routine in the while(1) loop will ask the user to input a desired
    //output frequency, calculate the corresponding register settings, update
    //the MAX2870 registers, and then independently use the programmed values
    //from the registers to re-calculate the output frequency chosen
    while(1){
        pc.printf("\n\rEnter a frequency in MHz:");
        fgets(buffer,256,stdin);        //store entry as string until newline entered
        freq_entry = atof (buffer);     //convert string to a float
        MAX2870.frequency(freq_entry);  //update MAX2870 registers for new frequency
        MAX2870.readRegister6();        //read register 6 and update MAX2870.reg6

        //Examples for how to calculate important operation parameters like
        //PFD frequency and divisor ratios using members of the MAX2870 class
        freq_pfd = MAX2870.f_reference*(1+MAX2870.reg2.bits.dbr)/(MAX2870.reg2.bits.r*(1+MAX2870.reg2.bits.rdiv2));
        pll_coefficient = (MAX2870.reg0.bits.n+1.0*MAX2870.reg0.bits.frac/MAX2870.reg1.bits.m);
        vco_divisor = powf(2,MAX2870.reg4.bits.diva);

        //calculate expected f_out_A based on the register settings
        freq_actual = freq_pfd*pll_coefficient/vco_divisor;
        pc.printf("\n\rTarget: %.3f MHz\tActual: %.3f MHz",freq_entry,freq_actual);
        pc.printf("\n\rDie: %d, VCO: %d, F_PFD: %f",MAX2870.reg6.bits.die,MAX2870.reg6.bits.v,freq_pfd);
        pc.printf("\n\rN: %d, F: %d, M: %d, N+F/M: %f",MAX2870.reg0.bits.n,MAX2870.reg0.bits.frac,MAX2870.reg1.bits.m,pll_coefficient);
    }

  }

  @endcode
*/
class MAX2870 {
  public:

    ///@brief MAX2870 Constructor
    MAX2870(const uint8_t MAX2870_pin_LE, const uint8_t MAX2870_pin_CE, const uint8_t MAX2870_pin_RF_EN, const uint8_t MAX2870_pin_LD);

    //MAX2870 Registers
    enum Registers_e
    {
      REG0          = 0x00,
      REG1          = 0x01,
      REG2          = 0x02,
      REG3          = 0x03,
      REG4          = 0x04,
      REG5          = 0x05,
      REG6          = 0x06
    };

    //Register 0 bits
    union REG0_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr       : 3;
        uint32_t frac       : 12;
        uint32_t n          : 16;
        uint32_t intfrac    : 1;
      } bits;
    };

    //Register 1 bits
    union REG1_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr         : 3;
        uint32_t m            : 12;
        uint32_t p            : 12;
        uint32_t cpt          : 2;
        uint32_t cpl          : 2;
        uint32_t cpoc         : 1;
      } bits;
    };

    //Register 2 bits
    union REG2_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr      : 3;
        uint32_t rst       : 1;
        uint32_t tri       : 1;
        uint32_t shdn      : 1;
        uint32_t pdp       : 1;
        uint32_t ldp       : 1;
        uint32_t ldf       : 1;
        uint32_t cp        : 4;
        uint32_t reg4db    : 1;
        uint32_t r         : 10;
        uint32_t rdiv2     : 1;
        uint32_t dbr       : 1;
        uint32_t mux       : 3;
        uint32_t sdn       : 2;
        uint32_t lds       : 1;
      } bits;
    };

    //Register 3 bits
    union REG3_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr      : 3;
        uint32_t cdiv      : 12;
        uint32_t cdm       : 2;
        uint32_t reserved1 : 1;
        uint32_t reserved2 : 1;
        uint32_t reserved3 : 5;
        uint32_t retune    : 1;
        uint32_t vas_shdn  : 1;
        uint32_t vco       : 6;
      } bits;
    };

    //Register 4 bits
    union REG4_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr        : 3;
        uint32_t apwr        : 2;
        uint32_t rfa_en      : 1;
        uint32_t bpwr        : 2;
        uint32_t rfb_en      : 1;
        uint32_t bdiv        : 1;
        uint32_t reservered1 : 1;
        uint32_t reservered2 : 1;
        uint32_t bs          : 8;
        uint32_t diva        : 3;
        uint32_t fb          : 1;
        uint32_t bs2         : 2;
        uint32_t sdref       : 1;
        uint32_t sddiv       : 1;
        uint32_t sdldo       : 1;
        uint32_t reservered3 : 3;
      } bits;
    };

    //Register 5 bits
    union REG5_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr      : 3;
        uint32_t reserved1 : 15;
        uint32_t mx3       : 1;
        uint32_t reserved2 : 3;
        uint32_t ld        : 2;
        uint32_t f01       : 1;
        uint32_t reserved4 : 7;

      } bits;
    };

    //Register 6 bits
    union REG6_u
    {
      //Access all bits
      uint32_t all;

      //Access individual bits
      struct BitField_s
      {
        uint32_t addr      : 3;
        uint32_t v         : 6;
        uint32_t vasa      : 1;  //max2871?
        uint32_t reserved1 : 5;  //max2871?
        uint32_t adcv      : 1;  //max2871?
        uint32_t adc       : 7;  //max2871?
        uint32_t por       : 1;  //max2871?
        uint32_t reserved2 : 4;  //max2871?
        uint32_t die       : 4;
      } bits;
    };


    ///@brief Writes raw 32-bit data pattern. The MAX2870 accepts 32-bit words at a time; 29 data bits and 3 address bits.
    ///@param[in] data - 32-bit word to write to the MAX2870. Bits[31:3] contain the register data, and Bits[2:0] contain the register address.
    void writeData(uint32_t data);
    void setConfig();

    void start();
    void setActive(bool isOn);
    void setPFD(double referenceFreqMHz, uint16_t rdiv);
    void set_frequency_OUT_A(double freqMHz);
    void set_power_OUT_A();
    void set_noiseMode();
    void set_chargePumpCurrent();

    double getPFD();
    double get_frequency_OUT_A();
    
    //config data
    uint8_t noiseMode[3] = {B0, B10, B11};
    uint8_t noiseMode_idx = 0; // 0 1 2
    String noiseMode_verb[3] = {"N", "S1", "S2"};

    uint8_t outPower[4] = {B0, B01, B10, B11};
    uint8_t outPower_idx = 0;  // 0 1 2 3
    int8_t outPower_verb[4] = { -4, -1, 2, 5};

    uint8_t chargePumpCurrent[16] = {B0000, B0001, B0010, B0011, B0100, B0101, B0110, B0111, B1000, B1001, B1010, B1011, B1100, B1101, B1110, B1111};
    uint8_t chargePumpCurrent_idx = 0;// 0-15

  private:

    REG0_u reg0;
    REG1_u reg1;
    REG2_u reg2;
    REG3_u reg3;
    REG4_u reg4;
    REG5_u reg5;
    REG6_u reg6;

    double f_pfd;
    double f_out_A; //frequency at OUT A

    uint8_t pin_LE;
    uint8_t pin_CE; //Chip Enable. 0 = powerDown
    uint8_t pin_RF_EN;  //RF Output Enable. 0 = powerDown
    uint8_t pin_LD; // OUT  1=lockDetect ok.
};

#endif /* _MAX2870_H_ */


/*
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


