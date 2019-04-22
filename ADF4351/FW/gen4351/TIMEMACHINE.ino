void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();
  if (TIMEMACHINE_currMillis > TIMEMACHINE_next_5ms) {
    TIMEMACHINE_5ms();
    TIMEMACHINE_next_5ms = TIMEMACHINE_currMillis + 5L;
  }
  if (TIMEMACHINE_currMillis > TIMEMACHINE_next_311ms) {
    TIMEMACHINE_311ms();
    TIMEMACHINE_next_311ms = TIMEMACHINE_currMillis + 311L;
  }
 
}

void TIMEMACHINE_5ms() {
  SYS_events_check();
}

void TIMEMACHINE_311ms() {
  BUTTON_check();
 // Serial.println(ADF4351_frequency);
}



