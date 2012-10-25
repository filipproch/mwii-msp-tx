void msp_babel(uint16_t rx_chs[8]){ 
  char msp_rx_string[18];
  char tabela[16];
  char checksum=0;
  
  //int msp_babel(int yaw, int th, int roll, int pitch, int aux1, int aux2, int aux3){
    /*
    msp_rx_string[]=velikost_payloada, MSP_SET_RAW_RC, payload[],checksumm
    */
    uint8_t msb_pl=0;
    uint8_t lsb_pl=1;
    
    for(int i=0;i<8;i++){
      tabela[msb_pl]=rx_chs[i] & 0xff;
      msb_pl=msb_pl+2;
      
      tabela[lsb_pl]=rx_chs[i] >> 8;
      lsb_pl=lsb_pl+2;
    }
    
    msp_rx_string[0]=sizeof(tabela) & 0xff;
    checksum ^= (sizeof(tabela) & 0xFF);
    
    msp_rx_string[1]=MSP_SET_RAW_RC & 0xff;
    checksum ^= (MSP_SET_RAW_RC & 0xFF);
    
    for(int j=0;j<16;j++){
      msp_rx_string[2+j]=tabela[j] & 0xff;
      checksum ^= (tabela[j] & 0xff);
    }
    
    msp_rx_string[18]= checksum;
    
    Serial.print("$M<"); 
    for(int u=0;u<19;u++){
     
     Serial.write(msp_rx_string[u]);
    }
}
