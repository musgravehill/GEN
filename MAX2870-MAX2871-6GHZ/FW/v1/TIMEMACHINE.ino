void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();
  if (TIMEMACHINE_currMillis > TIMEMACHINE_next_101ms) {
    TIMEMACHINE_101ms();
    TIMEMACHINE_next_101ms = TIMEMACHINE_currMillis + 101L;
  }
  if (TIMEMACHINE_currMillis > TIMEMACHINE_next_311ms) {
    TIMEMACHINE_311ms();
    TIMEMACHINE_next_311ms = TIMEMACHINE_currMillis + 311L;
  }
  if (TIMEMACHINE_currMillis > TIMEMACHINE_next_2000ms) {
    TIMEMACHINE_2000ms();
    TIMEMACHINE_next_2000ms = TIMEMACHINE_currMillis + 2000L;
  }
}

void TIMEMACHINE_101ms() {
  SYS_events_check();
}

void TIMEMACHINE_311ms() {
  BUTTON_check();
  // Serial.println(ADF4351_frequency);
  MONITOR_render();
}

void TIMEMACHINE_2000ms() {
  //simulate
  MAX2870_OUT_A_frequency_real += MAX2870_step[MAX2870_step_idx];
  if (MAX2870_OUT_A_frequency_real > 6500000000) {
    MAX2870_OUT_A_frequency_real = 20000000;
  }

  MAX2870_step_idx++;
  if (MAX2870_step_idx > 5) {
    MAX2870_step_idx = 0;
  }  

}



