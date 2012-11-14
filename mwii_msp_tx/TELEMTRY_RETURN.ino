//curently checks for RAW_RC return code 
void getstatus(){
  int inByte[6];
  uint8_t i=0;
  int properResp[]={
    36,77,62,0,200,200  };

  CONNECTION_OK=0;
  i=0;

#if defined(MEGA)
  if (Serial1.available()) {
    while (Serial1.available()) {
      inByte[i] = Serial1.read();
      if (i<6){ //read only 6 ints
        i++;
      }
      else{
        i=6;
      }
    } 
  }
#endif

#if !defined(MEGA)
  if (Serial.available()) {
    while (Serial.available()) {
      inByte[i] = Serial.read();
      if (i<6){ //read only 6 ints
        i++;
      }
      else{
        i=6;
      }
    } 
  }
#endif

if (inByte[0]==properResp[0] && inByte[2]==properResp[2]){
   CONNECTION_OK=1;
}

/*for (int u=0;u<3;u++){
  if (inByte[u]==properResp[u]){
    CONNECTION_OK &= CONNECTION_OK;
  }
  else{
    CONNECTION_OK=0;
  }
}*/

}








