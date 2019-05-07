//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  uint32_t ADF4351_frequency = 243300000L; //*10 Hz = 2.433 GHz
  uint16_t f_m = ADF4351_frequency / 100000L; //*10Hz
  uint32_t f_k =  ADF4351_frequency % 100000; //*10Hz

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(f_m);
  lcd.print(".");
  lcd.print(f_k,5);
  lcd.print(" MHz");
  lcd.setCursor(0, 1);
  lcd.print("6.25kHz");
  lcd.print(" LN -4dBm");

  /*lcd.init();                      // initialize the lcd
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print((float)12000.004, 3);
    lcd.print("kHz");
    lcd.print(" SIN");
    lcd.setCursor(0, 1);
    lcd.print("6.25kHz");
    lcd.print("  amp256");*/

}


void loop()
{

}

