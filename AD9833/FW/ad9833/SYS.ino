void SYS_events_check() {
  if (SYS_isNeedProcessConfig) {

    AD9833_interface_process();
    AD9833_setConfig();

    //at the end
    MONITOR_render();
    SYS_isNeedProcessConfig = false;
  }
}
