
void BUTTON_init() {
     
}

void BUTTON_check() {

  if (digitalRead(MAX2870_pin_LD)) {
     
  } else {
     
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
    if ( (ENCODER_interrupt_state == 1 && !ENCODER_interrupt_pin_B_val) || (ENCODER_interrupt_state == -1 && ENCODER_interrupt_pin_B_val) ) { // Если на линии А снова единица, значит шаг был
      ENCODER_interrupt_delta -= ENCODER_interrupt_state;
      ENCODER_interrupt_state = 0;
      SYS_isNeedProcessConfig = true;
    }
  }
  sei(); // Разрешаем обработку прерываний
}


