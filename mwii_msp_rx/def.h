//#define DEBUG
//#define HKF20A  //using brushless ESC for controler
//#define TP1  //using brushless ESC for controler
#define BS_PF_12A  //using brushless ESC for controler
//#define NO_FAILSAFE
//#define SHUNC24G
//Position of signal in PPM train, differs for TX
#define FAILSAFE_DELAY 2000 //2 sec

#define  ROLL     0
#define  PITCH    1
#define  YAW      2
#define  THROTTLE 3

#if defined(SHUNC24G)
#define SERIAL_SPEED 115200
#else
#define SERIAL_SPEED 19200
#endif

#define RC_CHANS 8
#define MSP_SET_RAW_RC 200


#if  defined(__AVR_ATmega2560__)
#define SER1_R Serial3.read()
#define SER1_AV Serial3.available()
#define SER_BEGIN Serial3.begin(SERIAL_SPEED); Serial.begin(SERIAL_SPEED);

#define SER_DBG_BEGIN Serial.begin(SERIAL_SPEED)
#define SER_DBG_AV Serial.available()
#define SER_DBG_R Serial.read()
#define DEBUG

#else
#define SER1_R Serial.read()
#define SER1_AV Serial.available()
#define SER_BEGIN Serial.begin(SERIAL_SPEED);
#endif

#if defined(TP1)
#define ROLL_P 11
#define PITCH_P 12
#define YAW_P 13
#define TH_P 2
#define AUX1_P 4
#define AUX2_P 5
#endif

#if defined(BS_PF_12A)
#define ROLL_P PB0
#define PITCH_P PC4
#define YAW_P PD5
#define TH_P PD2
#define AUX1_P PD4
#define AUX2_P PC3
#endif

#define ROLL_ATTACH  ROLLS.attach(ROLL_P);  
#define PITCH_ATTACH  PITCHS.attach(PITCH_P);
#define YAW_ATTACH  YAWS.attach(YAW_P); 
#define TH_ATTACH  THS.attach(TH_P); 
#define AUX1_ATTACH  AUX1S.attach(AUX1_P);
#define AUX2_ATTACH  AUX2S.attach(AUX2_P);



