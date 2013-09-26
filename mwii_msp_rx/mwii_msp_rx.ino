/*
MWII-MSP-RX by Andrej Javorsek
 Sept 2013
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 */
/*Code snippets taken directly from MultiWii http://www.multiwii.com */


#include "def.h"
#include <Servo.h>
Servo ROLLS;
Servo PITCHS;
Servo YAWS;
Servo THS;
Servo AUX1S;
Servo AUX2S;
//Servo S6;
//Servo S7;
//Servo S8;

int i=1000;
uint32_t GUT=0, T=0;
// CH. order ROLL/PITCH/YAW/THROTTLE/AUX1/AUX2/AUX3AUX4
volatile uint16_t servo_val[]={1500,1500,1500,1000,1500,1500,1500,1500};

void setup() {
  SER1_BEGIN;
  #if  defined(__AVR_ATmega2560__)
  SER_DBG_BEGIN;
  #endif
         ROLLS.attach(11);  // attaches the servo on pin 11 to the servo object MOSI
         PITCHS.attach(12); //MISO
         YAWS.attach(13); //SCK
         THS.attach(2); //INTERRUPT 0 (servo in)
         AUX1S.attach(4);
         AUX2S.attach(5);
}

void loop(){
  GUT=millis();
  serialCom();
  write_servo();
  T=GUT;
}
