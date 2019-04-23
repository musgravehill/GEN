void SYS_events_check() {
  if (SYS_isNeedProcessConfig) {
    int32_t dF = ENCODER_interrupt_delta * ADF4351_freqStepCurrent;
    if (dF < 0 && abs(dF) >= ADF4351_frequency) {
      ADF4351_frequency = 3500000L;
    } else {
      ADF4351_frequency += dF;
    }
    if (ADF4351_frequency > 500000000L) {
      ADF4351_frequency = 500000000L;
    }
    else if (ADF4351_frequency < 3500000L) {
      ADF4351_frequency = 3500000L;
    }
    ENCODER_interrupt_delta = 0;
    ADF4351_setConfig();

    //at the end
    MONITOR_render();
    SYS_isNeedProcessConfig = false;
  }
}
