//NOT CURRENTLY USED////
void stickcal(){
  uint32_t caltime=millis();
  uint16_t calv=0;
  
  
  analogWrite(BUZERPIN,50);

  while((millis()-caltime)<10000){
    
    for(int u=0;u<4;u++){
      calv=analogRead(u+1);
      
      if(calv>ANLOGMINMAX[u][1]){
        ANLOGMINMAX[u][1]=calv;
      }
      
      if(calv<ANLOGMINMAX[u][0]){
        ANLOGMINMAX[u][0]=calv;
      }
      
    }
    
  }
  
  analogWrite(BUZERPIN,0);
  
}

