void SYS_events_check() {
  //do actions, change params
  ADF4351_setConfig();

  //at the end
  if (SYS_isNeedProcessConfig) {
    SYS_isNeedRender = true;
  }
  SYS_isNeedProcessConfig = false;
}
