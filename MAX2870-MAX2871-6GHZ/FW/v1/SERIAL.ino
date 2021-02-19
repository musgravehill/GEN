void SERIAL_processData() {
  if (SERIAL_isDataReady) {

    uint16_t SWEEP_freqMHz_from;
    uint16_t SWEEP_freqMHz_to;

    uint8_t index = SERIAL_data.indexOf(";");
    SWEEP_freqMHz_from = atol(SERIAL_data.substring(0, index).c_str());
    SERIAL_data = SERIAL_data.substring(index + 1);

    index = SERIAL_data.indexOf(";");
    SWEEP_freqMHz_to = atol(SERIAL_data.substring(0, index).c_str());
    SERIAL_data = SERIAL_data.substring(index + 1);

    SERIAL_data = "";
    SERIAL_isDataReady = false;

    GEN_sweep(SWEEP_freqMHz_from, SWEEP_freqMHz_to);
  }
}
