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

#define MSP_SET_RAW_RC 200
#define MSP_ALTITUDE  109
#define ROLLP A0  //644 512  371
#define PITCHP A1  // 643 512 376
#define THP A2 ///373 512 650
#define YAWP A3 // 647 512 370        370  512  650 t123

#define RC_REFRESH_RATE 1000/20 //v Hz
#define LED_REFRESH_RATE 1000/2  //v Hz
#define LED_ON_OK 9
#define BUZERPIN 6

uint8_t AUXP[]={
  13,12,11,5};
uint8_t SIG=0;
uint32_t GUT=0 ,T=0, T_LED=0, TG_SW=0; 
uint16_t ROLL=1500, PITCH=1500, TH=1000, YAW=1500;
uint16_t AUX[4]={
  1000,1000,1000,1000};
uint16_t AUXOLD[4];
uint16_t SIGN[8]={
  0,0,0,0};
uint8_t CONNECTION_OK=0;
uint8_t STANJE=0;
uint8_t ANLOGPINS[]={
  1,2,3,4};
uint16_t ANLOGMINMAX[4][2];
 uint16_t PN=0;


void setup() {
  Serial.begin(9600);
  for (int m=0;m<4;m++){
    ANLOGMINMAX[m][0]=384;
    ANLOGMINMAX[m][1]=640;
  }

  //stickcal();
  for (int i=0;i<4;i++){
    pinMode(AUXP[i],INPUT);
    digitalWrite(AUXP[i],HIGH);
  }

  tone(BUZERPIN,NOTE_B5);
  delay(50);
  tone(BUZERPIN,NOTE_B6);
  delay(50);
  tone(BUZERPIN,NOTE_B7);
  delay(100);
  noTone(BUZERPIN);
}

void loop() { 

  CONNECTION_OK=1;
  if((GUT-T_LED)>LED_REFRESH_RATE){
    //getstatus();
    if (CONNECTION_OK){
      analogWrite(LED_ON_OK,40);
    }
    else{
      analogWrite(LED_ON_OK,0);
    }
    T_LED=GUT;
  }

  if((GUT-T)>RC_REFRESH_RATE){    //moved all anolog/digital reading/calculating inside 20Hz loop

    ROLL=(uint16_t) (((double)(ANLOGMINMAX[0][1] - analogRead(ROLLP)) / (ANLOGMINMAX[0][1]-ANLOGMINMAX[0][0]) ) * 1050.0) + 1000;
    PITCH=(uint16_t) (((double)(ANLOGMINMAX[1][1] - analogRead(PITCHP)) / (ANLOGMINMAX[1][1]-ANLOGMINMAX[1][0]) ) * 1050.0) + 1000;
    TH=(uint16_t) (((double)(ANLOGMINMAX[2][1] - analogRead(THP)) / (ANLOGMINMAX[2][1]-ANLOGMINMAX[2][0]) ) * 1050.0) + 1000;
    YAW=(uint16_t) (((double)(ANLOGMINMAX[3][1] - analogRead(YAWP)) / (ANLOGMINMAX[3][1]-ANLOGMINMAX[3][0]) ) * 1050.0) + 1000;

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

    msp_babel(SIGN);
    T=GUT;
  }

  singndance(SIG);

  GUT=millis();
}





