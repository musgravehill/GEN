
void AD9833_init() {
  pinMode(AD9833_DIGIPOT_CS, OUTPUT);
  digitalWrite(AD9833_DIGIPOT_CS, HIGH);

  SPI.begin();
  delay(50);
  AD9833_DIGIPOT_set();

  AD9833_gen.Begin();
  delay(5);
}

void AD9833_DIGIPOT_set() {
  byte AD9833_DIGIPOT_val_b = AD9833_DIGIPOT_val;
  delay(5);
  digitalWrite(AD9833_DIGIPOT_CS, LOW); //begin SPI transfer
  SPI.transfer(B00010001); //device addr
  SPI.transfer(AD9833_DIGIPOT_val_b); //set val 0..255
  digitalWrite(AD9833_DIGIPOT_CS, HIGH); //end SPI transfer
  delay(5);
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

  AD9833_gen.ApplySignal(SINE_WAVE, REG0, AD9833_frequency);
  AD9833_gen.EnableOutput(true);   // Turn ON the output - it defaults to OFF
  delay(5000);
  AD9833_gen.ApplySignal(TRIANGLE_WAVE, REG0, AD9833_frequency);
  delay(5000);
  AD9833_gen.ApplySignal(SQUARE_WAVE, REG0, AD9833_frequency);
  delay(5000);
  AD9833_gen.ApplySignal(HALF_SQUARE_WAVE, REG0, AD9833_frequency);
  delay(5000);


  AD9833_DIGIPOT_set();

}

