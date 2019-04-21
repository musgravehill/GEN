void SYS_events_check() {
  //do actions, change params
  ADF4351_setConfig();

  //at the end
  if (SYS_isNeedProcessConfig) {
    MONITOR_render();
  }
  SYS_isNeedProcessConfig = false;
}
