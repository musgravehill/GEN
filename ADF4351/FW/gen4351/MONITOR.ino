void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}



void MONITOR_render() {
  SYS_isNeedRender = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ENCODER_interrupt_count);

  lcd.setCursor(0, 1);
  lcd.print(OLED_stepsVariants_val[ADF4351_stepsVariantsNumCurrent]);
  lcd.print(' ');
  lcd.print(ADF4351_lowNoiseOrSpur_verb[ADF4351_lowNoiseOrSpur_current]);
  lcd.print(' ');
  lcd.print(ADF4351_outputPower_verb[ADF4351_outputPower_current]);
}
