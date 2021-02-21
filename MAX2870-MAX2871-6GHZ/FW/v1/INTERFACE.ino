
void BUTTON_init() {

}

void BUTTON_check() {

  //lock detect
  boolean button_state = digitalRead(MAX2870_pin_LD);
  if (button_state != MAX2870_LD_isOk) {
    MAX2870_LD_isOk = button_state;
    MONITOR_render();
  }
  MAX2870_LD_isOk = button_state;

  //btns analog keyboard
  //if BTNs====> GEN_****_next() ====> SYS_isNeedProcessConfig=true;
  uint8_t btn_code = KEYBOARD_get();
  switch (btn_code) {
    case KEYBOARD_power:
      GEN_outPower_next();
      break;
    case KEYBOARD_future:
      
      break;
    case KEYBOARD_step:
      GEN_step_next();
      break;
    case KEYBOARD_noiseMode:
      GEN_noiseMode_next();
      break;
    case KEYBOARD_cpc:
      GEN_chargePumpCurrent_next();
      break;
  }
  if (btn_code > 0) {
    SYS_isNeedProcessConfig = true;
  }

}

uint8_t KEYBOARD_get() {
  uint16_t ADC_in; //  max65k
  // The ADC provides us with 10 Bit resolution. So to get 11 Bit resolution we need to oversample by:
  // 4^n,  (n= 11-10=1)    => 4 samples.
  ADC_in = 0;
  for (byte i = 0; i < 4; i++) {
    ADC_in += analogRead(BTN_ANALOG_IN);
  }
  ADC_in = (ADC_in  >> 2) + 1;     //+1 for chart, charts draw if data>0


#ifdef DBG  //#endif    
  Serial.print(';');
  Serial.println(ADC_in, DEC);
#endif

  /// 1=power  210=null 430=step 625=noise 826=cp 1024=noBtn
  if (ADC_in < 70) return KEYBOARD_power;
  else if (ADC_in < 300) return KEYBOARD_future;
  else if (ADC_in < 500) return KEYBOARD_step;
  else if (ADC_in < 700) return KEYBOARD_noiseMode;
  else if (ADC_in < 900) return KEYBOARD_cpc;
  else return 0;
}


void ENCODER_init() {
  pinMode(ENCODER_pin_A, INPUT);////--------------remove  pullup if user hardware resistors
  pinMode(ENCODER_pin_B, INPUT);////--------------remove  pullup if user hardware resistors
  attachInterrupt(0, ENCODER_interrupt, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала d2
}

void ENCODER_interrupt() {
  ENCODER_interrupt_pin_A_val = digitalRead(ENCODER_pin_A);            // Получаем состояние пинов A и B
  ENCODER_interrupt_pin_B_val = digitalRead(ENCODER_pin_B);

  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
  if (!ENCODER_interrupt_pin_A_val &&  ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
  if (!ENCODER_interrupt_pin_A_val && !ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
  if (ENCODER_interrupt_pin_A_val && ENCODER_interrupt_state != 0) {
    if ( (ENCODER_interrupt_state == 1 && !ENCODER_interrupt_pin_B_val) || (ENCODER_interrupt_state == -1 && ENCODER_interrupt_pin_B_val) ) { // Если на линии А снова единица, значит шаг был
      ENCODER_interrupt_delta -= ENCODER_interrupt_state;
      ENCODER_interrupt_state = 0;
      SYS_isNeedProcessConfig = true;
    }
  }
  sei(); // Разрешаем обработку прерываний
}

void ENCODER_process() {
  int64_t dF = (int64_t) ENCODER_interrupt_delta *  MAX2870_step[MAX2870_step_idx];  

  //dont overflow, dont goto null
  if (dF < 0 && abs(dF) >= MAX2870_OUT_A_frequency_target) {
    MAX2870_OUT_A_frequency_target = (uint64_t) MAX2870_freqHz_min;
  } else {
    MAX2870_OUT_A_frequency_target += (uint64_t) dF;
  }

  MAX2870_OUT_A_frequency_target = (uint64_t) constrain(MAX2870_OUT_A_frequency_target, MAX2870_freqHz_min, MAX2870_freqHz_max);

  /*if (MAX2870_OUT_A_frequency_target > MAX2870_freqHz_max) {
    MAX2870_OUT_A_frequency_target = MAX2870_freqHz_max;
    }
    else if (MAX2870_OUT_A_frequency_target < MAX2870_freqHz_min) {
    MAX2870_OUT_A_frequency_target = MAX2870_freqHz_min;
    }*/

  ENCODER_interrupt_delta = 0;

  MAX2870_my.pre_set_frequency_OUT_A(MAX2870_OUT_A_frequency_target);

}


