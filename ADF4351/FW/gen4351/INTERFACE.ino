
void BUTTON_init() { 
  pinMode(BTN_lownoisespur, INPUT_PULLUP);////--------------remove  pullup if user hardware resistors
  pinMode(BTN_out_power, INPUT_PULLUP);////--------------remove  pullup if user hardware resistors  
  pinMode(LD_pin, INPUT); //lock detect - if ADF4351 generate freq or not
  pinMode(LED_lock_detect, OUTPUT);
}

void BUTTON_check() {
  boolean button_state;

  

  //BTN LOW noise\spur mode
  button_state = digitalRead(BTN_lownoisespur);
  if (!button_state) {
    ADF4351_lowNoiseSpurMode_next();
  }

  //BTN output rf power
  button_state = digitalRead(BTN_out_power);
  if (!button_state) {
    ADF4351_out_power_next();
  }

  //BTN ENCODER
  button_state = digitalRead(ENCODER_button);
  if (!button_state) {
    ADF4351_step_next();
  }
}


void ENCODER_init() {
  pinMode(ENCODER_pin_A, INPUT_PULLUP);////--------------remove  pullup if user hardware resistors
  pinMode(ENCODER_pin_B, INPUT_PULLUP);////--------------remove  pullup if user hardware resistors
  attachInterrupt(0, ENCODER_interrupt, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала d2

  pinMode(ENCODER_button, INPUT_PULLUP);////--------------remove  pullup if user hardware resistors
}

void ENCODER_interrupt() {
  ENCODER_interrupt_pin_A_val = digitalRead(ENCODER_pin_A);            // Получаем состояние пинов A и B
  ENCODER_interrupt_pin_B_val = digitalRead(ENCODER_pin_B);

  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
  if (!ENCODER_interrupt_pin_A_val &&  ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
  if (!ENCODER_interrupt_pin_A_val && !ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
  if (ENCODER_interrupt_pin_A_val && ENCODER_interrupt_state != 0) {
    if (ENCODER_interrupt_state == 1 && !ENCODER_interrupt_pin_B_val || ENCODER_interrupt_state == -1 && ENCODER_interrupt_pin_B_val) { // Если на линии А снова единица, значит шаг был
      ENCODER_interrupt_delta += ENCODER_interrupt_state;
      ENCODER_interrupt_state = 0;      
    }
  }
  SYS_isNeedProcessConfig = true;
  sei(); // Разрешаем обработку прерываний
}


//ADF4351_freq_inc();

//ADF4351_freq_dec();



