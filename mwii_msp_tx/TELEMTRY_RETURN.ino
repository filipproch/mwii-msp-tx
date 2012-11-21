#if defined(MEGA)
#define READ Serial1.read()
#define AVAIL Serial1.available() 
#endif
#if !defined(MEGA)
#define READ Serial.read() 
#define AVAIL Serial.available() 
#endif
uint8_t TRASH; //placeholder for bad messages
int checksum_in=0, chksum_read=1; //set different read checksum

///MWII ripout
uint32_t read32() {
  uint32_t t = read16();
  t+= (uint32_t)read16()<<16;
  return t;
}
uint16_t read16() {
  uint16_t t = read8();
  t+= (uint16_t)read8()<<8;
  return t;
}
uint8_t read8()  {
  int8_t curbt = READ;
  checksum_in ^= curbt;
  return (curbt)&0xff;
}

void getstatus(){
  int msg[20];
  int shebang[3]; 
  uint8_t msp_type=0, pl_size=0;

  CONNECTION_OK=CONNECTION_OK+1;
  SINGNDANCE=1;

  if (AVAIL) {      
    for (uint8_t u=0;u<3;u++){ //just to move to the end of $M>
      shebang[u]=read8();
    }

    if(shebang[0]==36 && shebang[1]==77 && shebang[2]==62){ //if MWII returns proper shebang we start processing response
      CONNECTION_OK=0; //if we get shebang back than connection must be O.K. !!!?!!
      SINGNDANCE=0; //enable vario noise  :)
      checksum_in = 0;
      pl_size = read8(); // it is allready int
      msp_type = read8(); //it is already int

      switch (msp_type){
      case MSP_SET_RAW_RC:
        read16();
        break;

      case MSP_ALTITUDE:
        ALTITUDE=read32();
        VARIO=read16();
        break;

      case MSP_BAT:
        VBAT_V=read8();
        VBAT_SUM=read16();
        break;

      case MSP_IDENT:
        VERSION=read8();
        MULTITYPE=read8();
        MSP_VERSION=read8();
        CAPAB=read32();
        break;

      }
      chksum_read=READ; //reading last byte == checksum directly with Serial.read
    }

    else { //erase serial buffer
      while (AVAIL){
        TRASH=read8();
      }
    }

    if (checksum_in!=chksum_read){
      ALTITUDE=-100;
      VARIO=0;
    }
  }

}





