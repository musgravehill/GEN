void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_5ms) > 5L) {
    TIMEMACHINE_5ms();
    TIMEMACHINE_prev_5ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_311ms) > 311L) {
    TIMEMACHINE_311ms();
    TIMEMACHINE_prev_311ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_2000ms) > 2000L) {
    TIMEMACHINE_2000ms();
    TIMEMACHINE_prev_2000ms = TIMEMACHINE_currMillis;
  }
}

void TIMEMACHINE_5ms() {
  MONITOR_render(false);
}

void TIMEMACHINE_311ms() {
  BUTTON_check();
}

void TIMEMACHINE_2000ms() {
  MONITOR_render(true);
}


