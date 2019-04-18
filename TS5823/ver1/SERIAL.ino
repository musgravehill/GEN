void SERIAL_processData() {
  if (SERIAL_isDataReady) {  
      
    TS5823_startSweep();
    
    //ready to get new data
    SERIAL_data = "";
    SERIAL_isDataReady = false;
  }
}
