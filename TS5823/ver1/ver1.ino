
#define TS5823_pin_1 4
#define TS5823_pin_2 5
#define TS5823_pin_3 6
#define TS5823_pin_4 7
#define TS5823_pin_5 8

boolean isSweep = false;
boolean sweep_tmp = false;
uint8_t TS5823_frch = 1; //1-32
uint16_t TS5823_frequency_MHz;

String SERIAL_data = "";
boolean SERIAL_isDataReady = false;

//================================== TIMEMACHINE =================================================================
uint32_t TIMEMACHINE_prev_311ms = 0L;
uint32_t TIMEMACHINE_prev_101ms = 0L;

void setup() {
  TS5823_init();
  //  BUTTON_init();
  Serial.begin(57600);
  analogReference(DEFAULT); // 5v
}

void loop() {
  TIMEMACHINE_loop();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '[') {
      SERIAL_data = "";
      //SERIAL_isDataReady = false;
    }
    else if (inChar == ']') {
      SERIAL_isDataReady = true;
    }
    else {
      SERIAL_data += inChar;
      //SERIAL_isDataReady = false;
    }
  }
}





