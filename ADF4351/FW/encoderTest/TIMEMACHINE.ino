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
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_2222ms) > 2222L) {
    TIMEMACHINE_2222ms();
    TIMEMACHINE_prev_2222ms = TIMEMACHINE_currMillis;
  }
}

void TIMEMACHINE_5ms() {
   
}

void TIMEMACHINE_311ms() {
  //BUTTON_check();
}

void TIMEMACHINE_2222ms() {
  Serial.println(ENCODER_count);
}


