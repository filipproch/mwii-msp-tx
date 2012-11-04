//NOT CURRENTLY USED////
void getstatus(){
  char inByte[4];
  uint8_t i=0;
  char properResp[]={
    '$','M','>'      };

  CONNECTION_OK=0;
  if (Serial.available()) {
    while (Serial.available()) {
      if (i<3){   //read first 3 char
      i++;
      }
      else{ //truncate the rest
        i=3;
      }
      inByte[i] = Serial.read();
    }
    CONNECTION_OK=1;
  }
  //if (inByte[0]==properResp[0] && inByte[1]==properResp[1] && inByte[2]==properResp[2]){
    //CONNECTION_OK=1;
  //}
}




