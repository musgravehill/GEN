void MONITOR_init() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(F("START..."));

  lcd.setCursor(14, 0);
  lcd.print(F("Hz"));

  switch (MONITOR_screen_num) {
    case 0:
      MONITOR_screen_0_init();
      break;
    case 1:
      MONITOR_screen_1_init();
      break;
  }
}

void MONITOR_screen_next() {
  MONITOR_screen_num++;
  if (MONITOR_screen_num > 1) {
    MONITOR_screen_num = 0;
  }

  switch (MONITOR_screen_num) {
    case 0:
      MONITOR_screen_0_init();
      break;
    case 1:
      MONITOR_screen_1_init();
      break;
  }
}

void MONITOR_screen_0_init() {
  lcd.setCursor(0, 2);
  lcd.print(F("Low           <    >"));
  lcd.setCursor(0, 3);
  lcd.print(F("CP=              dBm"));
}

void MONITOR_screen_1_init() {
  lcd.setCursor(0, 2);
  lcd.print(F("A1=                 "));
  lcd.setCursor(0, 3);
  lcd.print(F("A2=                 "));
}

void MONITOR_render() {
  MAX2870_OUT_A_frequency_real = MAX2870_my.get_frequency_OUT_A();
  uint16_t frq_GHz = MAX2870_OUT_A_frequency_real / 1000000000;
  uint16_t frq_MHz = (MAX2870_OUT_A_frequency_real % 1000000000) / 1000000;
  uint16_t frq_kHz = (MAX2870_OUT_A_frequency_real % 1000000) / 1000;
  uint16_t frq_Hz = (MAX2870_OUT_A_frequency_real % 1000);

  lcd.setCursor(0, 0);
  if (frq_GHz > 0) {
    lcd.print(frq_GHz);
  } else {
    lcd.print(' ');
  }
  lcd.print(' ');
  if (frq_MHz < 100) {
    lcd.print(0);
  }
  if (frq_MHz < 10) {
    lcd.print(0);
  }
  lcd.print(frq_MHz);
  lcd.print(' ');

  if (frq_kHz < 100) {
    lcd.print(0);
  }
  if (frq_kHz < 10) {
    lcd.print(0);
  }
  lcd.print(frq_kHz);
  lcd.print(' ');

  if (frq_Hz < 100) {
    lcd.print(0);
  }
  if (frq_Hz < 10) {
    lcd.print(0);
  }
  lcd.print(frq_Hz);

  if (MAX2870_LD_isOk) {
    lcd.setCursor(18, 0);
    lcd.print(F("LD"));
    lcd.setCursor(1, 1);
    lcd.print(F("                 "));
  } else {
    lcd.setCursor(18, 0);
    lcd.print(F("--"));
    lcd.setCursor(1, 1);
    lcd.print(F("LOCK DETECT ERROR"));
  }

  switch (MONITOR_screen_num) {
    case 0:
      MONITOR_screen_0();
      break;
    case 1:
      MONITOR_screen_1();
      break;
  }
}

void MONITOR_screen_0() {
  lcd.setCursor(15, 2);
  lcd.print(F("    "));
  lcd.setCursor(15, 2);
  lcd.print(MAX2870_step_verb[MAX2870_step_idx]);

  lcd.setCursor(3, 2);
  lcd.print(MAX2870_noiseMode_verb[MAX2870_noiseMode_idx]);

  lcd.setCursor(15, 3);
  lcd.print(MAX2870_outPower_verb[MAX2870_outPower_idx]);  //dBm

  lcd.setCursor(3, 3);
  lcd.print(F("    "));
  lcd.setCursor(3, 3);
  lcd.print(MAX2870_my.chargePumpCurrent[MAX2870_chargePumpCurrent_idx], BIN); //charge pump current
}

void MONITOR_screen_1() {
  lcd.setCursor(3, 2);
  lcd.print(F("    "));
  lcd.setCursor(3, 2);
  lcd.print(PORT_ANALOG_IN_get(PORT_ANALOG_IN_1));

  lcd.setCursor(3, 3);
  lcd.print(F("    "));
  lcd.setCursor(3, 3);
  lcd.print(PORT_ANALOG_IN_get(PORT_ANALOG_IN_2));

}

void MONITOR_onSweep() {
  lcd.setCursor(0, 0);
  lcd.print(F("       SWEEP      "));
}

void MONITOR_periodical() {
  switch (MONITOR_screen_num) {
    case 0:
      break;
    case 1:
      MONITOR_screen_1();
      break;
  }
}

