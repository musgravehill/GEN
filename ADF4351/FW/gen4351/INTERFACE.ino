
void BUTTON_init() {
  pinMode(BTN_step, INPUT_PULLUP);
  pinMode(BTN_lownoisespur, INPUT_PULLUP);
  pinMode(BTN_out_power, INPUT_PULLUP);
  pinMode(LD_pin, INPUT); //lock detect - if ADF4351 generate freq or not
}

void BUTTON_check() {
  boolean button_state;
  //BTN STEP
  button_state = digitalRead(BTN_step);
  if (!button_state) {
    ADF4351_step_next();
  }
  //BTN LOW noise\spur mode
  button_state = digitalRead(BTN_lownoisespur);
  if (!button_state) {
    ADF4351_lowNoiseSpurMode_next();
  }

  /*
    //BTN output rf power
    button_state = digitalRead(BTN_out_power);
    if (!button_state) {
    //if ((currMillis - INTERFACE_action_prev_ms) > 503L) {
    //INTERFACE_action_prev_ms = currMillis;
    ADF4351_out_power_next();
    //}
    }*/


  //BTN ENCODER
  button_state = digitalRead(ENCODER_button);
  if (!button_state) {
    ADF4351_setConfig();
  }

}


void ENCODER_init() {
  pinMode(ENCODER_pin_A, INPUT);
  pinMode(ENCODER_pin_B, INPUT);
  attachInterrupt(0, ENCODER_interrupt, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала d2

  pinMode(ENCODER_button, INPUT);
}

void ENCODER_interrupt() {
  ENCODER_pin_A_val = digitalRead(ENCODER_pin_A);            // Получаем состояние пинов A и B
  ENCODER_pin_B_val = digitalRead(ENCODER_pin_B);

  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
  if (!ENCODER_pin_A_val &&  ENCODER_pin_B_val) ENCODER_state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
  if (!ENCODER_pin_A_val && !ENCODER_pin_B_val) ENCODER_state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
  if (ENCODER_pin_A_val && ENCODER_state != 0) {
    if (ENCODER_state == 1 && !ENCODER_pin_B_val || ENCODER_state == -1 && ENCODER_pin_B_val) { // Если на линии А снова единица, значит шаг был
      ENCODER_count += ENCODER_state;
      ENCODER_state = 0;
    }
  }
  sei(); // Разрешаем обработку прерываний
}


//ADF4351_freq_inc();

//ADF4351_freq_dec();



