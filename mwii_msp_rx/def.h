//#define DEBUG
#define HKF20A  //using brushless ESC for controler
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
#define SER1_BEGIN Serial3.begin(SERIAL_SPEED)

#define SER_DBG_BEGIN Serial.begin(SERIAL_SPEED)
#define SER_DBG_AV Serial.available()
#define SER_DBG_R Serial.read()
#define DEBUG

#else
#define SER1_R Serial.read()
#define SER1_AV Serial.available()
#define SER1_BEGIN Serial.begin(SERIAL_SPEED)
#endif
