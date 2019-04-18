void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_311ms) > 911L) {
    TIMEMACHINE_311ms();
    TIMEMACHINE_prev_311ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prev_101ms) > 101L) {
    TIMEMACHINE_101ms();
    TIMEMACHINE_prev_101ms = TIMEMACHINE_currMillis;
  }
}

void TIMEMACHINE_311ms() {
   SERIAL_processData();
    TS5823_sweep();
}

void TIMEMACHINE_101ms() {
 
}


