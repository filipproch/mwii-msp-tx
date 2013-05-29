/*
MWII-MSP-TX by Andrej Javorsek
 November 2012
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */
 
 /*V2 to use as PPM to MSP converter
Testing (But untested) support for TI Stelaris Launchpad and www.Energia.nu */

//#define DEBUG
//#define STELARIS

#define MSP_SET_RAW_RC           200

#define LED_REFRESH_RATE 1000/2  //in Hz
#define LED_ON_OK 9
#if defined(STELARIS)
 #define PPM_PIN_INTERRUPT          attachInterrupt(PC_4, rxInt, RISING);
#else
 #define PPM_PIN_INTERRUPT          attachInterrupt(1, rxInt, RISING);
#endif
#define RC_CHANS 8

uint16_t SIGN[8]={
   0,0,0,0};

uint8_t SIG=0;
uint32_t GUT=0 ,T=0, T_LED=0, TG_SW=0, ADP_DELAY=0, VARIO_BEEP_D=0; 
uint8_t STANJE=0;
uint16_t PN=0;
uint8_t PPM_OK=0;
uint8_t  RC_REFRESH_RATE=(1000/40); //in Hz

uint16_t rcValue[]={1500,1500,1500,1500,1500,1500,1500,1500};

#if defined(STELARIS)
 HardwareSerial Serial1;
 #define HSER Serial1   //define use of hardware serial port 1 on StelarPad
#else
 #define HSER Serial
#endif

void setup() {
  #if defined(STELARIS)
   HSER.setModule(1);
   pinMode(PC_4,INPUT);
  #else
   pinMode(3,INPUT);
  #endif
  HSER.begin(19200);
  PPM_PIN_INTERRUPT;
  digitalWrite(LED_ON_OK,HIGH);
}

void loop() {
  if((GUT-T)>RC_REFRESH_RATE){ 
    
    SIGN[0]=rcValue[1];
    SIGN[1]=rcValue[2];
    SIGN[2]=rcValue[3];
    SIGN[3]=rcValue[0];
    
    for(int i=4;i<RC_CHANS;i++){
      SIGN[i]=rcValue[i];
    }
    
    #if !defined(DEBUG)
    if(PPM_OK){
        msp_babel(MSP_SET_RAW_RC, 8,SIGN);
        PPM_OK=0;
    }
    #else
     for (int i=0;i<RC_CHANS;i++){
       HSER.println(rcValue[i]);
     }
     HSER.println(" ");
     delay(80);
    #endif
    
   T=GUT; 
  }
  GUT=millis();
}










