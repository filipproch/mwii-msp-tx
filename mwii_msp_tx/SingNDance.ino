uint8_t SINGNDANCE=0;
uint8_t VBON=0;

void singndance(uint8_t pip){

  switch (pip){
  case 1:
    PN=NOTE_A5;
    SINGNDANCE=1;
    if ((GUT-TG_SW) > 50){
      PN=NOTE_A6;
    }
    if ((GUT-TG_SW) > 100){
      PN=NOTE_B7;
    }
    if ((GUT-TG_SW) > 150){
      PN=0;
      SINGNDANCE=0;
    }
    break;

  case 2:
    SINGNDANCE=1;
    PN=NOTE_A5;
    if ((GUT-TG_SW) > 50){
      PN=NOTE_A6;
    }
    if ((GUT-TG_SW) > 100){
      PN=NOTE_B5;
    }
    if ((GUT-TG_SW) > 150){
      PN=0;
      SINGNDANCE=0;
    }
    break;

  case 3:
    SINGNDANCE=1;
    PN=NOTE_F1;
    if ((GUT-TG_SW) > 50){
      PN=0;
      SINGNDANCE=0;
    }
    break;

  default:
    PN=0;
    SINGNDANCE=0;
    break;
  }


  if (PN != 0){
    tone(BUZERPIN,PN);
  }
  else{
    noTone(BUZERPIN);
  }
}

void vario_sound(int16_t vario_beep){
  uint16_t nota=0;
  uint8_t scale=100;
  uint32_t bepdelay=constrain((uint32_t)(1000.0/((vario_beep/500.0)*scale)),100,2500);
  if(SINGNDANCE==0){
    if(vario_beep >= 5){
      if((GUT-VARIO_BEEP_D) > bepdelay){
      VBON=1;   
      }
      if((GUT-VARIO_BEEP_D) > (bepdelay+100)){
        VBON=0;
        VARIO_BEEP_D=GUT;
      }

      if(VBON){
        nota=440;
      }
      else{
        nota=0;
      }
    } 
    else if(vario_beep < -5) {
      nota=constrain((700+vario_beep),200,700);
    }
    else{
      nota=0;
    }
  }
  else{
    nota=0;
  }
  
  if(nota>0){
    tone(BUZERPIN,nota);
  }
  else if (SINGNDANCE==0 && nota==0){
    noTone(BUZERPIN);
  }
}




