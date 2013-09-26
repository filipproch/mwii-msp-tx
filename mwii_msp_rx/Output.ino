void write_servo(){
  //CH order ROLL/PITCH/YAW/THROTTLE/AUX1/AUX2/AUX3AUX4
#if !defined (NO_FAILSAFE) 
  if(GUT-LAST_SUCCESFUL_RX<FAILSAFE_DELAY){
    ROLLS.writeMicroseconds(servo_val[0]);
    PITCHS.writeMicroseconds(servo_val[1]);
    YAWS.writeMicroseconds(servo_val[2]);
    THS.writeMicroseconds(servo_val[3]);
    AUX1S.writeMicroseconds(servo_val[4]);
    AUX2S.writeMicroseconds(servo_val[5]);
  }
  else{ //if failsafe timeout center channels and turn off motor
    ROLLS.writeMicroseconds(1500);
    PITCHS.writeMicroseconds(1500);
    YAWS.writeMicroseconds(1500);
    THS.writeMicroseconds(1000);
    AUX1S.writeMicroseconds(1500);
    AUX2S.writeMicroseconds(1500);
  }
#else
  ROLLS.writeMicroseconds(servo_val[0]);
  PITCHS.writeMicroseconds(servo_val[1]);
  YAWS.writeMicroseconds(servo_val[2]);
  THS.writeMicroseconds(servo_val[3]);
  AUX1S.writeMicroseconds(servo_val[4]);
  AUX2S.writeMicroseconds(servo_val[5]);
#endif
}




