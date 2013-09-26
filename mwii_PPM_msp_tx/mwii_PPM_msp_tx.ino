/*
MWII-MSP-TX by Andrej Javorsek
 June 2013
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */
/*Code snippets taken directly from MultiWii http://www.multiwii.com */

//V2 to use as PPM to MSP converter

#include "def.h"

uint16_t SIGN[8]={
  0,0,0,0};
uint8_t SIG=0;
uint32_t GUT=0 ,T=0;
uint8_t PPM_OK=0;
uint8_t  RC_REFRESH_DELAY=(1000/RC_REFRESH_RATE);

uint16_t rcValue[]={
  1500,1500,1500,1500,1500,1500,1500,1500};

void setup() {
  pinMode(3,INPUT);
  Serial.begin(SERIAL_SPEED);
  PPM_PIN_INTERRUPT;
  //digitalWrite(APC_ENABLE_PIN,HIGH); //After setup enable APC module
}

void loop() {
  #if defined(NO_FAILSAFE)
  PPM_OK=1;
  #endif
  if((GUT-T)>RC_REFRESH_DELAY){ 

    SIGN[0]=rcValue[PITCH];
    SIGN[1]=rcValue[YAW];
    SIGN[2]=rcValue[THROTTLE];
    SIGN[3]=rcValue[ROLL];

    for(int i=4;i<RC_CHANS;i++){
      SIGN[i]=rcValue[i];
    }

#if !defined(DEBUG)
    if(PPM_OK){
      msp_babel(MSP_SET_RAW_RC, 8,SIGN);
      PPM_OK=0;
    }
#else
    if(PPM_OK){
      for (int i=0;i<RC_CHANS;i++){
        Serial.println(rcValue[i]);
      }
      Serial.println(" ");
      PPM_OK=0;
      delay(80); //small delay when debug printing
    }
#endif

    T=GUT; 
  }
  GUT=millis();
}
