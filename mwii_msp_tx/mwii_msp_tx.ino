#include <avr/pgmspace.h>

#define MSP_SET_RAW_RC 200
#define ROLLP A0  //644 512  371
#define PITCHP A1  // 643 512 376
#define THP A2 ///373 512 650
#define YAWP A3 // 647 512 370        370  512  650 t12

#define RC_REFRESH_RATE 1000/50 //v Hz
#define LED_REFRESH_RATE 1000/1  //v Hz
#define LED_ON_OK 9
#define BUZERPIN 6

uint8_t AUXP[]={10,11,12,5};
uint32_t GUT=0 ,T=0, T_LED=0; 
uint16_t ROLL=1500, PITCH=1500, TH=1000, YAW=1500;
uint16_t AUX[4]={1000,1000,1000,1000};
uint16_t SIGN[8]={0,0,0,0};
uint8_t CONNECTION_OK=0;
uint8_t STANJE=0;
uint8_t ANLOGPINS[]={1,2,3,4};
uint16_t ANLOGMINMAX[4][2];


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
    analogWrite(BUZERPIN,150);
    delay(50);
    analogWrite(BUZERPIN,0);
  }
  pinMode(LED_ON_OK,OUTPUT);
}

void loop() { 

  if((GUT-T_LED)>LED_REFRESH_RATE){
    if (CONNECTION_OK){
      STANJE=1;
    }
    else{
      STANJE=!STANJE;
    }
    T_LED=GUT;
    digitalWrite(LED_ON_OK,STANJE);
  }

  //                           (max - vrednost) / (max-min))
  //                            ((vrednost - (max - (max-min)/2))/((max-min)/2)*1000)+1500
  //ROLL=((4000*analogRead(ROLLP))/(2*(ANLOGMINMAX[0][1]-ANLOGMINMAX[0][0])))+1499;
  ROLL=(uint16_t) (((double)(ANLOGMINMAX[0][1] - analogRead(ROLLP)) / (ANLOGMINMAX[0][1]-ANLOGMINMAX[0][0]) ) * 1050.0) + 1000;
  PITCH=(uint16_t) (((double)(ANLOGMINMAX[1][1] - analogRead(PITCHP)) / (ANLOGMINMAX[1][1]-ANLOGMINMAX[1][0]) ) * 1050.0) + 1000;
  TH=(uint16_t) (((double)(ANLOGMINMAX[2][1] - analogRead(THP)) / (ANLOGMINMAX[2][1]-ANLOGMINMAX[2][0]) ) * 1050.0) + 1000;
  YAW=(uint16_t) (((double)(ANLOGMINMAX[3][1] - analogRead(YAWP)) / (ANLOGMINMAX[3][1]-ANLOGMINMAX[3][0]) ) * 1050.0) + 1000;

 for (int i=0;i<4;i++){
    AUX[i]=digitalRead(AUXP[i])*1000+1000;
  }

  SIGN[0]=ROLL;
  SIGN[1]=PITCH;
  SIGN[2]=YAW;
  SIGN[3]=TH;
  
  for(int i=4;i<8;i++){
    SIGN[i]=AUX[i-4];
  }

  if((GUT-T)>RC_REFRESH_RATE){
    msp_babel(SIGN);
     
     /**(for (int k=0;k<4;k++){
       Serial.println(SIGN[k],DEC);
       //Serial.println(ANLOGMINMAX[0][1],DEC);
       //Serial.println(ANLOGMINMAX[0][0],DEC);
     }
     Serial.println(" ");**/
     
    T=GUT;
  }


  GUT=millis();
}

