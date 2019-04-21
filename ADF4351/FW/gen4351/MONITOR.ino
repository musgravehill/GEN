void MONITOR_init() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}

void MONITOR_render_force() {
  if (SYS_isNeedRender) {
    MONITOR_render();
  }
}

void MONITOR_render() {
  SYS_isNeedRender = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ENCODER_interrupt_count);
}
