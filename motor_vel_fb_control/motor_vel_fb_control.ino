#include <PinChangeInt.h>

// pin number
// motor1
const int PIN_MOTOR_DIR_1 = 2;  // dir
const int PIN_MOTOR_PWM_1 = 3;  // pwm
const int PIN_ENCODER_A_1 = 4;  // encoder A
const int PIN_ENCODER_B_1 = 5;  // encoder B

volatile int enc_counting_1 = 0;  // encoder pulse count
                                  // inc/dec by interrupt handler

const int T_S = 10000;  // control period (us)

void setup() {
  // change PWM freq
  TCCR1B=TCCR1B&0xf8|0x01;  // pin9,pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;  // pin3,pin11 PWM 31250Hz

  // set pin mode
  pinMode(PIN_MOTOR_PWM_1, OUTPUT);
  pinMode(PIN_MOTOR_DIR_1, OUTPUT);
  pinMode(PIN_ENCODER_A_1, INPUT);
  pinMode(PIN_ENCODER_B_1, INPUT);

  // set a handler for pin interrupt (for encoder pulse measurement)
  PCintPort::attachPinChangeInterrupt(PIN_ENCODER_A_1, riseEncA1, RISING);

  // serial
  Serial.begin(9600);
}

void riseEncA1() {
  // write a code here
  // 
}

void loop() {
  // for keep control period
  long int start_time, end_time;

  // encorder pulse count for control law
  int enc_count_1 = 0;

  // pwm duty ratio including direction (-255 ~ 255)
  static int pwm_com_1 = 0;
  
  // desired motor rotational velocity (deg/s)
  static float desired_dphi_deg_s_1 = 0.0;

  // start control period
  start_time = micros();

  // update enc_count_1
  enc_count_1 = enc_counting_1;
  enc_counting_1 = 0;

  // set direction
  if (pwm_com_1 >= 0) {
    // write a code here  // CW
  }
  else {
    // write a code here  // CCW
  }

  // output pwm
  // write a code here

  // process key inputs
  if (Serial.available() > 0) {
    desired_dphi_deg_s_1 = processSerialKeyInputForMotorControl(desired_dphi_deg_s_1);
  }

  // PI control
  pwm_com_1 = limitCOM(calPIControl(desired_dphi_deg_s_1, enc_count_1));
  
  // for keep control period
  end_time = micros();
  delayMicroseconds(max(0, start_time + T_S - end_time));
}

int limitCOM(int _com) {
  if (_com >  255) return 255;
  if (_com < -255) return -255;
  return _com;
}

float processSerialKeyInputForMotorControl(float _desired_dphi_deg_s_1) {
  switch (Serial.read()) {
    case 'a':
      return _desired_dphi_deg_s_1 + 90.0;
    case 's':
      return 0.0;
    case 'd':
      return _desired_dphi_deg_s_1 - 90.0;
    default:
      return _desired_dphi_deg_s_1;
  }
}

int calPIControl(float _desired_dphi_deg_s_1, int _enc_count_1) {
  // PI gains
  const float Kp = 1;
  const float Ki = 0;

  // errors
  int err_enc_count_1 = 0;  // error (desired - actual)
  static int acc_err_enc_count_1 = 0;  // accumulated error
  
  // convert unit
  const int GEAR_RATIO = 64;
  const int RES_ENC = 12;
  const float DEG_S_2_COUNT = ((GEAR_RATIO * RES_ENC) / 360.0) * (T_S / (1000.0 * 1000));
  int desired_enc_count_1 = (int)(_desired_dphi_deg_s_1 * DEG_S_2_COUNT);

  // error (desired - actual)
  // write a code here
  
  // accumulated error
  // write a code here
  
  // print
  Serial.print(_desired_dphi_deg_s_1);
  Serial.print(" ");
  Serial.print(desired_enc_count_1);
  Serial.print(" ");
  Serial.println(_enc_count_1);
  
  // PI control
  return (int)(0.0);  // modify this code here
}
