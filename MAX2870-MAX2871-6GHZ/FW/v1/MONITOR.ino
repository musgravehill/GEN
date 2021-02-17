void MONITOR_init() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SYSTEM START...");
}



void MONITOR_render() {
  lcd.clear();
  uint16_t frq_GHz = MAX2870_OUT_A_frequency_real / 1000000000;
  uint16_t frq_MHz = (MAX2870_OUT_A_frequency_real % 1000000000) / 1000000;
  uint16_t frq_kHz = (MAX2870_OUT_A_frequency_real % 1000000) / 1000;
  uint16_t frq_Hz = (MAX2870_OUT_A_frequency_real % 1000);

  lcd.setCursor(0, 0);
  lcd.print(frq_GHz, DEC);
  lcd.setCursor(2, 0);
  lcd.print(frq_MHz, DEC);
  lcd.setCursor(6, 0);
  lcd.print(frq_kHz, DEC);
  lcd.setCursor(10, 0);
  lcd.print(frq_Hz, DEC);

 /* lcd.setCursor(18, 0);
  lcd.print("Hz");

  lcd.setCursor(13, 1);
  lcd.print("<> 100k");

  lcd.setCursor(12, 2);
  lcd.print("LowNoise");

  lcd.setCursor(12, 3);
  lcd.print("PA +5dBm");

  lcd.setCursor(0, 2);
  lcd.print("A0=1023");
  lcd.setCursor(0, 3);
  lcd.print("A1=1023");*/
}
