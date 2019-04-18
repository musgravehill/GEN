void SERIAL_processData() {
  if (SERIAL_isDataReady) {
    uint8_t index = SERIAL_data.indexOf(";");
    ADF4351_SWEEP_freq_from = atol(SERIAL_data.substring(0, index).c_str());
    SERIAL_data = SERIAL_data.substring(index + 1);

    index = SERIAL_data.indexOf(";");
    ADF4351_SWEEP_freq_to = atol(SERIAL_data.substring(0, index).c_str());
    SERIAL_data = SERIAL_data.substring(index + 1);

    ADF4351_SWEEP_freq_from = (ADF4351_SWEEP_freq_from - 1) * 100000;
    ADF4351_SWEEP_freq_to = ADF4351_SWEEP_freq_to * 100000;
    ADF4351_frequency = ADF4351_SWEEP_freq_from;

    ADF4351_SWEEP_isOn = true;

    //ready to get new data
    SERIAL_data = "";
    SERIAL_isDataReady = false;
  }
}
