void rxInt() {
  uint16_t now,diff;
  static uint16_t last = 0;
  static uint8_t chan = 0;

  now = micros();
  diff = now - last;
  last = now;
  if(diff>3000) chan = 0;
  else {
    if( 900<diff && diff<2200 && chan<RC_CHANS ) {
      rcValue[chan] = diff;
      PPM_OK=1;
    }
    else{
      PPM_OK=0;
    }
    chan++;
  }
}


