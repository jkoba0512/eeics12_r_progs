// pin number
// motor1
const int PIN_MOTOR_DIR_1 = 2;  // dir
const int PIN_MOTOR_PWM_1 = 3;  // pwm

// pwm_com (-255 ~ 255)
int pwm_com_1 = 0;

void setup() {
  // change PWM freq
  TCCR1B=TCCR1B&0xf8|0x01;  // pin9,pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;  // pin3,pin11 PWM 31250Hz

  // set pin mode
  pinMode(PIN_MOTOR_PWM_1, OUTPUT);
  pinMode(PIN_MOTOR_DIR_1, OUTPUT);

  // serial
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'q':
        pwm_com_1 += 1;
        break;
      case 's':
        pwm_com_1 = 0;
        break;
      case 'd':
        pwm_com_1 -= 1;
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

    // set direction
    if (pwm_com_1 >= 0) {
      digitalWrite(PIN_MOTOR_DIR_1, 0);  // CW
    }
    else {
      digitalWrite(PIN_MOTOR_DIR_1, 1);  // CCW
    }

    // output pwm
    analogWrite(PIN_MOTOR_PWM_1, abs(pwm_com_1));

    // print pwm_com_1
    Serial.println(pwm_com_1);
  }
}
