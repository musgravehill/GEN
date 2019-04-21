void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}



void MONITOR_render() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print((int)(ADF4351_frequency/100000));
  lcd.print("MHz ");
  lcd.print((ADF4351_frequency%100000)/100);
  lcd.print("kHz ");
 // lcd.print(ADF4351_frequency);

  lcd.setCursor(0, 1);
  lcd.print(OLED_stepsVariants_val[ADF4351_stepsVariantsNumCurrent]);
  lcd.print(' ');
  lcd.print(ADF4351_lowNoiseOrSpur_verb[ADF4351_lowNoiseOrSpur_current]);
  lcd.print(' ');
  lcd.print(ADF4351_outputPower_verb[ADF4351_outputPower_current]);
}
