void GEN_sweep(uint16_t SWEEP_freqMHz_from, uint16_t SWEEP_freqMHz_to) {
  if ( SWEEP_freqMHz_from < ADF4351_freqMHz_min || SWEEP_freqMHz_to > ADF4351_freqMHz_max ) {
    return;
  }
  if ( SWEEP_freqMHz_from >= SWEEP_freqMHz_to ) {
    return;
  }

  MONITOR_onSweep();

  uint16_t ADC_in; //  max65k
  uint16_t freqMHz_print;
  ADF4351_frequency = (uint32_t) SWEEP_freqMHz_from * 100000;//  *10 = Hz
  uint32_t SWEEP_freqHz_to_div10 = (uint32_t) SWEEP_freqMHz_to * 100000; //  *10 = Hz
  while (ADF4351_frequency <= SWEEP_freqHz_to_div10) {

    ADF4351_setConfig();

    freqMHz_print = ADF4351_frequency / 100000; //  *10 = Hz
    Serial.print(freqMHz_print); //MHz
    Serial.print(';');
    ADF4351_frequency = (uint32_t) ADF4351_frequency + ADF4351_freqStepCurrent;

    //wait until generator lock detect?  Get real time for LD and write simple delay(timeToLD_ms);
    if (!digitalRead(ADF4351_LD_pin)) {
      delay(5);
      if (!digitalRead(ADF4351_LD_pin)) {
        delay(5);
        if (!digitalRead(ADF4351_LD_pin)) {
          delay(5);
          if (!digitalRead(ADF4351_LD_pin)) {
            delay(5);
          }
        }
      }
    }
    delay(50); //??? for ADC stabilization, capacitor charge-discharge

    Serial.print(PORT_ANALOG_IN_get(PORT_ANALOG_IN_0), DEC);//0-1024
    Serial.print(';');

    Serial.print(0, DEC);//0-1024
    Serial.print(';');

    Serial.print(F("\r\n"));
  }

  MONITOR_init();
  MONITOR_render();

}
