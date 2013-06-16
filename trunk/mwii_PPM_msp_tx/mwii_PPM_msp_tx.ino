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

//#define DEBUG

//Position of signal in PPM train, differs for TX
#define  ROLL     0
#define  PITCH    1
#define  YAW      2
#define  THROTTLE 3

#define RC_REFRESH_RATE 40 //in Hz (40 gives good respones)
#define SERIAL_SPEED 19200
#define APC_ENABLE_PIN 9    //Enable pin is needed on APC802
#define PPM_INTERUPT_CHANNEL 1 // on Adru ProMini Channel 1 is on pin 3  http://arduino.cc/en/Reference/AttachInterrupt

#define PPM_PIN_INTERRUPT          attachInterrupt(PPM_INTERUPT_CHANNEL, rxInt, RISING);
#define RC_CHANS 8
#define MSP_SET_RAW_RC 200

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
  digitalWrite(APC_ENABLE_PIN,HIGH); //After setup enable APC module
}

void loop() {
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
