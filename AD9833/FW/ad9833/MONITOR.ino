void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}


void MONITOR_render() {
  uint32_t frq_MHz = AD9833_frequency / 1000000;
  uint32_t frq_kHz = (AD9833_frequency % 1000000) / 1000;
  uint32_t frq_Hz = AD9833_frequency % 1000;


  lcd.clear();
  lcd.setCursor(0, 0);

  
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
  lcd.print(AD9833_step_verbs[AD9833_step_currPos]);
  lcd.setCursor(6, 1);
  lcd.print(AD9833_wavetype_verbs[AD9833_wavetype_currPos]);
  lcd.setCursor(11, 1);
  lcd.print("a");
  lcd.print(AD9833_DIGIPOT_val);
}
