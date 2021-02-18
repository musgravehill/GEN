void SYS_events_check() {
  if (SYS_isNeedProcessConfig) {

    ENCODER_process();

    //!!!!!!! MAX2870_my.setConfig();

    MONITOR_render();
    SYS_isNeedProcessConfig = false;
  }
}
