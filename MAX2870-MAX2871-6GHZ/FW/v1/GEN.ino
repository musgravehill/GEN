void SWEEP(uint16_t SWEEP_freqMHz_from, uint16_t SWEEP_freqMHz_to) {

  if (SWEEP_freqMHz_from < MAX2870_freqMHz_min || SWEEP_freqMHz_to > MAX2870_freqMHz_max) {
    return;
  }

  MONITOR_onSweep();

  uint16_t idx = 0;
  MAX2870_OUT_A_frequency_target = SWEEP_freqMHz_from * 1000000;
  uint64_t SWEEP_freqHz_to = SWEEP_freqMHz_to * 1000000;
  while (MAX2870_OUT_A_frequency_target <= SWEEP_freqHz_to) {
    //MAX2870_my.pre_set_frequency_OUT_A(MAX2870_OUT_A_frequency_target);
    //MAX2870_my.setConfig();
    Serial.print(float(MAX2870_OUT_A_frequency_target/1000000.0), 6); //MHz
    Serial.print(';');
    idx++;
    MAX2870_OUT_A_frequency_target += idx * MAX2870_step[MAX2870_step_idx];

    delay(30);
    /*
      The ADC provides us with 10 Bit resolution. So to get 11 Bit resolution we need to oversample by:
      4^n,  (n= 11-10=1)    => 4 samples.
    */
    uint16_t ADC_in; //  max65k
    ADC_in = 0;
    for (byte i = 0; i < 4; i++) {
      ADC_in += analogRead(PORT_ANALOG_IN_0);
    }
    ADC_in = (ADC_in  >> 2) + 1;     //+1 for chart, charts draw if data>0
    Serial.print(ADC_in, DEC);//0-1023
    Serial.print(';');
    Serial.print("\r\n");

  }

  MONITOR_init();
  MONITOR_render();
}
