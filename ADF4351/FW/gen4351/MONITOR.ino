void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}

void MONITOR_render(bool isForce) {
  if (!isForce) { //AND NOT isNeedRender
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ENCODER_interrupt_count);
}
