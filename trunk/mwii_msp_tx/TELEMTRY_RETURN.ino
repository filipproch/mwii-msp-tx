//NOT CURRENTLY USED////
void getstatus(){
  char inByte[20];
  uint8_t i=0;
  char properResp[]={
    '$','M','>'      };

  CONNECTION_OK=0;
  if (Serial.available()) {

    while (Serial.available()) {    
      inByte[i] = Serial.read();
      i++;
    }
  }
  if (inByte[0]==properResp[0] && inByte[1]==properResp[1] && inByte[2]==properResp[2]){
    CONNECTION_OK=1;
  }
}




