
void BUTTON_init() {
  pinMode(BTN_wavetype, INPUT);
  pinMode(BTN_digipot, INPUT);
  pinMode(ENCODER_button, INPUT);
}

void BUTTON_check() {
  boolean button_state;

  button_state = digitalRead(BTN_digipot);
  if (!button_state) {
    if (AD9833_encoder_state_menus ==  AD9833_encoder_state_menus_digipot) {
      AD9833_encoder_state_menus = AD9833_encoder_state_menus_freq;
    } else {
      AD9833_encoder_state_menus = AD9833_encoder_state_menus_digipot;
    }
  }

  button_state = digitalRead(BTN_wavetype);
  if (!button_state) {
    AD9833_wavetype_next();
  }

  button_state = digitalRead(ENCODER_button);
  if (!button_state) {
    AD9833_step_next();
  }
}


void ENCODER_init() {
  pinMode(ENCODER_pin_A, INPUT);////--------------remove  pullup if user hardware resistors
  pinMode(ENCODER_pin_B, INPUT);////--------------remove  pullup if user hardware resistors
  attachInterrupt(0, ENCODER_interrupt, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала d2

  pinMode(ENCODER_button, INPUT);////--------------remove  pullup if user hardware resistors
}

void ENCODER_interrupt() {
  ENCODER_interrupt_pin_A_val = digitalRead(ENCODER_pin_A);            // Получаем состояние пинов A и B
  ENCODER_interrupt_pin_B_val = digitalRead(ENCODER_pin_B);

  cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
  if (!ENCODER_interrupt_pin_A_val &&  ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
  if (!ENCODER_interrupt_pin_A_val && !ENCODER_interrupt_pin_B_val) ENCODER_interrupt_state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
  if (ENCODER_interrupt_pin_A_val && ENCODER_interrupt_state != 0) {
    if (ENCODER_interrupt_state == 1 && !ENCODER_interrupt_pin_B_val || ENCODER_interrupt_state == -1 && ENCODER_interrupt_pin_B_val) { // Если на линии А снова единица, значит шаг был
      ENCODER_interrupt_delta -= ENCODER_interrupt_state;
      ENCODER_interrupt_state = 0;
      SYS_isNeedProcessConfig = true;
    }
  }
  sei(); // Разрешаем обработку прерываний
}






