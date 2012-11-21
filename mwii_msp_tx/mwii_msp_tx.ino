/*
MWII-MSP-TX by Andrej Javorsek
 November 2012
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */

#include <avr/pgmspace.h>
#include "tones.h"

//#define DEBUG

#define MSP_SET_RAW_RC           200
#define MSP_ALTITUDE             109
#define MSP_IDENT                100
#define MSP_BAT                  110

#define ROLLP A0
#define PITCHP A1
#define THP A2
#define YAWP A3
#define LED_REFRESH_RATE 1000/2  //in Hz
#define LED_ON_OK 9
#define BUZERPIN 6
#define BUZERPIN_VARIO 7
#define ANALOGMAX 626
#define ANALOGMIN 397
#define ANALOGMMD (ANALOGMAX-ANALOGMIN)

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)   ///debugging and printing option
#define MEGA
#define BUZERPIN 13
#endif

uint16_t MIDR=512, MIDP=512, MIDY=512; 
uint16_t DB=1500;
uint8_t AUXP[]={
  13,12,11,5};
uint8_t SIG=0;
uint32_t GUT=0 ,T=0, T_LED=0, TG_SW=0, ADP_DELAY=0, VARIO_BEEP_D=0; 
uint16_t ROLL=512, PITCH=512, TH=512, YAW=512;
uint16_t AUX[4]={
  1000,1000,1000,1000};
uint16_t AUXOLD[4];
uint16_t SIGN[8]={
  0,0,0,0};
uint16_t CONNECTION_OK=0;
uint8_t STANJE=0;
uint8_t ANLOGPINS[]={
  1,2,3,4};
uint16_t PN=0;
uint8_t  RC_FAST_REFRESH_RATE=(1000/40); //in Hz
uint8_t  RC_SLOW_REFRESH_RATE=(1000/18); //in Hz
uint8_t RC_REFRESH_RATE = RC_FAST_REFRESH_RATE;
uint8_t SLOW_RATE=0;
uint8_t taskOrder=0; //to call different serial stuff less often

//telemetry
int32_t ALTITUDE=0;  //in cm
int16_t VARIO=0;    //in cm/s
int8_t VBAT_V=0;
uint16_t VBAT_SUM=0;

uint8_t VERSION=0;
uint8_t MULTITYPE=0;
uint8_t MSP_VERSION=0;
uint32_t CAPAB=0;  //spektrum bind capable


void setup() {
  Serial.begin(19200);
#if defined(MEGA)
  Serial1.begin(19200);
#endif
  for (int i=0;i<4;i++){
    pinMode(AUXP[i],INPUT);
    digitalWrite(AUXP[i],HIGH);
  }

  MIDR=constrain(analogRead(ROLLP),ANALOGMIN,ANALOGMAX);  //find currently trimmed midpoints for adaptive TX frequency
  MIDP=constrain(analogRead(PITCHP),ANALOGMIN,ANALOGMAX);
  MIDY=constrain(analogRead(YAWP),ANALOGMIN,ANALOGMAX);
  
  msp_babel(MSP_IDENT, 0,0);
  getstatus();

  tone(BUZERPIN,NOTE_B5);
  delay(50);
  tone(BUZERPIN,NOTE_B6);
  delay(50);
  tone(BUZERPIN,NOTE_B7);
  delay(100);
  noTone(BUZERPIN);
}

void loop() {

  ROLL=constrain(analogRead(ROLLP),ANALOGMIN,ANALOGMAX);
  PITCH=constrain(analogRead(PITCHP),ANALOGMIN,ANALOGMAX);
  TH=constrain(analogRead(THP),ANALOGMIN,ANALOGMAX);
  YAW=constrain(analogRead(YAWP),ANALOGMIN,ANALOGMAX);

  uint16_t RDIFF=(uint16_t) (pow((MIDR-ROLL),2));
  uint16_t PDIFF=(uint16_t) (pow((MIDP-PITCH),2));
  //uint16_t YDIFF=(uint16_t) (pow((MIDY-YAW),2));

  if (RDIFF>DB || PDIFF>DB ){
    ADP_DELAY=GUT;
  }

  if (GUT>3000 && (GUT-ADP_DELAY)<3000){
    RC_REFRESH_RATE=RC_FAST_REFRESH_RATE;
    SLOW_RATE=0;
  }
  else{
    RC_REFRESH_RATE=RC_SLOW_REFRESH_RATE;
    SLOW_RATE=1;
  }


  if (SLOW_RATE !=1 ){
    CONNECTION_OK=0;
    analogWrite(LED_ON_OK,255);
  }

  if((GUT-T_LED)>LED_REFRESH_RATE){
    if (CONNECTION_OK<3){
      analogWrite(LED_ON_OK,10);
    }
    else{
      analogWrite(LED_ON_OK,0);
    }
    T_LED=GUT;
  }

#if defined(DEBUG)
  RC_REFRESH_RATE=(1000/10);
#endif

  if((GUT-T)>RC_REFRESH_RATE){

    ROLL=(uint16_t) (((double)(ANALOGMAX - ROLL + 1) / (ANALOGMMD) ) * 1000.0) + 1000;
    PITCH=(uint16_t) (((double)(ANALOGMAX - PITCH + 1) / (ANALOGMMD) ) * 1000.0) + 1000;
    TH=(uint16_t) (((double)(ANALOGMAX - TH + 1) / (ANALOGMMD) ) * 1000.0) + 1000;
    YAW=(uint16_t) (((double)(ANALOGMAX - YAW + 1) / (ANALOGMMD) ) * 1000.0) + 1000;

    for (int i=0;i<4;i++){
      AUX[i]=digitalRead(AUXP[i])*1000+1000;

      if (AUX[i]!=AUXOLD[i] && AUX[i]>AUXOLD[i]){
        TG_SW=GUT;
        AUXOLD[i]=AUX[i];
        SIG=1;
      }
      if (AUX[i]!=AUXOLD[i] && AUX[i]<AUXOLD[i]){
        TG_SW=GUT;
        AUXOLD[i]=AUX[i];
        SIG=2;
      }
    }

    SIGN[0]=ROLL;
    SIGN[1]=PITCH;
    SIGN[2]=YAW;
    SIGN[3]=TH;

    for(int i=4;i<8;i++){
      SIGN[i]=AUX[i-4];
    }

    if (!SLOW_RATE){
      msp_babel(MSP_SET_RAW_RC, 8,SIGN);
    }

    if (SLOW_RATE){
      switch (taskOrder % 11){
      case 0:
        taskOrder++;
        msp_babel(MSP_ALTITUDE, 0,0);
        getstatus();
        break;
      default:
        taskOrder++;
        msp_babel(MSP_SET_RAW_RC, 8,SIGN);
        break;
      }
    }

#if defined(DEBUG) || defined(MEGA)
    Serial.print(ALTITUDE,DEC);
    Serial.print(" ");
    Serial.println(VARIO,DEC);
    // Serial.println(ROLL,DEC);
    // Serial.println(" ");
#endif
    T=GUT;
  }  

  singndance(SIG);
  vario_sound(VARIO);

  GUT=millis();
}










