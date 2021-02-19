
void GEN_noiseMode_next() {
  MAX2870_noiseMode_idx++;
  if (MAX2870_noiseMode_idx > 2) {
    MAX2870_noiseMode_idx = 0;
  }
  MAX2870_my.pre_set_noiseMode(MAX2870_noiseMode_idx);
}

void GEN_outPower_next() {
  MAX2870_outPower_idx++;
  if (MAX2870_outPower_idx > 3) {
    MAX2870_outPower_idx = 0;
  }
  MAX2870_my.pre_set_power_OUT_A(MAX2870_outPower_idx);
}

void GEN_chargePumpCurrent_next() {
  MAX2870_chargePumpCurrent_idx++;
  if (MAX2870_chargePumpCurrent_idx > 15) {
    MAX2870_chargePumpCurrent_idx = 0;
  }
  MAX2870_my.pre_set_chargePumpCurrent(MAX2870_chargePumpCurrent_idx);
}

void GEN_step_next() {
  MAX2870_step_idx++;
  if (MAX2870_step_idx > 5) {
    MAX2870_step_idx = 0;
  }
}

void GEN_sweep(uint16_t SWEEP_freqMHz_from, uint16_t SWEEP_freqMHz_to) {
  if ( SWEEP_freqMHz_from < (MAX2870_freqHz_min / 1000000) || SWEEP_freqMHz_to > (MAX2870_freqHz_max / 1000000) ) {
    return;
  }
  if ( SWEEP_freqMHz_from >= SWEEP_freqMHz_to ) {
    return;
  }
  MONITOR_onSweep();

  uint16_t ADC_in; //  max65k
  uint16_t freqMHz_print;
  MAX2870_OUT_A_frequency_target = (uint64_t) SWEEP_freqMHz_from * 1000000;
  uint64_t SWEEP_freqHz_to = (uint64_t) SWEEP_freqMHz_to * 1000000;
  while (MAX2870_OUT_A_frequency_target <= SWEEP_freqHz_to) {
    MAX2870_my.pre_set_frequency_OUT_A(MAX2870_OUT_A_frequency_target);
    MAX2870_my.setConfig();

    freqMHz_print = MAX2870_OUT_A_frequency_target / 1000000;
    Serial.print(freqMHz_print); //MHz
    Serial.print(';');
    MAX2870_OUT_A_frequency_target += MAX2870_step[MAX2870_step_idx];

    //wait until generator lock detect?  Get real time for LD and write simple delay(timeToLD_ms);
    if (!digitalRead(MAX2870_pin_LD)) {
      delay(5);
      if (!digitalRead(MAX2870_pin_LD)) {
        delay(5);
        if (!digitalRead(MAX2870_pin_LD)) {
          delay(5);
          if (!digitalRead(MAX2870_pin_LD)) {
            delay(5);
          }
        }
      }
    }
    delay(10); //??? for ADC stabilization, capacitor charge-discharge 


    // The ADC provides us with 10 Bit resolution. So to get 11 Bit resolution we need to oversample by:
    // 4^n,  (n= 11-10=1)    => 4 samples.
    ADC_in = 0;
    for (byte i = 0; i < 4; i++) {
      ADC_in += analogRead(PORT_ANALOG_IN_0);
    }
    ADC_in = (ADC_in  >> 2) + 1;     //+1 for chart, charts draw if data>0
    Serial.print(ADC_in, DEC);//0-1023
    Serial.print(';');

    ADC_in = 0;
    for (byte i = 0; i < 4; i++) {
      ADC_in += analogRead(PORT_ANALOG_IN_1);
    }
    ADC_in = (ADC_in  >> 2) + 1;     //+1 for chart, charts draw if data>0
    Serial.print(ADC_in, DEC);//0-1023
    Serial.print(';');

    Serial.print(F("\r\n"));
  }
  MONITOR_init();
  MONITOR_render();
}
