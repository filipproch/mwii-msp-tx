void singndance(uint8_t pip){
 
  switch (pip){
  case 1:
    PN=NOTE_A5;
    if ((GUT-TG_SW) > 50){
      PN=NOTE_A6;
    }
    if ((GUT-TG_SW) > 100){
      PN=NOTE_B7;
    }
    if ((GUT-TG_SW) > 150){
      PN=0;
    }
    break;
    
  case 2:
    PN=NOTE_A5;
    if ((GUT-TG_SW) > 50){
      PN=NOTE_A6;
    }
    if ((GUT-TG_SW) > 100){
      PN=NOTE_B5;
    }
    if ((GUT-TG_SW) > 150){
      PN=0;
    }
    break;
    
  default:
    PN=0;
    break;
  }


  if (PN != 0){
    tone(BUZERPIN,PN);
  }
  else{
    noTone(BUZERPIN);
  }

}




