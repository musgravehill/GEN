//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print((float)4444.004, 3);
  lcd.print(" MHz");


  lcd.setCursor(0, 1);
  lcd.print("NOT_LOCK");
  lcd.print(" STEP_100kHz");
  lcd.setCursor(0, 2);
  lcd.print("LOW_NOISE  5dBm");

}


void loop()
{

}

