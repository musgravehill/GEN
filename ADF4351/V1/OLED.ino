void OLED_init() {
  myOLED.begin();
  myOLED.setBrightness(1);
}

void OLED_display() {
  //LD lock detected
  if (digitalRead(LD_pin)) {
    digitalWrite(LED_pin, 1);
  } else {
    digitalWrite(LED_pin, 0);
  }

  //OLED
  myOLED.clrScr();

  myOLED.setFont(SmallFont);
  myOLED.print(".", 60, 8);
  myOLED.print("STEP", 0, 23);
  myOLED.print(OLED_stepsVariants_kmhz[ADF4351_stepsVariantsNumCurrent], 76, 23);

  /*FUTURE_
    if (INTERFACE_freq_multiplier_current > 0) {
    myOLED.print("X", 108, 23);
    }*/
 // myOLED.print("OUT", 0, 40);
 // myOLED.print("dBm", 45, 41);

  if (ADF4351_SWEEP_isOn) {
    myOLED.print("SWEEP", 92, 56);
  }

  myOLED.print(ADF4351_lowNoiseOrSpur_verb[ADF4351_lowNoiseOrSpur_current], 0, 56);
  if (ADF4351_isNeedSetNewConfig) {
    if (OLED_blynk_state) {
      myOLED.print("SET", 110, 41);
      OLED_blynk_state = false;
    } else {
      OLED_blynk_state = true;
    }
  }

  myOLED.setFont(MediumNumbers);

  /*FUTURE_
    if (INTERFACE_freq_multiplier_current > 0) {
      myOLED.printNumI(INTERFACE_freq_multiplier_variants[INTERFACE_freq_multiplier_current], 116, 18);
    }
  */
  //FUTURE_   INTERFACE_freq_multiplier_variants[INTERFACE_freq_multiplier_current] *

  uint16_t f_m = ADF4351_frequency / 100000L; //*10Hz
  uint32_t f_k =  ADF4351_frequency % 100000; //*10Hz
  myOLED.printNumI(f_m, 0, 0, 5, '0');
  myOLED.printNumI(f_k, 68, 0, 5, '0');

  myOLED.print(OLED_stepsVariants_val[ADF4351_stepsVariantsNumCurrent], 26, 18);

  //myOLED.print("-4", 20, 36); //ADF4351_outputPower_verb[ADF4351_outputPower_current]

  myOLED.update();
}





