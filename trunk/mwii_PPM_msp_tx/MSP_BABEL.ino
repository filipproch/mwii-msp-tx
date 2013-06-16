void msp_babel(uint8_t MSPTYPE, uint8_t ploadsize, uint16_t pload_in[12]){ 
  char msp_string[20];
  char tabela[20];
  char checksum=0;

  if (ploadsize>0){ //non zero payload message
    uint8_t msb_pl=0;
    uint8_t lsb_pl=1;

    for(int i=0;i<ploadsize;i++){
      tabela[msb_pl]=pload_in[i] & 0xff;
      msb_pl=msb_pl+2;
      tabela[lsb_pl]=pload_in[i] >> 8;
      lsb_pl=lsb_pl+2;
    }

    msp_string[0]=ploadsize*2 & 0xff;
    checksum ^= (ploadsize*2 & 0xFF);
    msp_string[1]=MSPTYPE & 0xff;
    checksum ^= (MSPTYPE & 0xFF);
    for(int j=0;j<(ploadsize*2);j++){
      msp_string[2+j]=tabela[j] & 0xff;
      checksum ^= (tabela[j] & 0xff);
    }
    msp_string[ploadsize*2+2]= checksum;
  }

  else{
    msp_string[0]=ploadsize*2 & 0xff;
    checksum ^= (ploadsize*2 & 0xFF);
    msp_string[1]=MSPTYPE & 0xff;
    checksum ^= (MSPTYPE & 0xFF);
    msp_string[ploadsize*2+2]= checksum;
  }

#if !defined(DEBUG) 
  Serial.print("$M<"); 
  for(int u=0;u<(ploadsize*2+3);u++){
    Serial.write(msp_string[u]);
  }
#endif
}


