void SYS_events_check() {
  if (SYS_isNeedProcessConfig) {
    int32_t dF = ENCODER_interrupt_delta * AD9833_freqStepCurrent;
    if (dF < 0 && abs(dF) >= AD9833_frequency) {
      AD9833_frequency = 3500000L;
    } else {
      AD9833_frequency += dF;
    }
    if (AD9833_frequency > 500000000L) {
      AD9833_frequency = 500000000L;
    }
    else if (AD9833_frequency < 3500000L) {
      AD9833_frequency = 3500000L;
    }
    ENCODER_interrupt_delta = 0;
    AD9833_setConfig();

    //at the end
    MONITOR_render();
    SYS_isNeedProcessConfig = false;
  }
}
