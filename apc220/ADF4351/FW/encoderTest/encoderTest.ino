

//========================================== INTERFACE ==========================================================
#define ENCODER_pin_A 2 //Пин прерывания
#define ENCODER_pin_B 3 //Любой другой пин 

volatile int ENCODER_count = 0;       // Счетчик оборотов. Периодически проверять ENCODER_count и делать действтия,потом ENCODER_count=0 и снова ждем вращения
// в прерываниях делать дела нельзя - слишком долго
volatile int ENCODER_state = 0;       // Переменная хранящая статус вращения

volatile int ENCODER_pin_A_val = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int ENCODER_pin_B_val = 0;   // Переменные хранящие состояние пина, для экономии времени


#define ENCODER_button 8

//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_prev_5ms = 0L;
uint32_t TIMEMACHINE_prev_311ms = 0L;
uint32_t TIMEMACHINE_prev_2222ms = 0L;

void setup() { 
  ENCODER_init(); 
  Serial.begin(9600);  
}

void loop() {
  TIMEMACHINE_loop();
}


