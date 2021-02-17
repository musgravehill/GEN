void SYS_events_check() {
  if (SYS_isNeedProcessConfig) {
    int64_t dF = ENCODER_interrupt_delta * MAX2870_step[MAX2870_step_idx];
    if (dF < 0 && abs(dF) >= MAX2870_OUT_A_frequency_target) {
      MAX2870_OUT_A_frequency_target = 20000000;
    } else {
      MAX2870_OUT_A_frequency_target += dF;
    }
    if (MAX2870_OUT_A_frequency_target > 6500000000) {
      MAX2870_OUT_A_frequency_target = 6500000000;
    }
    else if (MAX2870_OUT_A_frequency_target < 20000000) {
      MAX2870_OUT_A_frequency_target = 20000000;
    }
    ENCODER_interrupt_delta = 0;
    //!!!!!!!!!!!!!!!!!!!!!!TODO MAX2870 SET FREQ here

    
    MONITOR_render();
    SYS_isNeedProcessConfig = false;
  }
}
