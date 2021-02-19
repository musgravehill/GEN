
void BUTTON_init() {

}

void BUTTON_check() {
  boolean button_state;

  //lock detect
  button_state = digitalRead(MAX2870_pin_LD);
  if (button_state != MAX2870_LD_isOk) {
    MAX2870_LD_isOk = button_state;
    MONITOR_render();
  }
  MAX2870_LD_isOk = button_state;


  //btns analog keyboard


  /*MAX2870_my.outPower_idx = 2; //+2dBm
    MAX2870_my.pre_set_power_OUT_A();

    MAX2870_my.noiseMode_idx = 1; //los spur #1
    MAX2870_my.pre_set_noiseMode();

    MAX2870_my.chargePumpCurrent_idx = 7; //B0111=2.56mA
    MAX2870_my.pre_set_chargePumpCurrent();
  */

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
  int64_t dF = ENCODER_interrupt_delta * MAX2870_step[MAX2870_step_idx];

  //dont overflow, dont goto null
  if (dF < 0 && abs(dF) >= MAX2870_OUT_A_frequency_target) {
    MAX2870_OUT_A_frequency_target = MAX2870_freqHz_min;
  } else {
    MAX2870_OUT_A_frequency_target += dF;
  }

  MAX2870_OUT_A_frequency_target = constrain(MAX2870_OUT_A_frequency_target, MAX2870_freqHz_min, MAX2870_freqHz_max);

  /*if (MAX2870_OUT_A_frequency_target > MAX2870_freqHz_max) {
    MAX2870_OUT_A_frequency_target = MAX2870_freqHz_max;
    }
    else if (MAX2870_OUT_A_frequency_target < MAX2870_freqHz_min) {
    MAX2870_OUT_A_frequency_target = MAX2870_freqHz_min;
    }*/

  ENCODER_interrupt_delta = 0;

  //MAX2870_my.pre_set_frequency_OUT_A(MAX2870_OUT_A_frequency_target);

}


