#include <PinChangeInt.h>

// pin number
// motor1
const int PIN_MOTOR_DIR_1 = 2;  // dir
const int PIN_MOTOR_PWM_1 = 3;  // pwm
const int PIN_ENCODER_A_1 = 4;  // encoder A
const int PIN_ENCODER_B_1 = 5;  // encoder B

// pwm_com (-255 ~ 255)
int pwm_com_1 = 0;

// encorder pulse count
volatile int enc_counting_1 = 0;  // inc/dec by interrupt handler
volatile int enc_count_1 = 0;     // for control law

// control period (us)
const int T_S = 10000;

// for keep control period
long int start_time, end_time;

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
  if (digitalRead(PIN_ENCODER_B_1) == LOW) enc_count_1++;  // CW
  else enc_count_1--;  // CCW
}

void loop() {
  start_time = micros();  // start control period

  // update enc_count_1
  enc_count_1 = enc_counting_1;
  enc_counting_1 = 0;

  // set direction
  if (pwm_com_1 >= 0) {
    digitalWrite(PIN_MOTOR_DIR_1, 0);  // CW
  }
  else {
    digitalWrite(PIN_MOTOR_DIR_1, 1);  // CCW
  }

  // output pwm
  analogWrite(PIN_MOTOR_PWM_1, abs(pwm_com_1));

  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'a':
        pwm_com_1 = limitCOM(pwm_com_1 + 10);
        break;
      case 's':
        pwm_com_1 = 0;
        break;
      case 'd':
        pwm_com_1 = limitCOM(pwm_com_1 - 10);
        break;
      case 'w':
        pwm_com_1 = 255;
        break;
      case 'x':
        pwm_com_1 = - 255;
        break;
      default:
        ;
    }

    // print pwm_com_1
    Serial.println(pwm_com_1, enc_count_1);
  }

  // for keep control period
  end_time = micros();
  delayMicroseconds(max(0, start_time + T_S - end_time));
}

int limitCOM(int _com) {
  if (_com >  255) return 255;
  if (_com < -255) return -255;
  return _com;
}
