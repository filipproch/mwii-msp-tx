  void rxInt() {
    uint16_t now,diff;
    static uint16_t last = 0;
    static uint8_t chan = 0;
  
    now = micros();
    diff = now - last;
    last = now;
    if(diff>3000) chan = 0;
    else {
      if( 900<diff && diff<2200 && chan<RC_CHANS ) {   //Only if the signal is between these values it is valid, otherwise the failsafe counter should move up
        rcValue[chan] = diff;
      }
    chan++;
  }
}

