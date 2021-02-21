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
  SERIAL_processData();   
}

void TIMEMACHINE_2000ms() {  

}



