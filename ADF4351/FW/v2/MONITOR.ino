void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}

void MONITOR_onSweep() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SWEEP");
}

void MONITOR_screen_next() {
  MONITOR_screen_num++;
  if (MONITOR_screen_num > 1) {
    MONITOR_screen_num = 0;
  }
  SYS_isNeedProcessConfig = true;
}

void MONITOR_render() {
  uint32_t frq_MHz = ADF4351_frequency / 100000;
  uint32_t frq_kHz = (ADF4351_frequency % 100000) / 100;
  uint32_t frq_Hz = (ADF4351_frequency % 100) * 10;


  lcd.clear();
  lcd.setCursor(0, 0);

  if (frq_MHz < 1000) {
    lcd.print(' ');
  }
  if (frq_MHz < 100) {
    lcd.print(' ');
  }
  if (frq_MHz < 10) {
    lcd.print(' ');
  }
  lcd.print(frq_MHz);
  lcd.print(".");

  if (frq_kHz < 100) {
    lcd.print(0);
  }
  if (frq_kHz < 10) {
    lcd.print(0);
  }
  lcd.print(frq_kHz);
  lcd.print(".");

  if (frq_Hz < 100) {
    lcd.print(0);
  }
  if (frq_Hz < 10) {
    lcd.print(0);
  }
  lcd.print(frq_Hz);
  lcd.print("  Hz");

  lcd.setCursor(0, 1);
  if (MONITOR_screen_num == 0) {
    lcd.print(OLED_stepsVariants_val[ADF4351_stepsVariantsNumCurrent]);
  }   

  lcd.setCursor(8, 1);
  lcd.print(ADF4351_lowNoiseOrSpur_verb[ADF4351_lowNoiseOrSpur_current]);
  lcd.setCursor(11, 1);
  lcd.print(ADF4351_outputPower_verb[ADF4351_outputPower_current]);
}

void MONITOR_periodical() {
  switch (MONITOR_screen_num) {
    case 0:
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("A0=    ");
      lcd.setCursor(3, 1);
      lcd.print(PORT_ANALOG_IN_get(PORT_ANALOG_IN_0));
      break;
  }
}
