
/*
  Предположим, вы хотите напечатать "number" в HEX:
  uint64_t number;
  unsigned long long1 = (unsigned long)((number & 0xFFFF0000) >> 16 );
  unsigned long long2 = (unsigned long)((number & 0x0000FFFF));
  String hex = String(long1, HEX) + String(long2, HEX); // six octets
*/



#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup() {
  uint64_t ADF4351_frequency = 5123456789;  //uint_32t max 4294967295

  uint16_t frq_GHz = ADF4351_frequency / 1000000000;
  uint16_t frq_MHz = (ADF4351_frequency % 1000000000) / 1000000;
  uint16_t frq_kHz = (ADF4351_frequency % 1000000) / 1000;
  uint16_t frq_Hz = (ADF4351_frequency % 1000);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(frq_GHz, DEC);
  lcd.setCursor(2, 0);
  lcd.print(frq_MHz, DEC);
  lcd.setCursor(6, 0);
  lcd.print(frq_kHz, DEC);
  lcd.setCursor(10, 0);
  lcd.print(frq_Hz, DEC);    
  
  lcd.setCursor(18, 0);
  lcd.print("Hz");

  lcd.setCursor(12, 1);
  lcd.print("STP 100k");

  lcd.setCursor(12, 2);
  lcd.print("LowNoise");

  lcd.setCursor(12, 3);
  lcd.print("PA +5dBm");

  lcd.setCursor(0, 2);
  lcd.print("A0=1023");
  lcd.setCursor(0, 3);
  lcd.print("A1=1023");

   
}


void loop() {
}
