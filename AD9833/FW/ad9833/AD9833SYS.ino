
void AD9833_init() {
  pinMode(AD9833_DIGIPOT_CS, OUTPUT);
  AD9833_DIGIPOT_set();

  AD9833_gen.Begin();
  delay(5);
}

void AD9833_DIGIPOT_set() {
  delay(5);
  digitalWrite(AD9833_DIGIPOT_CS, LOW); //begin SPI transfer
  SPI.transfer(B00010001); //device addr
  SPI.transfer(AD9833_DIGIPOT_val); //set val 0..255
  digitalWrite(AD9833_DIGIPOT_CS, HIGH); //end SPI transfer
  delay(5);
}



void AD9833_wavetype_next() {
  AD9833_wavetype_currPos += 1;
  if (AD9833_wavetype_currPos >= 4) {  //cycle, return to 0-pos
    AD9833_wavetype_currPos = 0;
  }
  SYS_isNeedProcessConfig = true;
}

void AD9833_step_next() {
  AD9833_step_currPos += 1;
  if (AD9833_step_currPos >= 6) {  //cycle, return to 0-pos
    AD9833_step_currPos = 0;
  }
  SYS_isNeedProcessConfig = true;
}

void AD9833_interface_process() {
  switch (AD9833_encoder_state_menus) {
    case AD9833_encoder_state_menus_freq:
      AD9833_freq_incdec();
      break;
    case AD9833_encoder_state_menus_digipot:
      AD9833_digipot_incdec();
      break;
  }
}

void AD9833_freq_incdec() {
  int32_t dF = ENCODER_interrupt_delta * AD9833_step_vals[AD9833_step_currPos];;
  if (dF < 0 && abs(dF) >= AD9833_frequency) {
    AD9833_frequency = 1UL;
  } else {
    AD9833_frequency += dF;
  }
  if (AD9833_frequency > 12500000UL) {
    AD9833_frequency = 12500000UL;
  }
  else if (AD9833_frequency < 1UL) {
    AD9833_frequency = 1UL;
  }
  ENCODER_interrupt_delta = 0;
}

void AD9833_digipot_incdec() {
  AD9833_DIGIPOT_val += ENCODER_interrupt_delta;
  if (AD9833_DIGIPOT_val < 0) {
    AD9833_DIGIPOT_val = 0;
  }
  if (AD9833_DIGIPOT_val >= 256) {
    AD9833_DIGIPOT_val = 255;
  }
  ENCODER_interrupt_delta = 0;
}

void AD9833_setConfig() {
  // Apply a 1000 Hz sine wave using REG0 (register set 0). There are two register sets,
  // REG0 and REG1.
  // Each one can be programmed for:
  //   Signal type - SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, and HALF_SQUARE_WAVE
  //   Frequency - 0 to 12.5 MHz
  //   Phase - 0 to 360 degress (this is only useful if it is 'relative' to some other signal
  //           such as the phase difference between REG0 and REG1).
  // In ApplySignal, if Phase is not given, it defaults to 0.
  AD9833_gen.ApplySignal(SINE_WAVE, REG0, 1000);
  AD9833_gen.EnableOutput(true);   // Turn ON the output - it defaults to OFF

  AD9833_DIGIPOT_set();

}

