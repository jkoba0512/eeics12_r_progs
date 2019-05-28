#include <PinChangeInt.h>

// pin number
// motor1
const int PIN_MOTOR_DIR_1 = 2;  // dir
const int PIN_MOTOR_PWM_1 = 3;  // pwm
const int PIN_ENCODER_A_1 = 4;  // encoder A
const int PIN_ENCODER_B_1 = 5;  // encoder B
// motor2
const int PIN_MOTOR_DIR_2 = 12;  // dir
const int PIN_MOTOR_PWM_2 = 11;  // pwm
const int PIN_ENCODER_A_2 = 6;  // encoder A
const int PIN_ENCODER_B_2 = 7;  // encoder B

volatile int enc_counting_1 = 0;  // encoder pulse count
volatile int enc_counting_2 = 0;  // inc/dec by interrupt handler

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
  pinMode(PIN_MOTOR_PWM_2, OUTPUT);
  pinMode(PIN_MOTOR_DIR_2, OUTPUT);
  pinMode(PIN_ENCODER_A_2, INPUT);
  pinMode(PIN_ENCODER_B_2, INPUT);

  // set a handler for pin interrupt (for encoder pulse measurement)
  PCintPort::attachPinChangeInterrupt(PIN_ENCODER_A_1, riseEncA1, RISING);
  PCintPort::attachPinChangeInterrupt(PIN_ENCODER_A_2, riseEncA2, RISING);

  // serial
  Serial.begin(9600);
}

void riseEncA1() {
  // write a code here
  // 
}

void riseEncA2() {
  // write a code here
  // 
}

void loop() {
  // for keep control period
  long int start_time, end_time;

  // encorder pulse count for control law
  int enc_count_1 = 0;
  int enc_count_2 = 0;

  // pwm duty ratio including direction (-255 ~ 255)
  static int pwm_com_1 = 0;
  static int pwm_com_2 = 0;
  
  // desired motor rotational velocity (deg/s)
  static float desired_dphi_deg_s_1 = 0.0;
  static float desired_dphi_deg_s_2 = 0.0;

  // desired mobile robot translational and rotational velocities (m/s, rad/s)
  static float desired_vel_m_s = 0.0;
  static float desired_omega_rad_s = 0.0;

  // start control period
  start_time = micros();

  // update enc_count_1, enc_count_2
  enc_count_1 = enc_counting_1;
  enc_counting_1 = 0;
  enc_count_2 = enc_counting_2;
  enc_counting_2 = 0;

  // set direction
  if (pwm_com_1 >= 0) {
    // write a code here  // CW
  }
  else {
    // write a code here  // CCW
  }
  if (pwm_com_2 >= 0) {
    // write a code here  // CW
  }
  else {
    // write a code here  // CCW
  }

  // output pwm
  // write a code here
  // write a code here

  // process key inputs
  if (Serial.available() > 0) {
    processSerialKeyInputForMobileRobotControl(desired_vel_m_s, desired_omega_rad_s);
    Serial.print(desired_vel_m_s);
    Serial.print(" ");
    Serial.println(desired_omega_rad_s);
  }

  // convert desired mobile robot velocities
  // to desired motor rotational velocities
  static float radius_wheel = 0.05;
  static float distance_wheels = 0.23;
  desired_dphi_deg_s_1 = 0.0;  // modify this code here
  desired_dphi_deg_s_2 = 0.0;  // modify this code here
  desired_dphi_deg_s_2 = - desired_dphi_deg_s_2;
  
  // PI control
  pwm_com_1 = limitCOM(calPIControl1(desired_dphi_deg_s_1, enc_count_1));
  pwm_com_2 = limitCOM(calPIControl2(desired_dphi_deg_s_2, enc_count_2));
  
  // for keep control period
  end_time = micros();
  delayMicroseconds(max(0, start_time + T_S - end_time));
}

int limitCOM(int _com) {
  if (_com >  255) return 255;
  if (_com < -255) return -255;
  return _com;
}

void processSerialKeyInputForMobileRobotControl(float& desired_vel_m_s, float& desired_omega_rad_s) {
  switch (Serial.read()) {
    case 'w':
      desired_vel_m_s += 0.1;
      break;
    case 'x':
      desired_vel_m_s -= 0.1;
      break;
    case 'a':
      desired_omega_rad_s += 60.0 * (PI / 180.0);
      break;
    case 'd':
      desired_omega_rad_s -= 60.0 * (PI / 180.0);
      break;
    case 's':
      desired_vel_m_s = 0.0;
      desired_omega_rad_s = 0.0;
      break;
    default:
      ;
  }
}

int calPIControl1(float _desired_dphi_deg_s_1, int _enc_count_1) {
  // arguments
  // _desired_dphi_deg_s_1 : desired motor rotational velocity (deg/s)
  // _enc_count_1 : encorder pulse count for control law
    
  // PI gains
  const float Kp = 1.0;
  const float Ki = 0.0;

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

  // PI control
  return (int)(0.0);  // modify this code here
}

int calPIControl2(float _desired_dphi_deg_s_2, int _enc_count_2) {
  // arguments
  // _desired_dphi_deg_s_2 : desired motor rotational velocity (deg/s)
  // _enc_count_2 : encorder pulse count for control law
  
  // PI gains
  const float Kp = 1.0;
  const float Ki = 0.0;

  // errors
  int err_enc_count_2 = 0;  // error (desired - actual)
  static int acc_err_enc_count_2 = 0;  // accumulated error
  
  // convert unit
  const int GEAR_RATIO = 64;
  const int RES_ENC = 12;
  const float DEG_S_2_COUNT = ((GEAR_RATIO * RES_ENC) / 360.0) * (T_S / (1000.0 * 1000));
  int desired_enc_count_2 = (int)(_desired_dphi_deg_s_2 * DEG_S_2_COUNT);

  // error (desired - actual)
  // write a code here
  
  // accumulated error
  // write a code here

  // PI control
  return (int)(0.0);  // modify this code here
}
