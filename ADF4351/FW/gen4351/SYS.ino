void SYS_events_check() {
  ADF4351_frequency += ENCODER_interrupt_delta * ADF4351_freqStepCurrent;
  ENCODER_interrupt_delta=0;
  ADF4351_setConfig();

  //at the end
  if (SYS_isNeedProcessConfig) {
    MONITOR_render();
  }
  SYS_isNeedProcessConfig = false;
}
