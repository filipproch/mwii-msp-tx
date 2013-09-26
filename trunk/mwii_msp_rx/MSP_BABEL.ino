uint8_t checksum=0;
uint8_t read_checksum=0;
uint8_t indRX;
uint8_t cmdMSP;
uint16_t rcData[8];
uint8_t rc_data=0;
uint8_t pldsize=0;
uint32_t LRT=0;
uint32_t LAST_SUCCESFUL_RX=0;

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
  uint8_t c = SER1_R;
  checksum ^= c;
  return (c)&0xff;
}

void serialCom() {
  rc_data=0;
  
  if (SER1_AV) {
    if(GUT-LRT>1){  //small delay in case serial does not start with shebang
      if (read8()=='$'){
        if (read8()=='M'){
          if (read8()=='<'){
            //if shebang O.K. start processing
            checksum=0;
            pldsize=read8();
            cmdMSP=read8();
            evaluateCommand();
            
          }
        }
      }
      else{
        LRT=GUT;
      } 
    }
  }
}

void evaluateCommand() {
  switch(cmdMSP) {
  case MSP_SET_RAW_RC:
    rc_data=1;
    for(uint8_t i=0;i<8;i++) {
      rcData[i] = read16();
    }
    break;
  default:  // we do not know how to handle the (valid) message, indicate error MSP $M!
    rc_data=0;
    //headSerialError(0);
    break;
  }
  uint8_t checksum_c=checksum;
  read_checksum=read8();

  if (rc_data==1 && read_checksum==checksum_c){   //if checksum and data O.K. assign to channel
    for (uint8_t u=0;u<8;u++){
      servo_val[u]=rcData[u];
      LAST_SUCCESFUL_RX=GUT;
    }
#if  defined(__AVR_ATmega2560__)
for (uint8_t k=0;k<8;k++){
    Serial.print(servo_val[k]);
    Serial.print(" ");
   // Serial.print(checksum_c);
}
Serial.println(" ");
#endif
  }

}








