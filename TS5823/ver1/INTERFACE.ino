
/*
void BUTTON_init() {
  pinMode(BTN_step, INPUT_PULLUP);
  pinMode(LED_pin, OUTPUT);
}

void BUTTON_check() {
  boolean button_state;
  uint32_t  currMillis = millis();
  //BTN STEP
  button_state = digitalRead(BTN_step);
  if (!button_state) {
    if ((currMillis - INTERFACE_action_prev_ms) > 503L) {
      INTERFACE_action_prev_ms = currMillis;
      ADF4351_step_next();
    }
  }
}
*/
